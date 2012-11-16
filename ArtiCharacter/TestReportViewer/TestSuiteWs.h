#pragma once
#include "../../SUnitTest/SUnitTestConsole.h"
#include "../../platform/typedef.h"
#include "TestSuiteInfo.h"
#include <vector>

using namespace std;
/** TestSuite Workspace 
		TestSuite Workspace �ǰ�����TestSuite���ļ��С�TestSuite Workspace ����ͨ���԰�����ʵ����״�Ĳ㼶��ϵ��*/

const byte SUITE_PASSED = 0x1;
const byte SUITE_FAILED = 0x2; 
const byte SUITE_ALL = 0x3;

class TestSuiteInfo;

class TestSuiteWs
{
private:
	vector<TestSuiteWs> m_subTestSuiteWss;		///< �Ӳ��԰�
	vector<TestSuiteInfo> m_subTestSuites;			///< �Ӳ����׼�

	_tstring	m_suiteWsName;
	bool		m_isAllPassed;										///< ����ù����ռ��µ����в����׼�/�����ռ��ͨ������ò��԰����б�ͨ��
	uint32	m_passedTestSuiteCount;
	uint32	m_failedTestSuiteCount;

	uint32	m_passedTestSuiteWssCount;
	uint32	m_failedTestSuiteWssCount;

public:
	TestSuiteWs();
	~TestSuiteWs(void);

	/** ������׼�ռ������һ���ӿռ� */
	bool Load(const TCHAR* wsName);

	/** ��ѯĳ���͵Ĳ����׼���������
		@param suiteState ָ�������׼�����������ط�������Ĳ����׼��������� 
	*/
	uint32 GetSuiteCount(byte suiteState);
	uint32 GetSuiteWsCount(byte suiteWsState);
	const _tstring& GetSuiteWsName();
	bool IsPassed();
};