#pragma once


// LJCX 对话框

class LJCX : public CDialogEx
{
	DECLARE_DYNAMIC(LJCX)

public:
	LJCX(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~LJCX();
	CString strText;
	int QD_ZD = 0;

// 对话框数据
//#ifdef AFX_DESIGN_TIME
//	enum { IDD = IDD_DIALOG_DRDT };
//#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnClose();
	afx_msg void OnEnChangeEditQdxx();
	afx_msg void OnEnChangeEditZdxx();
	afx_msg void OnEnSetfocusEditQdxx();
	afx_msg void OnEnSetfocusEditZdxx();
	afx_msg void OnLbnDblclkListDdxx();
	afx_msg void OnEnKillfocusEditQdxx();
	afx_msg void OnBnSetfocusOk();
	afx_msg void OnBnSetfocusCancel();
};
