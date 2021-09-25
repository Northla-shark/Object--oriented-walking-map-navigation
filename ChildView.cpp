
// ChildView.cpp: CChildView 类的实现
//

#include "pch.h"
#include "framework.h"
#include "实验五.h"
#include "ChildView.h"
#include <string>
#include "MainFrm.h"
using namespace std;
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CChildView

CChildView::CChildView()
{	
	
}

CChildView::~CChildView()
{
}



BEGIN_MESSAGE_MAP(CChildView, CWnd)
	ON_WM_TIMER()
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_RBUTTONDOWN()
	ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()



// CChildView 消息处理程序

BOOL CChildView::PreCreateWindow(CREATESTRUCT& cs) 
{
	if (!CWnd::PreCreateWindow(cs))
		return FALSE;

	cs.dwExStyle |= WS_EX_CLIENTEDGE;
	cs.style &= ~WS_BORDER;
	cs.lpszClass = AfxRegisterWndClass(CS_HREDRAW|CS_VREDRAW|CS_DBLCLKS, 
		::LoadCursor(nullptr, IDC_ARROW), reinterpret_cast<HBRUSH>(COLOR_WINDOW+1), nullptr);

	return TRUE;
}

void CChildView::OnInitialUpdate()
{

	// TODO: You may populate your ListView with items by directly accessing
	//  its list control through a call to GetListCtrl().

	m_status_bar = ((CMainFrame*)AfxGetMainWnd())->MainFrameGetStBar();
	
}

void CChildView::OnPaint() 
{
	
	CPaintDC dc(this); // 用于绘制的设备上下文
	C实验五App::currView = this;
	// TODO: 在此处添加消息处理程序代码



	CRect rctClient;
	GetClientRect(rctClient);
	CDC* pDC;
	CDC dcMem;
	pDC = this->GetDC();
	dcMem.CreateCompatibleDC(pDC);
	CBitmap m_bmp;
	BITMAP bitmap;
	CString string("hzd.bmp");
	HINSTANCE hh = AfxGetInstanceHandle();
	HBITMAP hbitmap = (HBITMAP)::LoadImage(AfxGetInstanceHandle(), string, IMAGE_BITMAP,
		1676, 820, LR_CREATEDIBSECTION | LR_LOADFROMFILE);
	if (hbitmap == NULL) {
		return;
	}
	m_bmp.Attach(hbitmap);
	m_bmp.GetBitmap(&bitmap);
	CBitmap* pbmpOld = dcMem.SelectObject(&m_bmp);
	pDC->StretchBlt(0, 0, rctClient.Width(), rctClient.Height(), &dcMem, 0, 0,
		bitmap.bmWidth, bitmap.bmHeight, SRCCOPY);

	
	//绘制地点实心圆
	CBrush fillbrush;
	fillbrush.CreateSolidBrush(RGB(255, 0, 0));
	pDC->SelectObject(&fillbrush);
	POINT point1, point2;//圆外接矩形的左上角点和右下角点
	for (int i = 0;C实验五App::map.points[i].x!=0 || C实验五App::map.points[i].y != 0;i++) {
		point1.x = C实验五App::map.points[i].x - 5;
		point1.y = C实验五App::map.points[i].y - 5;
		point2.x = C实验五App::map.points[i].x + 5;
		point2.y = C实验五App::map.points[i].y + 5;
		pDC->Ellipse(CRect(point1, point2));
	}
	fillbrush.DeleteObject();
	
	if (DWMC_flag==1 && mousemove_flag==1) {
		int num = C实验五App::map.danwei[DDXH].size();
		for (int i = 0,p=1;i < num;i++) {
			int j= C实验五App::map.danwei[DDXH][i];
			pDC->TextOutW(C实验五App::map.points[DDXH].x + 10, C实验五App::map.points[DDXH].y + 16*p++, C实验五App::map.name[j]);
		}
	}

	

	if (C实验五App::queried) {
		if (point_zd != point_qd && point_zd != -1 && point_qd != -1) //求起点和终点之间最短路径
		{
			pDC->MoveTo(C实验五App::map.points[point_qd]);
			char* buf;
			buf = C实验五App::map.ShortestPath_DJ(point_qd, point_zd);
			CString path(buf);
			int xuhao = 0;
			for (int i = 0, nLengthLeft = 0, nLength;(nLength = path.GetLength());i++) {
				CString A = CString(" ");
				nLengthLeft = path.Find(A);
				if (nLengthLeft == -1) {
					xuhao = _ttoi(path);
					pDC->LineTo(C实验五App::map.points[xuhao].x, C实验五App::map.points[xuhao].y);
					pDC->MoveTo({ 0,0 });
					break;
				}
				xuhao = _ttoi(path.Left(nLengthLeft));//获得等号左面的字符串
				pDC->LineTo(C实验五App::map.points[xuhao].x, C实验五App::map.points[xuhao].y);
				pDC->MoveTo(C实验五App::map.points[xuhao]);
				path = path.Right(nLength - nLengthLeft - 1);//获得等号右面的字符串
			}
		}
	}
	// 不要为绘制消息而调用 CWnd::OnPaint()
}

void CChildView::OnTimer(UINT_PTR nIDEvent)
{
	if (nIDEvent == TIMER_ChildView) {
		if (C实验五App::queried == 1)
			C实验五App::queried = 0;
		else C实验五App::queried = 1;
		if (C实验五App::currView != 0)
			C实验五App::currView->Invalidate();
	}
	if (nIDEvent == TIMER_Mousemove) {
		mousemove_flag = 1;
		C实验五App::currView->Invalidate();
	}
}

void CChildView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	OnInitialUpdate();
	int old = point_qd;
	for (int i = 0;C实验五App::map.points[i].x != 0 || C实验五App::map.points[i].y != 0;i++) {
		if (point.x - C实验五App::map.points[i].x <= 5 && point.x - C实验五App::map.points[i].x >= -5
			&& point.y - C实验五App::map.points[i].y <= 5 && point.y - C实验五App::map.points[i].y >= -5) //鼠标坐标在实心圆内
		{
			point_qd = i;
			CString in("起点：");
			for (int m = 0;m < C实验五App::map.danwei[i].size();m++) {
				int j = C实验五App::map.danwei[i][m];
				in = in+C实验五App::map.name[j]+CString(",");
			}
			in.SetAt(in.GetLength() - 1, '\0');
			m_status_bar->SetPaneText(0, in);
			if (point_zd != -1 && point_zd != i) { 
				C实验五App::currView->SetTimer(TIMER_ChildView, 1000, 0);
				C实验五App::queried = 1;
			}
			break;
		}
		else { point_qd = -1;m_status_bar->SetPaneText(0, CString("起点"));KillTimer(TIMER_ChildView); C实验五App::queried = 0; }
	}
	if (old != point_qd) Invalidate();
	else {point_qd = -1;KillTimer(TIMER_ChildView);Invalidate();}
	CWnd::OnLButtonDown(nFlags, point);
}


void CChildView::OnRButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	
	
	int flag = 0;
	OnInitialUpdate();
	int old = point_zd;
	for (int i = 0;C实验五App::map.points[i].x != 0 || C实验五App::map.points[i].y != 0;i++) {
		if (point.x - C实验五App::map.points[i].x <= 5 && point.x - C实验五App::map.points[i].x >= -5
			&& point.y - C实验五App::map.points[i].y <= 5 && point.y - C实验五App::map.points[i].y >= -5) //鼠标坐标在实心圆内
		{
			flag = 1;
			point_zd = i;
			CString in("终点：");
			for (int m = 0;m < C实验五App::map.danwei[i].size();m++) {
				int j = C实验五App::map.danwei[i][m];
				in = in + C实验五App::map.name[j] + CString(",");
			}
			in.SetAt(in.GetLength() - 1, '\0');
			m_status_bar->SetPaneText(1, in);
			if (point_qd != -1 && point_qd != i) {
				C实验五App::currView->SetTimer(TIMER_ChildView, 1000, 0);
				C实验五App::queried = 1; 
			}
			break;
		}
		else { point_zd = -1;m_status_bar->SetPaneText(1, CString("终点"));KillTimer(TIMER_ChildView); C实验五App::queried = 0; }
	}
	if (flag == 1) {
		if (old != point_zd) Invalidate();
		else { point_zd = -1;KillTimer(TIMER_ChildView);Invalidate(); }
	}
	else {
		CMenu menu;
		menu.LoadMenuW(IDR_MENU1);
		CMenu* pContextMenu = menu.GetSubMenu(0);
		CPoint point1;
		GetCursorPos(&point1);//获取当前光标的位置，以便使得菜单可以跟随光标 
		pContextMenu->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point1.x, point1.y, AfxGetMainWnd()); //在指定位置显示弹出菜单
	}
	CWnd::OnRButtonDown(nFlags, point);
}


void CChildView::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	int old = DWMC_flag;
	for (int i = 0;C实验五App::map.points[i].x != 0 || C实验五App::map.points[i].y != 0;i++) {
		if (point.x - C实验五App::map.points[i].x <= 5 && point.x - C实验五App::map.points[i].x >= -5
			&& point.y - C实验五App::map.points[i].y <= 5 && point.y - C实验五App::map.points[i].y >= -5) //鼠标坐标在实心圆内
		{
			DWMC_flag = 1;DDXH = i;break;
		}
		else { DWMC_flag = 0; DDXH = -1; }
	}
	if (old != DWMC_flag){
		if(!DWMC_flag && mousemove_flag) Invalidate();
		KillTimer(TIMER_Mousemove);
		mousemove_flag = 0;
	}
	else {
		C实验五App::currView->SetTimer(TIMER_Mousemove, 2000, 0);
	}
	CWnd::OnMouseMove(nFlags, point);
}

