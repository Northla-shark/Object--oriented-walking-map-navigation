#include "pch.h"
#include "graph.h"
#include <locale.h>
#include <afx.h>
graph::graph() {
	FILE* fp;
	int x, y;
	int i = 0;

	fp = fopen("Location.txt", "r");
	while (!feof(fp)) {
		fscanf(fp, "%d %d", &x, &y);
		points[i].x = x;
		points[i].y = y;
		points_num++;
		i++;
	}
	fclose(fp);

	fp = fopen("Edge.txt", "r");
	i = 0;
	while (!feof(fp)) {
		double temp = 0;
		fscanf(fp, "%d,%d", &x, &y);
		edge[i][0] = x;
		edge[i][1] = y;
		edge_num++;
		temp = sqrt((points[x].x - points[y].x) * (points[x].x - points[y].x) + (points[x].y - points[y].y)* (points[x].y - points[y].y));
		distance.push_back(temp);
		i++;
	}
	fclose(fp);

	fp = fopen("Locorg.txt", "r");
	i = 0;
	while (!feof(fp)) {
		fscanf(fp, "%d ", &x);
		if (x != -1)
			danwei[i].push_back(x);
		else i++;
	}
	fclose(fp);

	CFile file;
	CString strFileName = _T("Organization.txt");
	file.Open(strFileName, CFile::modeRead);
	INT sz = file.GetLength();
	WCHAR* wBuf = new WCHAR[sz + 1];
	char* buffer_src = new char[sz + 1];
	file.Read(buffer_src, sz * sizeof(char));
	buffer_src[sz] = '\0';
	wBuf[sz] = '\0';
	file.Close();
	MultiByteToWideChar(CP_UTF8, 0, buffer_src, -1, wBuf, sz + 1);
	
	CString strJson(wBuf); // wBuf这里已经是中文的了
	OutputDebugString(strJson);
	
	for (int i = 0, nLengthLeft = 0, nLength;(nLength = strJson.GetLength());i++) {
		CString A = CString("\r\n");
		nLengthLeft = strJson.Find(A);
		if (nLengthLeft == -1) { name[i] = strJson;break; }
		name[i] = strJson.Left(nLengthLeft);//获得等号左面的字符串
		name_num++;
		CString B = CString("\n");
		nLengthLeft = strJson.Find(B);
		strJson = strJson.Right(nLength - nLengthLeft - 1);//获得等号右面的字符串
	}
	name[6] = CString("计算机学院");

	vexnum = points_num;
	edgenum =edge_num;
	for (int i = 0;i < points_num;i++) {
		string abc=to_string(i);
		vexs[i] = abc;
	}
	for (int i = 0;i < points_num;i++)
		for (int j = 0;j < points_num;j++)
			edges[i][j] = MAX;//将矩阵初始化为MAX
	for (int k = 0;k < edge_num;k++)
	{
		int i = edge[k][0], j = edge[k][1];
		double weight = distance[k];
		edges[i][j] = weight;
		edges[j][i] = weight;
	}
}

char * graph::ShortestPath_DJ(int v,int zd)
{
	string abc = " ";
	int i, j, k;
	int final[MAX_VERTEX_NUM];//该数组用来标识顶点是否已确定了最短路径
	double dist[MAX_VERTEX_NUM],min;
	string path[2 * MAX_VERTEX_NUM];
	for (i = 0;i < vexnum;i++)
	{//初始化工作
		dist[i] = edges[v][i];//dist数组用来存储当前找到的v到其他各顶点的最短路径
		if (dist[i] < MAX)
			path[i] = vexs[v] + abc + vexs[i];//如果v到i有边的话，把顶点字符存到path字符数组中，表示路径
		else
			path[i] = "";
		final[i] = 0;//初始化标识数组为0
	}
	dist[v] = 0;
	final[v] = 1;
	for (j = 1;j < vexnum;j++)
	{
		min = MAX;
		for (i = 0;i < vexnum;i++)
			if (dist[i] < min && final[i] == 0)
			{
				min = dist[i];
				k = i;
			}//找到dist数组中最小值的位置k
		//cout << path[k] << " " << dist[k] << endl;//输出最短路径
		final[k] = 1;//设置标志位
		for (i = 0;i < vexnum;i++)
		{//遍历每个顶点i和当前的已求出的最短路径的顶点k作比较，若从源点经过顶点k到顶点i的路径，比dist[i]小，
			//则更新顶点dist[i]
			if (dist[i] > dist[k] + edges[k][i] && final[i] == 0)
			{
				dist[i] = dist[k] + edges[k][i];
				path[i] = path[k] + abc+vexs[i];
			}
		}//从整体上来看就是算出k的邻接点的当前最短路径
	}
	char* buf = new char[path[zd].size()+1];
	int length = path[zd].copy(buf, path[zd].size());
	buf[length] = '\0';
	return buf;
}

int graph::dwcx(CString str) 
{
	int i = 0,j=0;
	while (str != name[i] && i<name_num) { 
		i++; 
	}
	if (i == name_num) return -1;
	for (j = 0;j < points_num;j++) {
		vector<int>::iterator result = find(danwei[j].begin(), danwei[j].end(), i); //查找i
		if (result != danwei[j].end()) return j;
	}
}

graph::~graph() {

}