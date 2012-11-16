#include "SUnitTestConsole.h"

#include <iostream>
using namespace std;

//@{
/**
	STestCase��ʵ��
*/
STestCase::STestCase(const string& caseName) : m_testCaseName(caseName), m_passedTestMethodCount(0), m_failedTestMethodCount(0)
{
}

void STestCase::AddMethod(PTestMethodFunc pf){
	m_testMethodList.push_back(pf);
}

void STestCase::ExecuteTest(){
	m_testMethodReports.resize(m_testMethodList.size());

	for(size_t i = 0; i < m_testMethodList.size(); ++i ){
		bool rv = (*m_testMethodList[i])(&m_testMethodReports[i]);
		if (rv) {
			++m_passedTestMethodCount;
		} else {
			++m_failedTestMethodCount;
		}
	}
}

void STestCase::PrintReportToConsole(){
	cout << "Test Case: " << m_testCaseName << endl;

	for(size_t i = 0; i < m_testMethodList.size(); ++i){

		cout << "\tMethod: " << m_testMethodReports[i].testMethodName << endl;;
		for(int j = 0; j < m_testMethodReports[i].testItemCount; ++j ){
			cout	<< "\t\tTest "
				<< (const char*)(m_testMethodReports[i].testItemReports[j].isPass ? "SUCCEED : " : "FAILED : ")
					<< m_testMethodReports[i].testItemReports[j].content 
					<< endl;
		}

		cout	<< "\tTotal : " << m_testMethodReports[i].testItemCount
				<< " Succeed : " << m_testMethodReports[i].passedItemCount
				<< " Failure : " << m_testMethodReports[i].failedItemCount << endl;
	}

	cout	<< "Total Method: " << m_testMethodList.size()
			<< " Succeed Method : " << m_passedTestMethodCount
			<< " Failure Method : " << m_failedTestMethodCount << endl;

}

void STestCase::SaveTestReport(const string& logf)
{
	
}

#ifdef TINYXML_INCLUDED
void STestCase::SaveTestReport(TiXmlNode* pNode)
{
	//���� TestCase �ڵ�
	TiXmlElement* pTCElem= new TiXmlElement("TESTCASE");
	pTCElem->SetAttribute("name", m_testCaseName);
	pTCElem->SetAttribute("totalMethodCount", int(m_testMethodReports.size()));
	pTCElem->SetAttribute("passedMethodCount", m_passedTestMethodCount);
	pTCElem->SetAttribute("failedMethodCount", m_failedTestMethodCount);

	//д��ÿһ��Method�Ľڵ�
	for( size_t i = 0; i < m_testMethodReports.size(); ++i )
	{
		TiXmlElement* pTMElem = new TiXmlElement("TESTMETHOD");
		pTMElem->SetAttribute("name", m_testMethodReports[i].testMethodName);
		pTMElem->SetAttribute("totalItemCount", m_testMethodReports[i].testItemCount);
		pTMElem->SetAttribute("passedItemCount", m_testMethodReports[i].passedItemCount);
		pTMElem->SetAttribute("failedItemCount", m_testMethodReports[i].failedItemCount);

		//д��ÿһ��Method�ڵ��е�ÿһ��
		for( size_t j = 0; j < size_t(m_testMethodReports[i].testItemCount); ++j )
		{
			TiXmlElement* pTElem = new TiXmlElement("TEST");

				TiXmlElement* pTNElem = new TiXmlElement("CONTENT");
					TiXmlText* pTTElem = new TiXmlText(m_testMethodReports[i].testItemReports[j].content);
					pTNElem->LinkEndChild(pTTElem);
				pTElem->LinkEndChild(pTNElem);

				pTNElem = new TiXmlElement("ISPASS");
					pTTElem = new TiXmlText(m_testMethodReports[i].testItemReports[j].isPass ? "True" : "False");
					pTNElem->LinkEndChild(pTTElem);
				pTElem->LinkEndChild(pTNElem);

			pTMElem->LinkEndChild(pTElem);
		}

		pTCElem->LinkEndChild(pTMElem);
	}

	pNode->LinkEndChild(pTCElem);
}
#endif

//@} STestCase ʵ�ֽ���

//@{
/** SingleTestReport ʵ�� */

SingleTestReport::SingleTestReport(const string& content, bool isPass) : content(content), isPass(isPass)
{}

SingleTestReport::SingleTestReport(const SingleTestReport& rhs) : content(rhs.content), isPass(rhs.isPass)
{}

const SingleTestReport& SingleTestReport::operator = (const SingleTestReport& rhs){
	content = rhs.content;
	isPass = rhs.isPass;
	return *this;
}

//@} SingleTestReport ʵ�ֽ���

//@{
/** TestMethodReport ʵ�ֿ�ʼ */
TestMethodReport::TestMethodReport() : testItemCount(0), passedItemCount(0), failedItemCount(0), isPass(false){}

TestMethodReport::TestMethodReport(const TestMethodReport& rhs)	:	
testMethodName(rhs.testMethodName),
testItemCount(rhs.testItemCount),
passedItemCount(rhs.passedItemCount),
failedItemCount(rhs.failedItemCount),
isPass(rhs.isPass)
{}

const TestMethodReport& TestMethodReport::operator = (const TestMethodReport& rhs){
	testMethodName = rhs.testMethodName;
	testItemCount = rhs.testItemCount;
	passedItemCount = rhs.passedItemCount;
	failedItemCount = rhs.failedItemCount;
	isPass = rhs.isPass;
	return *this;
}

//}@ TestMethodReport ʵ�ֽ���

//@{
/** STestSuite ʵ�ֿ�ʼ */
STestSuite::STestSuite(const string& suiteName) : m_testSuiteName(suiteName), m_pXmlDoc(NULL) {}

void STestSuite::AddTestCase(STestCase* pTestCase){
	m_testCaseList.push_back(pTestCase);
}

void STestSuite::ExecuteTest(){
	for(size_t i = 0; i < m_testCaseList.size(); ++i){
		m_testCaseList[i]->ExecuteTest();
	}
}

#ifdef TINYXML_INCLUDED
/** ��XML��ʽ������Ա��� */
void STestSuite::MakeXMLDocument()
{
	//����Ѵ��ڲ��Ա����ĵ�����ɾ��
	if(m_pXmlDoc){
		delete m_pXmlDoc;
		m_pXmlDoc = NULL;
	}
	m_pXmlDoc = new TiXmlDocument();

	//���� ���Ա���DOM ��ͷ��
	TiXmlDeclaration* pDecl = new TiXmlDeclaration("1.0", "", "");
	m_pXmlDoc->LinkEndChild(pDecl);

	TiXmlElement* pTSElem = new TiXmlElement("TESTSUITE");
	pTSElem->SetAttribute("name", m_testSuiteName);

	for(size_t i = 0; i < m_testCaseList.size(); ++i){
		m_testCaseList[i]->SaveTestReport(pTSElem);
	}

	m_pXmlDoc->LinkEndChild(pTSElem);
}

void STestSuite::SaveTestReportXML(const string& fname){
	if(m_pXmlDoc){
		m_pXmlDoc->SaveFile(fname);
	}
}
#endif
//@} STestSuite ʵ�ֽ���