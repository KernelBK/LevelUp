//���ڻ��ƽ��߶�ͼ���Ƶ�������

#pragma once
#include "resource.h"
#include <atlgdi.h>
#include "MapEditor.h"
#include <Gdiplus.h>

using namespace Gdiplus;

class CMapView : public CDialogImpl<CMapView>
{
protected:
	//λͼ�࣬���ڴ��λͼ
	Bitmap* m_pbmp;
	//λͼ��Ϣ
	BITMAPINFO m_bi;
	//λͼ����ָ��
	BYTE*	m_pBitmapPixels;
	DWORD	m_viewMode;

	RECT	m_curRect;
public:
	enum {IDD = IDD_MAPVIEW};

	CMapView() : m_viewMode(ID_VIEW_FILL), m_pbmp(NULL), m_pBitmapPixels(NULL)
	{
		memset(&m_curRect, 0, sizeof(RECT));
	}
	~CMapView()
	{
		delete m_pbmp;
	}
	BOOL PreTranslateMessage(MSG* pMsg)
	{
		return CWindow::IsDialogMessage(pMsg);
	}

	BEGIN_MSG_MAP(CMapView)
		MESSAGE_HANDLER(WM_PAINT, OnPaint)
		MESSAGE_HANDLER(WM_SIZE, OnResize)
		MESSAGE_HANDLER(WM_VSCROLL, OnVScroll)
		MESSAGE_HANDLER(WM_HSCROLL, OnHScroll)
	END_MSG_MAP()

	LRESULT OnResize(UINT, WPARAM, LPARAM, BOOL&)
	{
		UpdateScrollBar();

		PostMessage(WM_PAINT);
		return 0;
	}

	LRESULT OnPaint(UINT, WPARAM, LPARAM, BOOL&)
	{
		CPaintDC dc(m_hWnd);
		MapInfo mi;
		g_mapEditor.GetMapInfo(&mi);

		//���û�е�ͼ����Ҳ�ͷ�λͼ��������
		if(!g_mapEditor.IsHasData())
		{
			if(m_pbmp)
			{
				delete m_pbmp;
				m_pbmp = NULL;
			}
			return 1;
		}
		
		//�����ͼ�����˸ı䣬Ҳɾ��λͼ���Ա����¿���
		if(g_mapEditor.IsMapSizeChanged(true))
		{
			if(m_pbmp)
			{
				delete m_pbmp;
				m_pbmp = NULL;
			}
		}

		if(g_mapEditor.IsHasData() && m_pbmp == NULL)
		{
			//������ݴ��ڣ�����λͼ�����ڣ��򴴽�һ����λͼ
			g_mapEditor.GetMapInfo(&mi);
			//����һ����λͼ
			CreateBitmap();
		}

		if(!m_pbmp) return E_FAIL;
		
		//δ���ƹ��Ļ�������������λͼ
		if(!g_mapEditor.IsDrawed(true))
		{
			SetBitmap();
		}

		//������������
		//��λͼ��ʾ��������
		if(m_viewMode == ID_VIEW_FILL)
		{
			PaintBitmapFill(mi.width, mi.height);
		}
		else
		{
			PaintBitmapOriginSize(mi.width, mi.height);
		}

		return 1;
	}

	BOOL SetViewMode(DWORD mode);
	DWORD GetViewMode();

	//ƥ��
	void PaintBitmapFill(int mapWidth, int mapHeight);
	void PaintBitmapOriginSize(int mapWidth, int mapHeight);

	void ClearBkg(Graphics* pg, RectF& dirtyRF);

	
	void SetBitmap();		//����λͼ
	void CreateBitmap(void);//����λͼ

	void UpdateScrollBar(LPSCROLLINFO h = NULL, LPSCROLLINFO v = NULL);
	//��Ӧ��������Ϣ
	LRESULT OnVScroll(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnHScroll(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
};