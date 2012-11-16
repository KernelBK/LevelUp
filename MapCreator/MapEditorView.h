// MapCreatorView.h : interface of the CMapCreatorView class
//
/////////////////////////////////////////////////////////////////////////////

#pragma once

#include "resource.h"
#include <atlwin.h>
#include <atlctrls.h>
#include <atldlgs.h>
#include <atlddx.h>
#include <atlframe.h>
#include <atlapp.h>
#include "MapEditor.h"

class CMapEditorView :	public CDialogImpl<CMapEditorView>, 
						public CWinDataExchange<CMapEditorView>
{
protected:
	//��ͼ���
	int m_mapWidth;
	int m_mapHeight;

	CEdit m_mapWidthEdit;
	CEdit m_mapHeightEdit;

	CButton m_submitBtn;
	CButton m_initBtn;

	CComboBox m_chooseInitMethodCombo;

public:
	enum { IDD = IDD_MAPEDITORVIEW };

	BOOL PreTranslateMessage(MSG* pMsg)
	{
		m_initBtn.EnableWindow(g_mapEditor.IsHasData());
		::EnableWindow(GetDlgItem(IDC_FIRFILTER), g_mapEditor.IsHasData());
		DoDataExchange(true);
		return CWindow::IsDialogMessage(pMsg);
	}

	BEGIN_DDX_MAP(CMapEditorView)
		DDX_INT(IDC_MAPW, m_mapWidth)
		DDX_INT(IDC_MAPH, m_mapHeight)
	END_DDX_MAP()

	BEGIN_MSG_MAP(CMapEditorView)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		COMMAND_HANDLER(IDC_BTNSUBMIT, BN_CLICKED, OnMapCreationSubmit)
		COMMAND_HANDLER(IDC_INITMAPBTN, BN_CLICKED, OnInitMap)
		COMMAND_HANDLER(IDC_FIRFILTER, BN_CLICKED, OnFIRFilter)
	END_MSG_MAP()

// Handler prototypes (uncomment arguments if needed):
//	LRESULT MessageHandler(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
//	LRESULT CommandHandler(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
//	LRESULT NotifyHandler(int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL& /*bHandled*/)

//��ʼ���Ի���
LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);

//��Ӧ�ύ��������Ϣ����������ͼ��
LRESULT OnMapCreationSubmit(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);

//��Ӧ�ļ�����Ϣ�����ļ�
LRESULT OnFileOpen(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);

//��Ӧ�ļ�������Ϣ�������ļ�
LRESULT OnFileSave(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);

//��Ӧ���Ϊ��Ϣ������ļ�
LRESULT OnFileSaveAs(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);

//��Ӧ�������ļ���Ϣ��׼�������µĵ�ͼ
LRESULT OnFileNew(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);

//��Ӧ��ʼ����ͼ��Ϣ��
LRESULT OnInitMap(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);

//��ʼ��Combo�ؼ����ṩ��ͼ��ʼ��ѡ��
void InitCombo();

//��ȡ��ͼ��Ϣ������ʾ
LRESULT GetMapInfoAndShow();

//�ж��Ƿ���Ը��ĵ�ͼ��Ϣ��������ָModify������ָ�����ͼ����
BOOL IsMapCanChange();
LRESULT OnFIRFilter(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
};
