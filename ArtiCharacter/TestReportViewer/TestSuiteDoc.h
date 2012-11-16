// ***************************************************************
//	@file 
//  TestSuiteDoc date: 01/29/2007
//  -------------------------------------------------------------
//  
//  -------------------------------------------------------------
//  Copyright (C) 2007 LightSky / wuye9036@gmail.com - All Rights Reserved
// ***************************************************************
// 
// ***************************************************************
#pragma once

/**
	��TreeView / MainFrame / ListView���Ӧ���߼����֣��ṩ���������ݷ��ɴ���/�������/��ʾ���̵����ݴ����
*/
#include "TestSuiteWs.h"
#include "ITreeItem.h"

class TestSuiteDoc
{
private:
	string				m_fileName;		///<	�ĵ��������ļ�����
	TestSuiteWs	m_rootWs;			///<	�������ռ�

public:
	TestSuiteDoc(void);
	~TestSuiteDoc(void);

public:
	/** 
	@brief ��һ�������ļ�
	@date 1/29/2007
	@author		LightSky
	@param	pfStr	�ļ�����
	@return	������ļ��ɹ��򷵻�true�����򷵻�false��			
	@bug
	@warning
	*/
	bool OpenReportFile(const TCHAR* pfStr);

	/**
	@brief		��һ���������ռ�
	@date		1/29/2007
	@author		LightSky
	@param		pWs	�����ռ�����ơ����������Ա����Ŀ¼��
	@return		���Ŀ¼�������򷵻�false��
	@bug
	@warning
	*/
	bool OpenReportWorkspace(const TCHAR* pWs);

	/** 
	@brief		��ȡ�б��������
	@date		1/29/2007
	@author		LightSky
	@param
	@return		����һ���������б�ؼ�����Ҫ���С�
	@bug
	@warning
	*/
	StringList GetListColumns(void);

	/** 
	@brief		���õ�ǰ����Ч����㡣List��������Tree�����͵õ����¡�
	@date		1/29/2007
	@author		LightSky
	@param		pTreeDir		����������·���ִ�����Ŀ¼���ơ�
	@return		����Ҳ�����Ӧ������㣬�򷵻�false��
	@bug
	@warning
	*/
	bool SetCurrentTreeNode(const TCHAR* pTreeDir);
};
