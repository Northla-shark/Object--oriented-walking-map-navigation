// LJCX.cpp: 实现文件
//

#include "pch.h"
#include "实验五.h"
#include "LJCX.h"
#include "afxdialogex.h"
#include<windows.h>
#include<algorithm>
// LJCX 对话框

IMPLEMENT_DYNAMIC(LJCX, CDialogEx)

int LCS(const  string strL1, const  string strL2, int& begin);
int LCS(const  string strL1, const  string strL2, int& begin)
{
	//获取两个源串的长度
	int  L1 = strL1.size(), L2 = strL2.size();
	//构建二维矩阵
	vector<vector< int >> x(L2, vector<int>(L1, 0));
	//特殊情况返回
	if (strL1 == strL2)
	{
		return  strL1.length();
	}
	if (strL1.size() == 0 || strL2.size() == 0)
	{
		return  0;
	}

	int  length = 0, index = 0;
	for (int i = 0;i < L2;i++)
	{
		for (int j = 0;j < L1;j++)
		{
			int  n = (i >= 1 && j >= 1) ? x[i - 1][j - 1] : 0;

			x[i][j] = (strL2[i] == strL1[j]) ? 1 + n : 0;
			//获得最长子串末尾数值和位置
			if (x[i][j] > length)
			{
				length = x[i][j];
				index = i;
			}
		}
	}
	begin = index - length + 1;
	return  string(strL2, index - length + 1, length).length();
}

LJCX::LJCX(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_LJCX, pParent)
{

}

LJCX::~LJCX()
{
}

void LJCX::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(LJCX, CDialogEx)
	ON_BN_CLICKED(IDOK, &LJCX::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &LJCX::OnBnClickedCancel)
	ON_WM_CLOSE()
	ON_EN_CHANGE(IDC_EDIT_QDXX, &LJCX::OnEnChangeEditQdxx)
	ON_EN_CHANGE(IDC_EDIT_ZDXX, &LJCX::OnEnChangeEditZdxx)
	ON_EN_SETFOCUS(IDC_EDIT_QDXX, &LJCX::OnEnSetfocusEditQdxx)
	ON_EN_SETFOCUS(IDC_EDIT_ZDXX, &LJCX::OnEnSetfocusEditZdxx)
	ON_LBN_DBLCLK(IDC_LIST_DDXX, &LJCX::OnLbnDblclkListDdxx)
	ON_EN_KILLFOCUS(IDC_EDIT_QDXX, &LJCX::OnEnKillfocusEditQdxx)
	ON_BN_SETFOCUS(IDOK, &LJCX::OnBnSetfocusOk)
	ON_BN_SETFOCUS(IDCANCEL, &LJCX::OnBnSetfocusCancel)
END_MESSAGE_MAP()


// LJCX 消息处理程序


void LJCX::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialogEx::OnOK();

	CString strqd,strzd;
	GetDlgItem(IDC_EDIT_QDXX)->GetWindowText(strqd);
	GetDlgItem(IDC_EDIT_ZDXX)->GetWindowText(strzd);
	if (strqd != CString("") && strzd != CString("")) 
	{
		C实验五App::currView->point_qd = C实验五App::map.dwcx(strqd);
		C实验五App::currView->point_zd = C实验五App::map.dwcx(strzd);
	}
	else
	{
		MessageBox(TEXT("请输入正确的地点!"), TEXT("错误"), MB_OK);
	}

	if (C实验五App::currView->point_zd != C实验五App::currView->point_qd 
		&& C实验五App::currView->point_zd != -1 && C实验五App::currView->point_qd != -1) {
		C实验五App::currView->SetTimer(TIMER_ChildView, 1000, 0);
		C实验五App::queried = 1;
		C实验五App::currView->Invalidate();
	}
	else 
	{
		C实验五App::currView->point_qd = -1;
		C实验五App::currView->point_zd = -1;
	}


	/*CDC* pDC;
	CDC dcMem;
	pDC = C实验五App::currView->GetDC();
	C实验五App::currView->SetTimer(TIMER_ChildView, 1000, 0);
	C实验五App::queried = 1;
	pDC->LineTo(123, 456);
	CString abc = CString("XXXXXXXXX");
	pDC->TextOutW(150, 150, abc);*/
}


void LJCX::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialogEx::OnCancel();
}


void LJCX::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CDialogEx::OnClose();
}


void LJCX::OnEnChangeEditQdxx()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。
	CListBox* listbox = (CListBox*)GetDlgItem(IDC_LIST_DDXX);
	int nCurSel = listbox->GetCount();
	if (nCurSel<=0) nCurSel = 10;
	while (listbox->GetCount())		listbox->DeleteString(0);//先删除所有文本 

	CString str_c;
	GetDlgItem(IDC_EDIT_QDXX)->GetWindowText(str_c);
	string str = (CW2A(str_c.GetString()));
	vector<double>	a;
	int num_pipei = 0;
	for (int i = 0;i < C实验五App::map.name_num;i++) {
		string str1(CW2A(C实验五App::map.name[i].GetString()));
		int begin=0;
		int x = LCS(str,str1,begin);//最大公共子串长度
		double J = 1.0 * x / (str.length() + str1.length() - x);//相似系数
		if (J == 1) { listbox->AddString(C实验五App::map.name[i]);return; }
		if (x >= 2 && begin%2==0) a.push_back(J),num_pipei++;
		else a.push_back(0);
	}
	if(num_pipei==1) {
		for(int i=0;i<a.size();i++)
			if(a[i])	listbox->AddString(C实验五App::map.name[i]);
		return; 
	}
	for (int i = 0;i < num_pipei && i < nCurSel;i++)
	{
		int m=-1;
		double max = 0,min=1;
		for (int j = 0;j < a.size();j++) 
 			if (a[j] > max) max = a[j], m = j;
		if (m != -1) { 
			listbox->AddString(C实验五App::map.name[m]); 
		}
		a[m] = 0;
	}
	// TODO:  在此添加控件通知处理程序代码
}


void LJCX::OnEnChangeEditZdxx()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。
	CListBox* listbox = (CListBox*)GetDlgItem(IDC_LIST_DDXX);
	int nCurSel = listbox->GetCount();
	if (nCurSel<=0) nCurSel = 10;
	while (listbox->GetCount())		listbox->DeleteString(0);//先删除所有文本 
	CString str_c;
	GetDlgItem(IDC_EDIT_ZDXX)->GetWindowText(str_c);
	string str = (CW2A(str_c.GetString()));
	vector<double>	a;
	int num_pipei = 0;
	for (int i = 0;i < C实验五App::map.name_num;i++) {
		string str1(CW2A(C实验五App::map.name[i].GetString()));
		int begin = 0;
		int x = LCS(str, str1, begin);//最大公共子串长度
		double J = 1.0 * x / (str.length() + str1.length() - x);
		if (J == 1) { listbox->AddString(C实验五App::map.name[i]);return; }
		if (x >= 2 && begin % 2 == 0) a.push_back(J), num_pipei++;
		else a.push_back(0);
	}
	if (num_pipei == 1) { listbox->AddString(C实验五App::map.name[0]);return; }
	for (int i = 0;i < num_pipei && i < nCurSel;i++)
	{
		int m = -1;
		double max = 0, min = 1;
		for (int j = 0;j < a.size();j++)
			if (a[j] > max) max = a[j], m = j;
		if (m != -1) listbox->AddString(C实验五App::map.name[m]);
		a[m] = 0;
	}
	// TODO:  在此添加控件通知处理程序代码
}



void LJCX::OnEnSetfocusEditQdxx()
{
	// TODO: 在此添加控件通知处理程序代码
	GetDlgItem(IDC_LIST_DDXX)->ShowWindow(SW_SHOW);
	CListBox* listbox = (CListBox*)GetDlgItem(IDC_LIST_DDXX);
	CString str_c;
	GetDlgItem(IDC_EDIT_QDXX)->GetWindowText(str_c);
	if (str_c == CString(""))
		while (listbox->GetCount())		listbox->DeleteString(0);//删除所有文本 
	QD_ZD = -1;
}


void LJCX::OnEnSetfocusEditZdxx()
{
	// TODO: 在此添加控件通知处理程序代码
	GetDlgItem(IDC_LIST_DDXX)->ShowWindow(SW_SHOW);
	CListBox* listbox = (CListBox*)GetDlgItem(IDC_LIST_DDXX);
	CString str_c;
	GetDlgItem(IDC_EDIT_ZDXX)->GetWindowText(str_c);
	if(str_c==CString(""))
		while (listbox->GetCount())		listbox->DeleteString(0);//删除所有文本 
	QD_ZD = 1;
}


void LJCX::OnLbnDblclkListDdxx()
{
	// TODO: 在此添加控件通知处理程序代码
	CListBox* listbox = (CListBox*)GetDlgItem(IDC_LIST_DDXX);
	int nCurSel=listbox->GetCurSel();
	listbox->GetText(nCurSel, strText);
	if (QD_ZD == 1) 
	{
		SetDlgItemText(IDC_EDIT_ZDXX, strText);
		/*QD_ZD = 0;*/
		strText = CString("");
	}
	else if (QD_ZD == -1) 
	{
		SetDlgItemText(IDC_EDIT_QDXX, strText);
		//QD_ZD = 0;
		strText = CString("");
	}
}


void LJCX::OnEnKillfocusEditQdxx()
{
	
	// TODO: 在此添加控件通知处理程序代码
}


void LJCX::OnBnSetfocusOk()
{
	GetDlgItem(IDC_LIST_DDXX)->ShowWindow(SW_HIDE);
	// TODO: 在此添加控件通知处理程序代码
}


void LJCX::OnBnSetfocusCancel()
{
	GetDlgItem(IDC_LIST_DDXX)->ShowWindow(SW_HIDE);
	// TODO: 在此添加控件通知处理程序代码
}
