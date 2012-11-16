/**
 * �ṩ��һ��Դ�뼶��ļ��׵�Ԫ���Թ���,�ṩһ���򵥵Ĳ��Ա��档
 * 
 * Copyright (c) 2007 by ������ת 
 * Email: wuye9036@gmail.com
 * ��������Ȩ��
 */
#pragma once
#define TIXML_USE_STL

#include <vector>
#include <string>
#include <tinyxml.h>

using namespace std;

enum TEST_STATE {
	UNKNOWN,
	PASS,
	FAILURE
};

/**��Assert�Ĳ��Խ��*/
struct SingleTestReport{
	string	content;	///< ����������(���ʽ)������
	bool	isPass;		///< ���Խ��

	SingleTestReport(const string& content, bool isPass);
	SingleTestReport(const SingleTestReport& rhs) ;

	const SingleTestReport& operator = (const SingleTestReport& rhs);
};

/**�������Է����Ľ��*/
struct TestMethodReport{
	vector<SingleTestReport> testItemReports;	///< ���Է��������еĲ��������б�
	string	testMethodName;								///< ���Է�������
	int		testItemCount;									///< ������Ŀ��
	int		passedItemCount;								///< ͨ����Ŀ��
	int		failedItemCount;									///< ʧ����Ŀ��
	bool	isPass;												///< ���Է����Ƿ�ͨ��(ֻ�����еĲ�����ͨ�������������Է���ͨ��)

	TestMethodReport();
	TestMethodReport(const TestMethodReport& rhs);
	const TestMethodReport& operator = (const TestMethodReport& rhs);
};

/** ����ִ�в��Է����ĺ���,�ռ��������� */
typedef bool (*PTestMethodFunc)(TestMethodReport*);

/** ��������,����������Է��� */
class STestCase
{
	string m_testCaseName;												///< ������������

	vector<PTestMethodFunc>	m_testMethodList;				///< ���Է����б�
	vector<TestMethodReport>	m_testMethodReports;			///< ����������ȫ�����Է����������

	int m_passedTestMethodCount;									///< ͨ���Ĳ���������
	int m_failedTestMethodCount;										///< ʧ�ܵĲ���������

public:
	STestCase(const string& caseName);

	void AddMethod(PTestMethodFunc pf);
	void ExecuteTest();
	void PrintReportToConsole();
	void SaveTestReport(const string& logf);

	/** ����ΪXML�ڵ� */
#ifdef TINYXML_INCLUDED
	void SaveTestReport(TiXmlNode* pNode);
#endif
};

/** �����׼������������������ */
class STestSuite
{
	vector<STestCase*> m_testCaseList;		///< ���������б�
	
	string m_testSuiteName;						///< �����׼�����

#ifdef TINYXML_INCLUDED
	TiXmlDocument* m_pXmlDoc;				///< XML�ĵ������ڱ�����Խ��
#endif

public:
	STestSuite(const string& suiteName);

	void AddTestCase(STestCase* pTestCase);
	void ExecuteTest();
	
#ifdef TINYXML_INCLUDED
	/** ��XML��ʽ������Ա��� */
	void MakeXMLDocument();
	void SaveTestReportXML(const string& fname);
#endif
};
#define CONTACT(s1, s2) s1##s2
#define CONTACT_S(s1, s2) CONTACT(s1, s2)
#define AUTO_GEN(s1) CONTACT_S(s1, __COUNTER__)

/**��ʼ��������*/
#define BEGIN_TESTCASE(tcName) \
	STestCase tcName(#tcName); \
	class tcName##Setting {\
		STestCase* pTestCase;\
	public:\
			   tcName##Setting() : pTestCase(&tcName){

/** �������������Ӳ��Ժ��� */
#define ADD_METHOD(method_name) \
	pTestCase->AddMethod(&method_name);

/**������������*/
#define END_TESTCASE() \
		} \
	} AUTO_GEN(TestCase); 

/** ���Է���������ʼ�����Է����п�����Ƕ������䡣���ǲ���ֻ��ͨ������� */
#define BEGIN_METHOD(method_name)  \
	bool method_name(TestMethodReport* report) { report->testMethodName = #method_name;

/** ������֤�ꡣexpr��һ�����ʽ������ı��ʽΪ�����������ͨ��, ������Բ�ͨ��*/
#define TEST_TRUE_PASS(expr) \
	++(report->testItemCount);\
	report->testItemReports.push_back(SingleTestReport(#expr, false)); \
	if(expr) { report->testItemReports.back().isPass = true; ++(report->passedItemCount);} \
	else { ++(report->failedItemCount); }

#define TEST_IN_RANGE_PASS(expr, l, u) \
	++(report->testItemCount);\
	report->testItemReports.push_back(SingleTestReport(#expr ##"is between" #l "and" #u, false)); \
	if((expr) > (l) && (expr) < (u)) { report->testItemReports.back().isPass = true; ++(report->passedItemCount);} \
	else { ++(report->failedItemCount); }

#define TEST_IN_DELTA_PASS(expr, v, delta) \
	++(report->testItemCount);\
	report->testItemReports.push_back(SingleTestReport(#expr " is in " #v "|+/-|" #delta, false)); \
	if((expr) > (v)-(delta) && (expr) < (v)+(delta)) { report->testItemReports.back().isPass = true; ++(report->passedItemCount);} \
	else { ++(report->failedItemCount); }

#define TEST_TRUE_PASS_DESC(expr, desc) \
	++(report->testItemCount);\
	report->testItemReports.push_back(SingleTestReport(desc, false)); \
	if(expr) { report->testItemReports.back().isPass = true; ++(report->passedItemCount);} \
	else { ++(report->failedItemCount); }

/** ���Է����������� */
#define END_METHOD() return report->isPass = (report->failedItemCount == 0);}

/** ִ�в��� */
#define EXECUTE_TEST(testName) testName.ExecuteTest();

/** ��ʼ��������׼� */
#define BEGIN_TESTSUITE(suiteName) \
	STestSuite suiteName(#suiteName);\
	class suiteName_##Setting {\
		STestSuite* pTestSuite;\
	public:\
		suiteName_##Setting() : pTestSuite(&suiteName) {

/** ��Ӳ������� */
#define ADD_CASE(case_name) \
	pTestSuite->AddTestCase(&case_name);

/** ���������׼����� */
#define END_TESTSUITE() \
		}\
	} AUTO_GEN(TestSuite);

/** ���ɲ������¼��XML�� */
#define MAKE_AND_SAVE_REPORT(suiteName, fname) \
	suiteName.MakeXMLDocument();\
	suiteName.SaveTestReportXML(fname);

