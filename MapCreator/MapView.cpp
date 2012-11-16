#include "StdAfx.h"
#include ".\mapview.h"

typedef void (*PSetBitmapImpl)(BYTE*, void*, int);

void SetBitmapImpl(BYTE* pBitmap, const float* pHMap, int nWidth, int nHeight, int stride)
{
	for(DWORD i = 0; i < DWORD(nHeight); ++i)
	{
		BYTE* pScanline = &(pBitmap[i * stride]);

		for(DWORD j = 0; j < DWORD(nWidth); ++j)
		{
			pScanline[3 * j + 2] = pScanline[3 * j + 1] = pScanline[3 * j] = BYTE(pHMap[(i * nWidth) + j] * 256);
		}
	}
}

BOOL CMapView::SetViewMode(DWORD mode)
{
	if(mode != m_viewMode)
	{
		PostMessage(WM_PAINT);
	}
	m_viewMode = mode;
	return TRUE;
}

DWORD CMapView::GetViewMode()
{
	return m_viewMode;
}

void CMapView::SetBitmap()
{
	//���û���м�λͼ�����½�������
	ATLASSERT(m_pbmp);

	MapInfo mi;
	g_mapEditor.GetMapInfo(&mi);

	BitmapData bmpData;

	m_pbmp->LockBits(NULL, ImageLockModeWrite, PixelFormat24bppRGB, &bmpData);
	//����߶�ֵ�����õ�ͼ����λͼ���Ҷȱ�ʾ������һ�����Կ���ʹ��MMX�Ż�
	SetBitmapImpl((BYTE*)(bmpData.Scan0), mi.pMapData, mi.width, mi.height, bmpData.Stride);
	m_pbmp->UnlockBits(&bmpData);
}

void CMapView::CreateBitmap(void)
{
	MapInfo mi;
	g_mapEditor.GetMapInfo(&mi);

	delete m_pbmp;
	m_pbmp = new Bitmap(mi.width, mi.height, PixelFormat24bppRGB);

	ATLASSERT(m_pbmp != NULL);
}

LRESULT CMapView::OnVScroll(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	// Get all the vertial scroll bar information
	SCROLLINFO si;

	si.cbSize = sizeof (si);
	si.fMask  = SIF_ALL;
	// Save the position for comparison later on
	GetScrollInfo (SB_VERT, &si);
	int xPos = si.nPos;
	switch (LOWORD (wParam))
	{
	// user clicked left arrow
	case SB_LINELEFT: 
		si.nPos -= 1;
		break;
              
	// user clicked right arrow
	case SB_LINERIGHT: 
		si.nPos += 1;
		break;
              
	// user clicked shaft left of the scroll box
	case SB_PAGELEFT:
		si.nPos -= 1;
		break;
              
	// user clicked shaft right of the scroll box
	case SB_PAGERIGHT:
		si.nPos += 1;
		break;
              
	// user dragged the scroll box
	case SB_THUMBTRACK: 
		si.nPos = si.nTrackPos;
		break;
              
	default :
		break;
	}
	// Set the position and then retrieve it.  Due to adjustments
	//   by Windows it may not be the same as the value set.
	si.fMask = SIF_POS;
	SetScrollInfo (SB_VERT, &si, TRUE);
	GetScrollInfo (SB_VERT, &si);

	if(si.nPos != xPos)
	{
		PostMessage(WM_PAINT);
	}
	return 0;
}

LRESULT CMapView::OnHScroll(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	// Get all the vertial scroll bar information
	SCROLLINFO si;

	si.cbSize = sizeof (si);
	si.fMask  = SIF_ALL;
	// Save the position for comparison later on
	GetScrollInfo (SB_HORZ, &si);
	int xPos = si.nPos;
	switch (LOWORD (wParam))
	{
	// user clicked left arrow
	case SB_LINELEFT: 
		si.nPos -= 1;
		break;
              
	// user clicked right arrow
	case SB_LINERIGHT: 
		si.nPos += 1;
		break;
              
	// user clicked shaft left of the scroll box
	case SB_PAGELEFT:
		si.nPos -= 1;
		break;
              
	// user clicked shaft right of the scroll box
	case SB_PAGERIGHT:
		si.nPos += 1;
		break;
              
	// user dragged the scroll box
	case SB_THUMBTRACK: 
		si.nPos = si.nTrackPos;
		break;
              
	default :
		break;
	}
	// Set the position and then retrieve it.  Due to adjustments
	//   by Windows it may not be the same as the value set.
	si.fMask = SIF_POS;
	SetScrollInfo (SB_HORZ, &si, TRUE);
	GetScrollInfo (SB_HORZ, &si);

	if(si.nPos != xPos)
	{
		PostMessage(WM_PAINT);
	}
	return 0;
}

//ԭ��С���
void CMapView::PaintBitmapOriginSize(int mapWidth, int mapHeight)
{
	SCROLLINFO hsbInfo;
	SCROLLINFO vsbInfo;	

	UpdateScrollBar(&hsbInfo, &vsbInfo);

	RECT r;
	GetClientRect(&r);
	int ch = r.right - r.left;
	int cv = r.bottom - r.top;

	int vRange = vsbInfo.nMax - vsbInfo.nMin;
	int hRange = hsbInfo.nMax - hsbInfo.nMin;
	
	int ph = hsbInfo.nPos;
	int pv = vsbInfo.nPos;

	float destW = float(mapWidth - hRange);
	float destH = float(mapHeight - vRange);
	float destLeft = float((ch - UINT(destW)) >> 1);
	float destTop = float((cv - UINT(destH)) >> 1);

	float srcLeft = float(ph);
	float srcTop = float(vRange - pv);
	float srcW = destW;
	float srcH = destH;

	HDC hDC = GetDC();

	Graphics g(hDC);
	//������һ���Ƚϸ��ӵĵط�
	//�󣬻���С���������ScrollBar��ʾһ���֣�С����������ʾ��
	RectF rf(destLeft, destTop, destW, destH);
	ClearBkg(&g, rf);
	g.DrawImage(m_pbmp, rf, srcLeft, srcTop, srcW, srcH, UnitPixel, NULL, NULL, NULL);
	g.ReleaseHDC(hDC);
	ReleaseDC(hDC);
}

void CMapView::PaintBitmapFill(int mapWidth, int mapHeight)
{
	UpdateScrollBar();

	RECT r;
	GetClientRect(&r);
	int s = min((r.right - r.left), (r.bottom - r.top));

	HDC hDC = GetDC();

	Graphics g(hDC);
	RectF rf(float((r.right - s) >> 1), float((r.bottom - s) >> 1), float(s), float(s));
	g.Clear(Color(255, 200, 220, 200));
	g.DrawImage(m_pbmp, rf, 0, 0, float(mapWidth), float(mapHeight), UnitPixel, NULL, NULL, NULL);
	g.ReleaseHDC(hDC);
	ReleaseDC(hDC);
}

//������������������
void CMapView::ClearBkg(Graphics* pg, RectF& dirtyRF)
{
	RECT rect;
	GetClientRect(&rect);

	//�����ڵ�ʣ�ಿ�ַ�Ϊ4������
	RectF top(0.0f, 0.0f, float(rect.right - rect.left), dirtyRF.GetTop());
	RectF bottom(0.0f, dirtyRF.GetBottom(), float(rect.right - rect.left), rect.bottom - dirtyRF.GetBottom());
	RectF left(0.0f, dirtyRF.GetTop(), dirtyRF.GetLeft(), dirtyRF.Height);
	RectF right(dirtyRF.GetRight(), dirtyRF.GetTop(), rect.right - dirtyRF.GetRight(), dirtyRF.Height);

	SolidBrush b(Color(200, 220, 200));
	pg->FillRectangle(&b, top);
	pg->FillRectangle(&b, bottom);
	pg->FillRectangle(&b, left);
	pg->FillRectangle(&b, right);
}

void CMapView::UpdateScrollBar(LPSCROLLINFO h, LPSCROLLINFO v)
{
	if(m_viewMode == ID_VIEW_FILL)
	{
		GetClientRect(&m_curRect);
		SetScrollRange(SB_HORZ, 0, 0);
		SetScrollRange(SB_VERT, 0, 0);
	}

	else
	{
		MapInfo mi;
		g_mapEditor.GetMapInfo(&mi);

		//���ý���������
		SCROLLINFO hsbInfo;
		SCROLLINFO vsbInfo;

		hsbInfo.cbSize = sizeof(hsbInfo);
		hsbInfo.fMask = SIF_ALL;
		hsbInfo.nPage = 20;
		vsbInfo = hsbInfo;

		RECT r;
		GetClientRect(&r);
		int ch = r.right - r.left;
		int cv = r.bottom - r.top;

		GetScrollInfo(SB_HORZ, &hsbInfo);
		GetScrollInfo(SB_VERT, &vsbInfo);

		//���浱ǰScrollBar�����λ�á�
		float ph = hsbInfo.nPos / (float(hsbInfo.nMax - hsbInfo.nMin) + 0.1f);
		float pv = vsbInfo.nPos / (float(vsbInfo.nMax - vsbInfo.nMin) + 0.1f);

		//����Client����Range�ͻ�׼Pos��
		int vRange = max(int(mi.height) - cv, 0);
		int hRange = max(int(mi.width) - ch, 0);

		hsbInfo.nMax = hRange;
		vsbInfo.nMax = vRange;
		hsbInfo.nMin = 0;
		vsbInfo.nMin = 0;

		hsbInfo.nPos = int((hRange * ph) + 0.5f);
		vsbInfo.nPos = int((vRange * pv) + 0.5f);

		hsbInfo.nPage = UINT(float(mi.width - hRange) / mi.width * hRange);
		vsbInfo.nPage = UINT(float(mi.height - vRange) / mi.height * vRange);

		SetScrollInfo(SB_HORZ, &hsbInfo);
		SetScrollInfo(SB_VERT, &vsbInfo);

		if(h)
		{
			*h = hsbInfo;
		}
		if(v)
		{
			*v = vsbInfo;
		}
	}


}