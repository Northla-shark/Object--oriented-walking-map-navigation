
// 实验五.h: 实验五 应用程序的主头文件
//
#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含 'pch.h' 以生成 PCH"
#endif
#include "graph.h"
#include "resource.h"       // 主符号
#include "ChildView.h"

// C实验五App:
// 有关此类的实现，请参阅 实验五.cpp
//

class C实验五App : public CWinApp
{
public:
	C实验五App() noexcept;
	static CChildView* currView;  //当前视图
	static int queried;
	static graph map;

// 重写
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// 实现

public:
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern C实验五App theApp;
