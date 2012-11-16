#include "StdAfx.h"
#include "MapEditorView.h"
#include <atlframe.h>
#include ".\mapeditorview.h"
#include "FilterDlg.h"
#include "PlasmaDlg.h"
#include "FaultDlg.h"

LRESULT CMapEditorView::OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	m_mapWidthEdit.Attach( GetDlgItem(IDC_MAPW) );
	m_mapHeightEdit.Attach( GetDlgItem(IDC_MAPH) );

	m_submitBtn.Attach( GetDlgItem(IDC_BTNSUBMIT) );
	m_initBtn.Attach( GetDlgItem(IDC_INITMAPBTN) );
	m_chooseInitMethodCombo.Attach( GetDlgItem(IDC_INITCHOOSE) );
	InitCombo();

	m_mapWidth = 0;
	m_mapHeight = 0;

	DoDataExchange(false);
	m_initBtn.EnableWindow(g_mapEditor.IsHasData());
	::EnableWindow(GetDlgItem(IDC_FIRFILTER), g_mapEditor.IsHasData());

	m_mapWidthEdit.SetReadOnly(true);
	m_mapHeightEdit.SetReadOnly(true);

	m_submitBtn.ShowWindow(false);
	return S_OK;
}

LRESULT CMapEditorView::OnMapCreationSubmit(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	if(g_mapEditor.CreateMapData(m_mapWidth, m_mapHeight))
	{
		m_mapWidthEdit.SetReadOnly(true);
		m_mapHeightEdit.SetReadOnly(true);

		m_submitBtn.ShowWindow(0);
	}
	else
	{
		MessageBox(_T("����ʧ�ܣ�"), _T("����"));
	}

	return S_OK;
}

LRESULT CMapEditorView::OnFileOpen(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	//�ж��û��Ƿ�������ļ�����ʱ���û������˻�ȥ��
	if(!IsMapCanChange())
	{
		return S_OK;
	}

	CFileDialog dlg(TRUE, _T("hmp"), NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, _T("HeightFeild Map Files (*.hmp)\0*.hmp\0All Files (*.*)\0*.*\0"), m_hWnd);

	if(dlg.DoModal() == IDCANCEL)
	{
		return S_OK;
	}

	g_mapEditor.SetFileName(dlg.m_szFileName);
	if(g_mapEditor.LoadFromFile())
	{
		GetMapInfoAndShow();
		return S_OK;
	}

	MessageBox(_T("�ļ���ȡ����"), _T("�򿪴���"));
	return S_OK;
}

LRESULT CMapEditorView::OnFileSave(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	// TODO: �ڴ���������������
	if(!g_mapEditor.IsAttachedFile())
	{
		BOOL b;
		OnFileSaveAs(0, 0, NULL, b);
	}
	else
	{
		if(g_mapEditor.SaveToFile())
		{
			return S_OK;
		}

		MessageBox(_T("�ļ��洢����"), _T("�洢����"));
	}
	return S_OK;
}

LRESULT CMapEditorView::OnFileSaveAs(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	// TODO: �ڴ���������������
	CFileDialog dlg(FALSE, _T("hmp|txt"), NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, _T("HeightFeild Map Files (*.hmp)\0*.hmp\0Text Files (*.txt)\0*.txt\0"), m_hWnd);

	// TODO: �ڴ���������������
	if(dlg.DoModal() == IDCANCEL)
	{
		return S_OK;
	}

	g_mapEditor.SetFileName(dlg.m_szFileName);
	if(g_mapEditor.SaveToFile())
	{
		return S_OK;
	}

	MessageBox(_T("�ļ��洢����"), _T("�洢����"));
	return S_OK;
	return 0;
}

LRESULT CMapEditorView::OnFileNew(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	//������½����˵�ʱ�������ж��Ƿ����ļ���Ҫ���档
	if(!IsMapCanChange())
	{
		return S_OK;
	}

	//�򿪡��ύ�½�����ť��������ͼ�����ó�ֵ����������ؼ���
	m_mapWidth = 0;
	m_mapHeight = 0;
	DoDataExchange(false);

	m_mapWidthEdit.SetReadOnly(false);
	m_mapHeightEdit.SetReadOnly(false);

	m_submitBtn.ShowWindow(1);

	return 0;
}

LRESULT CMapEditorView::GetMapInfoAndShow()
{
	MapInfo mi;
	g_mapEditor.GetMapInfo(&mi);
	ATLASSERT(mi.pMapData);

	m_mapWidth = mi.width;
	m_mapHeight = mi.height;

	DoDataExchange(false);
	return S_OK;
}

BOOL CMapEditorView::IsMapCanChange()
{
	//�ж��ļ��Ƿ��޸Ĺ��������޸Ĺ�������ʾ���档Open����ݷ��ص�������ж��Ƿ���ļ�ѡ��Ի���
	if(g_mapEditor.IsHasData() && g_mapEditor.IsModified())
	{
		int rv = ::MessageBoxEx(m_hWnd, _T("�ļ����޸ģ��Ƿ񱣴棿"), _T("�����ļ�"), MB_YESNOCANCEL, 0);
		if(rv == IDYES)
		{
			BOOL b;
			OnFileSave(0, 0, NULL, b);
		}
		else
		{
			if( rv == IDCANCEL) return FALSE;
		}
	}
	return TRUE;
}

LRESULT CMapEditorView::OnInitMap(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	//��ʼ����ͼ������û��ô�򵥿���
	if(g_mapEditor.IsHasData())
	{
		CString str;
		int id = m_chooseInitMethodCombo.GetCurSel();
		if(id == CB_ERR)
		{
			return 0;
		}
		m_chooseInitMethodCombo.GetLBText(id, str);
		if(_tcscmp(str, _T("������")) == 0)
		{
			g_mapEditor.InitMapByRandomHeight();
		}
		else if(_tcscmp(str, _T("�е��û�")) == 0)
		{
			CPlasmaDlg pdlg;
			pdlg.DoPlasma();
		}
		else if(_tcscmp(str, _T("����ϲ���")) == 0)
		{
			CFaultDlg cdlg;
			cdlg.DoFault();
		}
		else
		{
			ATLASSERT(true);
		}
	}
	return 0;
}

void CMapEditorView::InitCombo()
{
	m_chooseInitMethodCombo.AddString(_T("������"));
	m_chooseInitMethodCombo.AddString(_T("����ϲ���"));
	m_chooseInitMethodCombo.AddString(_T("�е��û�"));
	//m_chooseInitMethodCombo.AddString(_T("��ֵ���"));
}

//����˹��˰�ť�Ժ�ʹ�õ�ͨ�˲�������
LRESULT CMapEditorView::OnFIRFilter(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	CFilterDlg fdlg;
	fdlg.DoFilter();
	return 0;
}
