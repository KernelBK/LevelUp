// ConcurrentProgrammingTest.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <windows.h>
#include <boost/thread.hpp>
#include <iostream>
#include <time.h>
using namespace boost;

int CONCURRENT_NUM = 5;//��������
int BUFFER_SIZE = 10;//�����С
int INPUT_DATA_NUM = 100;//�������ݵ�����
int sleep_time = 200; //���ò��Գ��������ʱ�䳤��

mutex mtx;
condition cond;

volatile int get_pos = 0;  //��ǰ��ȡλ��
volatile int put_pos = 0;  //��ǰ���λ��
volatile int blank_num = BUFFER_SIZE; //����Ŀ�������Ĵ�С
volatile int updated_pos = -1;  //�Ѿ����µ�λ��
volatile int updating_pos = 0; //���ڸ��µ�λ��
double total_updating_time = 0.0;

int buffer[1024]; //

//���߳�
void get()
{
	for(int i = 0; i < INPUT_DATA_NUM; ++i)
	{
		{
			mutex::scoped_lock lock(mtx);
			while(blank_num == 0){
				cond.wait(lock);
			}
		}

		//��ȡֵ,�˲��������ȡ������¿���ʵ�ֲ���
		buffer[get_pos % BUFFER_SIZE] = get_pos;

		{
			mutex::scoped_lock lock(mtx);
			++get_pos;
			--blank_num;
		}

		cond.notify_all();
	}

	//�˳�ǰ����get_pos����������ŵĺ�һλ�����������ú���ı�ʾ�ҵ���ȷ�Ľ�β��
	{
			mutex::scoped_lock lock(mtx);
			get_pos = INPUT_DATA_NUM;
	}
}

//���߳�
void put()
{
	int put_start;

	while(1){
		{
			mutex::scoped_lock lock(mtx);

			put_start = put_pos % BUFFER_SIZE;
			while(put_pos > updated_pos){
				if(put_pos >= INPUT_DATA_NUM)
				{
					//std::cout << "Thread 'put' Terminated seccessful!" << std::endl; 
					cond.notify_all();
					return;
				}
				cond.wait(lock);
			}
		}

		//��ȡֵ,�˲��������ȡ������¿���ʵ�ֲ���
		//std::cout << buffer[put_start] << " ";

		{
			mutex::scoped_lock lock(mtx);
			++put_pos;
			++blank_num;
			//std::cout << blank_num << " " << put_pos << " " << updated_pos << std::endl;
		}

		cond.notify_all();
	}
}

//������ģ��
void update()
{
	int current_updating_pos;
	while(1) {
		//��һ��������������
		{
			mutex::scoped_lock lock(mtx);

			//�ж��Ƿ������ݿɹ�����
			while(updating_pos >= get_pos){
				if(updating_pos >= INPUT_DATA_NUM){
					//std::cout << "Thread 'update' Terminated seccessful!" << std::endl; 
					cond.notify_all();
					return;
				}
				cond.wait(lock);
			}
			current_updating_pos = updating_pos++;
		}

		clock_t start = clock();
		//����
		Sleep(sleep_time);
		buffer[current_updating_pos % BUFFER_SIZE] *= 2;
		clock_t end = clock();
		//�ڶ���������д��������֤˳���д��������
		{
			mutex::scoped_lock lock(mtx);
			total_updating_time += double(end - start)/double(CLOCKS_PER_SEC);
			while(current_updating_pos > updated_pos + 1){
				cond.wait(lock);
			}
			++updated_pos;
			//std::cout << current_updating_pos << " " << updated_pos << std::endl;
		}

		cond.notify_all();
	}
}

void InitData()
{
	get_pos = 0;
	put_pos = 0;
	blank_num = BUFFER_SIZE;
	updated_pos = -1;
	updating_pos = 0;
	total_updating_time = 0.0;
}

int _tmain(int argc, _TCHAR* argv[])
{	
	double st_eval_time, mt_eval_time, test_time;
	FILE* f = fopen("data.txt", "a");
	while(1){
		//��������
		std::cout << "���������벢�����������������������θ��º�ʱ�������С��" << std::endl;
		std::cin >> CONCURRENT_NUM >> INPUT_DATA_NUM >> sleep_time >> BUFFER_SIZE;
		
		if(CONCURRENT_NUM == 0){ break; }

		InitData();
		HANDLE hProc = GetCurrentProcess();
		SetPriorityClass(hProc, REALTIME_PRIORITY_CLASS);
		clock_t start, end;

		//���Կ�ʼ-------------------------------------
		start = clock();

		thread_group tg;
		tg.create_thread(&get);
		for(int i = 0; i < CONCURRENT_NUM; ++i){
			tg.create_thread(&update);
		}
		tg.create_thread(&put);
		tg.join_all();

		end = clock();
		//���Խ���-------------------------------------

		//������
		test_time = double (end - start) / double(CLOCKS_PER_SEC);
		st_eval_time = double(INPUT_DATA_NUM * sleep_time / 1000.0);
		mt_eval_time = double(INPUT_DATA_NUM * sleep_time) / double(CONCURRENT_NUM) / 1000.0;
		std::cout << "���Ժ�ʱ " << test_time << "s"<< std::endl;
		std::cout << "���߳�Ԥ��ʱ�䣺 " << st_eval_time << "s" << std::endl;
		std::cout << "��������ʱ�䣺 " << mt_eval_time << "s" << std::endl;
		std::cout << "���²���ռ��ʱ��ͣ�" << total_updating_time << "s" << std::endl;

		fprintf(f, "%d    %d    %d    %d    %.3f    %.3f    %.3f    %.3f\n",  CONCURRENT_NUM, INPUT_DATA_NUM, sleep_time, BUFFER_SIZE, test_time, st_eval_time, mt_eval_time, total_updating_time);
;
		SetPriorityClass(hProc, NORMAL_PRIORITY_CLASS);
	}
	fclose(f);
	system("pause");
	return 0;
}

