#pragma once

#include "../../platform/typedef.h"
#define TIXML_USE_STL
#include <tinyxml.h>
#include <util/string.h>
#include <vector>
#define TMPSTR_MAX_LEN 1024
using namespace std;

const byte TEST_PASSED = 0x1;
const byte TEST_FAILED = 0x2; 
const byte TEST_ALL = 0x3;

struct TestItemInfo{
	string		content;
	bool		isPassed;
};

class TestMethodInfo{
private:
	_tstring								m_testMethodName;

	vector<TestItemInfo>		m_testItems;
	uint32							m_testItemCount;
	uint32							m_passedItemCount;
	uint32							m_failedItemCount;

public:
	bool		LoadFromXMLNode(TiXmlElement* pMethodElem);
	uint32	GetItemCount(byte itemState);
	bool		IsPassed();
	const _tstring& GetMethodName();
	vector<TestItemInfo*> GetItemList(byte itemState);
};

class TestCaseInfo{
private:
	_tstring								m_testCaseName;

	vector<TestMethodInfo>		m_testMethods;
	uint32								m_testMethodCount;
	uint32								m_passedMethodCount;
	uint32								m_failedMethodCount;

public:
	bool		LoadFromXMLNode(TiXmlElement* pCaseElem);			///<	��XMLNode�ж�ȡ��Ϣ
	uint32	GetMethodCount(byte methodState);						///<	��ò���ͨ������Ŀ����
	bool		IsPassed();
	const _tstring& GetCaseName();
	vector<TestMethodInfo*> GetMethodList(byte methodState);
};

/**
@class	TestSuiteInfo
@brief	�����׼���Ϣ�� 
@date 1/29/2007
@author LightSky
@bug
@warning
*/
class TestSuiteInfo
{
	vector<TestCaseInfo>		m_testCaseInfos;
	TiXmlDocument				m_xmlDoc;
	
	_tstring							m_testSuiteName;
	uint32							m_testCaseCount;
	uint32							m_passedCaseCount;
	uint32							m_failedCaseCount;

public:
	TestSuiteInfo(void);
	~TestSuiteInfo(void);

	/** 
	@brief		��XML�ж�ȡ�����׼�����Ϣ
	@date		1/29/2007
	@author		LightSky
	@param		pfName	�����׼���XML�����ļ�
	@return		��������ɹ��򷵻�true
	@bug
	@warning
	*/
	bool LoadFromXMLFile(const TCHAR* pfName);
	uint32 GetTestCaseCount(byte caseState);
	vector<TestCaseInfo*> GetTestCaseList(byte caseState);
	const _tstring& GetSuiteName();
	bool IsPassed();
};
