#ifndef PROJECT_H_INCLUDED
#define PROJECT_H_INCLUDED

#include "common.h"

#include <string>
#include <vector>

class preprocessor
{
	/**
	ʹ�ñ������Դ�ļ�����Ϊ���룬�����һ��Ԥ��������ʱ�ļ���
	Ԥ�������ļ���#lineָ���ⲻ��Я������Ԥ����ָ�
	�ú�������Ϊ�̰߳�ȫ�ĺ��������Զ��߳�ִ�С�
	**/
	virtual std::string preprocess(const std::string& filename) = 0;
};

class error_reporter_tags{
public:
	static std::string compiler_config_error();
	static std::string syntax_error();
};

/***
�ṩ��Error Reportor�Ľӿڣ������ṩ�˱�׼Error Reportor�ĸ�ʽ�ĸ����ַ�����Ϣ��
Error Report��ʵ����ͨ����Ҫ������¹�����
	������;�����������ڵ��÷���ó���������ģ�
	�������Ľ��з���������Error Content��ʽ���ɿɶ��͹淶����ʽ��
	���к�����Ϊ����ִ�к�����
***/
class error_reporter
{
public:
	/**
	������������ڲ�����
	��Щ����ķ����������ɱ����������BUG����������
	**/
	virtual void report_compiler_internal_error(const std::string& error_content) = 0;

	/**
	���汻����������﷨����
	**/
	virtual void report_syntax_error(const std::string& errContent) = 0;

	/**
	����һ��error_reporter����ͬ���ͣ����ǳ�ʼ����Ϊ�ա�
	**/
	virtual error_reporter* clone() = 0;
};

/**
����Ļ�����λ��
**/
class unit
{
public:
	unit();

	///	��õ�ǰ�������ġ�
	context* get_current_context();

	///	���ȫ�ַ��ű�
	scope* get_global_scope();

	/// ����һ��Unit
	bool parse(const std::string& filename);

	/// ������Ӧ��Error Report
	error_reporter* get_error_reporter();

private:
	h_context			context_;
	h_symbol_table		symtbl_;
	h_error_reporter	error_reporter_;

};

/***
һ��������һ������������̵���С��λ����������һ����Դ�ļ���·�����Լ����������ڲ�ͬ���õı�����������
ÿ��������Ӧ�ó���ʵ��ֻ������һ�����̡�
�������ܹ���֧���ļ�����Ĳ����﷨������������������ӽ׶���Ȼֻ�ܴ�����ɡ�
Ŀǰ��ʵ��Ϊ���̷߳�ʽ��
***/
class project
{
public:
	static project& instance();

	/**
	����preprocessor��ErrorReportִ�г�ʼ�����̡�
	�ú�������������������ǰ������Ҫ��ִ��һ�Ρ�
	**/
	static void initialize(h_preprocessor pp, h_error_reporter err_reporter);
	std::vector<h_unit>& get_units();

	/**
		�Կ�ܽ����﷨�����������Ӧ���﷨����
		�������configName�����ѡ����Ӧ�����ý��з�����
		��������configName�����ڣ����߷������̳��ִ���������Զ�ֹͣ��
		��������configNameΪ�գ���ѡȡ���̵ĵ�һ��Config��
	**/
	void parse(const std::string& config_name = "");

	///����Project��preprocessor
	preprocessor* get_pp();

	///����Project��Error Rporter
	error_reporter* get_error_reporter();

private:
	/**
	��ȡһ����֪ID�ı��������á���������ļ��������򷵻ؿա�
	�����ɿ�ܵ��á���������������saslc�����е���������
	**/
	configuration* get_config(const std::string& config_name);

	///���ݴ�������ý��з�����
	bool parse_units(configuration* pconf);

	///���ӣ�����Ŀ����롣
	bool link(configuration* pconf);

	project();
	~project();

	static project		prj_;

	h_preprocessor		pp_;
	h_error_reporter	err_reporter_;
	h_makefile			mf_;
	std::vector<h_unit>	units_;
};

//project::Instance��һ���򻯵��÷�װ��
inline project& proj()
{
	return project::instance();
}

#endif // PROJECT_H_INCLUDED
