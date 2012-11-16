#pragma once

#include <boost/thread.hpp>
#include <boost/bind.hpp>
#include <string>
#include <vector>
#include "LogFile.h"
using namespace std;

enum ENDSTATE
{
	NONE,
	ABORT,
	UNSHORTEQUAL,
	UNLONGEQUAL,
	OK
};

//�ṩ�ڲ��̻߳���
class UrbanSimulator
{
public:
	UrbanSimulator(void);
	~UrbanSimulator(void);
	
	//������Ϣ
	string					m_folder;
	vector<const char *>	m_errs;
	LogFile					m_log;
	//�������
	float	m_alpha;
	float	m_beta;
	float	m_delta;
	float	m_gamma;
	int		m_n;
	float	m_L;
	float	m_phi;
	float	m_rho;
	float	m_sigma;
	float	m_sp;
	double	m_tc;
	float	m_w0;
	long	m_grdstep;
	int		m_shortit;
	int		m_longit;

	/////////////////////////////////////
	void Init();
	bool Calc();
	bool CalcImpl();
	bool IsDone();
	float GetPercentage();
	ENDSTATE GetEndState();

	void Reset(){}
	bool StopCalc();
private:
	ENDSTATE m_endState;	//������˳�״̬
	
	float	m_percentage;	//�����Ѿ�ִ�еİٷֱ�
	bool	m_isRunning;		//�߳��Ƿ���ִ��״̬
	bool	m_isDone;			//�߳��Ƿ����
	bool	m_isWaitingExit;	//�Ƿ�ȴ��˳�

	boost::mutex				m_mutex;
	boost::thread*			m_pThread;

	//�������������Ҫ�ı���
	vector<double> m_G, m_D, m_NW, m_iphi, m_Y, m_I, m_WR, m_RW;
	vector<double> m_lambda[9];

	/****************************
	���п��Ƹ���
	****************************/
	void SetPercentage(float v);
	void SetDone(bool v);
	void SetRunning(bool v);

	/***************************
	�������
	***************************/
	void LoadInitialData();	//��ʼ����־�ļ�
	pair<bool, int> ShortEql();	//���ھ��⣬����ֵ���Ƿ���ھ���/���������ѭ������
};
