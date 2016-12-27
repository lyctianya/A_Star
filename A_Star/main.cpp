/*
写一个a*算法
struct Point
{
int x;
int y;
int value;
};

List<Point> getWay();

*/
#define  _CRT_SECURE_NO_WARNINGS


#include <list>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <math.h>


using namespace std;

//定义一个可走int值

#define CANG0 0


//定义坐标及值
struct Point
{
	int x;
	int y;
	int value;
};
//打印坐标表
void PrintPointTable(Point *a, int max_x, int max_y)
{
	printf("-----------------------------------------\n");
	printf("\t");
	for (int i = 0; i < max_y; i++)
	{
		printf("y=%d\t", i);
	}
	printf("\n");


	for (int x = 0; x<max_x; x++)
	{
		printf("x=%02d\t", x);
		for (int y = 0; y < max_y;y++)
		{
			printf("%d\t",(Point*)(a+x*max_x+y)->value);
		}
		printf("\n");
	}
	printf("-----------------------------------------\n");
}
//从路径中读取地图配表
Point*getMapTableFromCSV(char*path,int*hang,int*lie)
{
	if (path == "")
	{
		return nullptr;
	}
	FILE*fp = fopen(path, "r");
	if (!fp)
	{
		printf("读取\"%s\"文件失败！", path);
		return nullptr;
	}
	//首先计算文件的大小
	fseek(fp, 0L, SEEK_END);
	int filesize = ftell(fp);
	char* buffer = (char*)malloc(filesize*sizeof(char));
	memset(buffer, 0, sizeof(buffer));
	rewind(fp);

	list<list<int>> infoList;
	while (!feof(fp))
	{
		memset(buffer, 0,sizeof(buffer));
		fscanf(fp, "%s", buffer);
		list<int> numlist;
		char*tmp = strtok(buffer, ",");
		while (tmp)
		{
			numlist.push_back(atoi(tmp));
			tmp = strtok(NULL, ",");
			
		}
		infoList.push_back(numlist);
	}
	//最后计算列有多少
	int x = infoList.size();
	int y = 0;
	if (x > 0)
	{
		y = infoList.front().size();
	}
	else
	{
		free(buffer);
		fclose(fp);
		return nullptr;
	}
	*hang = x;
	*lie = y;
	Point*tb = (Point*)malloc(x*y*sizeof(Point));
	memset(tb, 0, sizeof(tb));

	
	//根据infolist 生成表
	for (int i = 0; i < x;i++)
	{
		list<int> x_iter = infoList.front();
		for (int j = 0; j < y;j++)
		{
			struct Point tmp = { i, j, x_iter.front() };
			*(tb+i*y+j) = tmp;
			x_iter.pop_front();
		}
		infoList.pop_front();
	}

	free(buffer);
	fclose(fp);
	return tb;
}

//该点该方向是否可用
/*
    4
3	a   1  1->2->3->4
	2

*/

bool isPointDirectionEnable(Point*mapTb,Point maxnum,Point my)
{
	int cur_x = my.x;
	int cur_y = my.y;
	switch (my.value)
	{
	case 1:
		cur_y += 1;
		break;
	case 2:
		cur_x += 1;
		break;
	case 3:
		cur_y -= 1;
		break;
	case 4:
		cur_x -= 1;
		break;
	default:
		return false;
	}
	//首先判断是否出界
	if (cur_x<0||cur_x>=maxnum.x||cur_y<0||cur_y>=maxnum.y)
	{
		return false;
	}
	//再判断是否可走
	if ((*(mapTb + cur_x*maxnum.y + cur_y)).value == CANG0)
	{
		return true;
	}
	return false;
}

//根据方向获取新坐标
Point getNewPtByDirection(Point p, int dir)
{
	Point newPt = { p.x, p.y, 0 };
	switch (dir)
	{
	case 1:
		newPt.y += 1;
		break;
	case 2:
		newPt.x += 1;
		break;
	case 3:
		newPt.y -= 1;
		break;
	case 4:
		newPt.x -= 1;
		break;
	default:
		printf("出错了，方向有问题！");

	}
	return newPt;
}


//获取当前位置的下一个可走方向
int getCurrentPointNextDirection(Point*mapTb, Point maxnum, Point my,Point unPt)
{
	if (my.value<0||my.value>4)
	{
		return -1;
	}
	for (int i = my.value+1; i <= 4;i++)
	{
		if (isPointDirectionEnable(mapTb, maxnum, { my.x, my.y, i }))
		{
			Point tmp = getNewPtByDirection(my, i);
			if (!(tmp.x==unPt.x&&tmp.y==unPt.y))
			{
				return i;
			}
		}
	}
	return -1;
}


//获取路径
list<Point> getWay(Point*mapTb,Point maxnum,Point start,Point end)
{
	//参数校验不校验了，用的时候注意
	list<Point> resultTb;
	//首先将起点进栈，
	start.value = 0;
	Point currentPt = start;
	bool isBack = false;
	do 
	{
		//首先判断当前是否有路可走
		if (isBack)
		{
			currentPt.value += 1;
			isBack = false;
		}

		Point unPt = { -10, -10, 0 };
		if (resultTb.size()!=0)
		{
			unPt = resultTb.back();
		}
		int newdir = getCurrentPointNextDirection(mapTb, maxnum, currentPt, unPt);
		if (newdir!=-1)//有路可走
		{
			resultTb.push_back(currentPt);//先将当前位置压入队列
			currentPt = getNewPtByDirection(currentPt,newdir);
		}
		else//无路可走，返回吧
		{
			currentPt = resultTb.back();
			resultTb.pop_back();
			isBack = true;
		}
	} while (!((currentPt.x==end.x && currentPt.y==end.y)||(currentPt.x==start.x&&currentPt.y==start.y&&currentPt.value>=4)));

	if (currentPt.x == end.x && currentPt.y == end.y)
	{
		printf("找到路了！\n");
	}
	else if (currentPt.x == start.x&&currentPt.y == start.y&&currentPt.value >= 4)
	{
		printf("无路可走!\n");
	}
	else
	{
		printf("寻路出异常了!\n");
	}
	return resultTb;
}

void main()
{
	int x = 0;
	int y = 0;
	Point* mapTb=getMapTableFromCSV("F:/cppcode/A_Star/A_Star/zuobiao.csv",&x,&y);
	PrintPointTable(mapTb, x, y);
	//开始调取，函数返回路径

	list<Point> pt = getWay(mapTb, { x, y, 0 }, { 0, 0, 0 }, {9,9,0});

	int step = 1001;
	list<Point>::iterator itor=pt.begin();
	while (itor != pt.end())
	{
		Point tmp = *itor;
		(mapTb + tmp.x*y + tmp.y)->value = step;
		*itor++;
		step++;
	}
	
	PrintPointTable(mapTb, x, y);




	free(mapTb);
	getchar();

}