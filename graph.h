#pragma once
#include <vector>
#include "afx.h"
#include <string>
using namespace std;
#define MAX 10000000
#define MAX_VERTEX_NUM 40
class graph
{
public:
	int points_num = 0, edge_num = 0,name_num=0;
	CPoint points[1000] = {0,0};
	int edge[1000][2];
	vector<double> distance;
	vector<int> danwei[1000];
	CString name[1000] = {CString("")};


	double edges[MAX_VERTEX_NUM][MAX_VERTEX_NUM];
	string vexs[MAX_VERTEX_NUM];
	int vexnum, edgenum;
public:
	graph();
	char * ShortestPath_DJ(int v, int zd);//求最短路径
	int dwcx(CString str);//查询单位str所在的地点编号
	~graph();
};

