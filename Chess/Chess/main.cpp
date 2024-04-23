#include <iostream>
#include "ChessGame.h"
int main(void)
{

	Man man;
	//Chess chess;
	Chess chess(19, 25, 30, 52.3);
	//Chess chess(19, 34, 28, 25.21);
	AI ai;
	ChessGame game(&man, &ai, &chess);

	game.play();

	return 0;
}

//#include<graphics.h>//引用图形库头文件
//#include <conio.h>
//int main()
//{
//	initgraph(400, 400);    //创建窗口大小为640x480像素
//	circle(200, 200, 100);    //画圆，圆心(200,200),半径100
//	outtextxy(170, 200, _T("比特冬哥"));
//	_getch();      //按任意键继续,防止闪退
//	closegraph();   //关闭绘图窗口
//	return 0;
//}
