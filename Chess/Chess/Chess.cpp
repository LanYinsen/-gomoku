#include "Chess.h"
#include<math.h>
#include<mmsystem.h>
#include<graphics.h>
#pragma comment(lib,"winmm.lib")
#include<conio.h>

void putimagePNG(int x, int y, IMAGE* picture) //x为载入图片的X坐标，y为Y坐标
{
	// 变量初始化
	DWORD* dst = GetImageBuffer();    // GetImageBuffer()函数，用于获取绘图设备的显存指针，EASYX自带
	DWORD* draw = GetImageBuffer();
	DWORD* src = GetImageBuffer(picture); //获取picture的显存指针
	int picture_width = picture->getwidth(); //获取picture的宽度，EASYX自带
	int picture_height = picture->getheight(); //获取picture的高度，EASYX自带
	int graphWidth = getwidth();       //获取绘图区的宽度，EASYX自带
	int graphHeight = getheight();     //获取绘图区的高度，EASYX自带
	int dstX = 0;    //在显存里像素的角标

	// 实现透明贴图 公式： Cp=αp*FP+(1-αp)*BP ， 贝叶斯定理来进行点颜色的概率计算
	for (int iy = 0; iy < picture_height; iy++)
	{
		for (int ix = 0; ix < picture_width; ix++)
		{
			int srcX = ix + iy * picture_width; //在显存里像素的角标
			int sa = ((src[srcX] & 0xff000000) >> 24); //0xAArrggbb;AA是透明度
			int sr = ((src[srcX] & 0xff0000) >> 16); //获取RGB里的R
			int sg = ((src[srcX] & 0xff00) >> 8);   //G
			int sb = src[srcX] & 0xff;              //B
			if (ix >= 0 && ix <= graphWidth && iy >= 0 && iy <= graphHeight && dstX <= graphWidth * graphHeight)
			{
				dstX = (ix + x) + (iy + y) * graphWidth; //在显存里像素的角标
				int dr = ((dst[dstX] & 0xff0000) >> 16);
				int dg = ((dst[dstX] & 0xff00) >> 8);
				int db = dst[dstX] & 0xff;
				draw[dstX] = ((sr * sa / 255 + dr * (255 - sa) / 255) << 16)  //公式： Cp=αp*FP+(1-αp)*BP  ； αp=sa/255 , FP=sr , BP=dr
					| ((sg * sa / 255 + dg * (255 - sa) / 255) << 8)         //αp=sa/255 , FP=sg , BP=dg
					| (sb * sa / 255 + db * (255 - sa) / 255);              //αp=sa/255 , FP=sb , BP=db
			}
		}
	}
}


Chess::Chess(int gradeSize, int marginX, int marginY, float chessSize)
{
	this->gradeSize = gradeSize;
	this->margin_x = marginX;
	this->margin_y = marginY;
	this->chessSize = chessSize;
	playerFlag = CHESS_BLACK;

	for (int i = 0; i < gradeSize; i++) {
		vector<int> row;
		for (int j = 0; j < gradeSize; j++) {
			row.push_back(0);
		}
		chessMap.push_back(row);
	}
}

void Chess::init()
{
	//创建游戏窗口
	initgraph(1000, 995, EW_SHOWCONSOLE);
	//initgraph(800, 800);
	//显示棋盘
	loadimage(0, "chess.png");
	//播放开始提示音
	mciSendString("play start.mp3", 0, 0, 0);//???

	//加载黑棋和白棋的棋子的图片
	loadimage(&chessBlackImg, "black.png", chessSize, chessSize, true);
	loadimage(&chessWhiteImg, "white.png", chessSize, chessSize, true);

	//棋盘清零
	for (int i = 0; i < gradeSize; i++) {
		for (int j = 0; j < gradeSize; j++) {
			chessMap[i][j] = 0;
		}

	}
	playerFlag = true;
} // 棋盘的初始化

bool Chess::clickBoard(int x, int y, ChessPos* pos)
{
	int col = (x - margin_x) / chessSize;
	int row = (y - margin_y) / chessSize;
	int leftTopPosX = margin_x + chessSize * col;
	int leftTopPosy = margin_y + chessSize * row;

	int offset = chessSize * 0.4;//阈值
	//int offset = chessSize * 0.5;//阈值
	int len;
	bool ret = false;//是否有效点击

	//判断落子
	do
	{
		//左上角判断 坐标（leftTopPosX，leftTopPosy）
		len = sqrt(pow((x - leftTopPosX), 2) + pow((y - leftTopPosy), 2));
		if (len < offset) {
			pos->row = row;
			pos->col = col;
			if (chessMap[pos->row][pos->col] == 0) {
				ret = true;  //有效点击
			}
			break;
		}

		//右上角 坐标（x2,y2）
		int x2 = leftTopPosX + chessSize;
		int y2 = leftTopPosy;
		len = sqrt(pow(x - x2, 2) + pow(y - y2, 2));
		if (len < offset) {
			pos->row = row;
			pos->col = col + 1;
			if (chessMap[pos->row][pos->col] == 0) {
				ret = true;
			}
			break;
		}

		//左下角
		x2 = leftTopPosX;
		y2 = leftTopPosy + chessSize;
		len = sqrt(pow(x - x2, 2) + pow(y2 - y, 2));
		if (len < offset) {
			pos->row = row + 1;
			pos->col = col;
			if (chessMap[pos->row][pos->col] == 0) {
				ret = true;
			}
			break;
		}

		//右下角
		x2 = leftTopPosX + chessSize;
		y2 = leftTopPosy + chessSize;
		len = sqrt(pow(x2 - x, 2) + pow(y2 - y, 2));
		if (len < offset) {
			pos->row = row + 1;
			pos->col = col + 1;
			if (chessMap[pos->row][pos->col] == 0) {
				ret = true;
			}
			break;
		}
		/*else {
			break;
		}*/

	} while (0);

	return true;
}

void Chess::chessDown(ChessPos* pos, chess_kind_t kind)
{
	mciSendString("play black.mp3", 0, 0, 0);
	int x = margin_x + chessSize * pos->col - 0.5 * chessSize;
	int y = margin_y + chessSize * pos->row - 0.5 * chessSize;

	if (kind == CHESS_WHITE) {
		putimagePNG(x, y, &chessWhiteImg);
	}
	else {
		putimagePNG(x, y, &chessBlackImg);
	}

	updataGameMap(pos);//换棋
}

int Chess::getGradeSize()
{
	return gradeSize;
}

int Chess::getChessData(ChessPos* pos)
{
	return chessMap[pos->row][pos->col];
}

int Chess::getChessData(int row, int col)
{
	return chessMap[row][col];
}

bool Chess::checkOver()
{
	//checkWin()
	if (checkWin()) {
		Sleep(1000);
		if (playerFlag == false) {
			//刚才走棋的是黑方（棋手方）,棋手胜利
			mciSendString("play black.mp3", 0, 0, 0);
			loadimage(0, "win.png");
		}
		else {
			mciSendString("play white.mp3", 0, 0, 0);
			loadimage(0, "lose.png");
		}

		_getch();//暂停
		return true;
	}

	return false;
}

void Chess::updataGameMap(ChessPos* pos)
{
	lastPos = *pos;
	chessMap[pos->row][pos->col] = playerFlag ? CHESS_BLACK : CHESS_WHITE;
	playerFlag = !playerFlag;//黑白棋交换

}


bool Chess::checkWin()
{
	//最近落子点的位置
	int row = lastPos.row;
	int col = lastPos.col;

	//落子点的水平方向
	for (int i = 0; i < 5; i++) {
		//对于第i次
		//五子连珠的最左侧位置
		//i=0 (row,col)(row,col+1)...(row,col+4)
		//i=1 (row,col-1)(row,col+1)...(row,col+3)
		if (col - i >= 0 && col - i + 4 < gradeSize &&
			chessMap[row][col - i] == chessMap[row][col - i + 1] &&
			chessMap[row][col - i] == chessMap[row][col - i + 2] &&
			chessMap[row][col - i] == chessMap[row][col - i + 3] &&
			chessMap[row][col - i] == chessMap[row][col - i + 4]) {
			return true;
		}
	}

	//垂直方向
	for (int i = 0; i < 5; i++) {
		if (row - i >= 0 && row - i + 4 < gradeSize &&
			chessMap[row - i][col] == chessMap[row - i + 1][col] &&
			chessMap[row - i][col] == chessMap[row - i + 2][col] &&
			chessMap[row - i][col] == chessMap[row - i + 3][col] &&
			chessMap[row - i][col] == chessMap[row - i + 4][col]) {
			return true;
		}
	}

	//左下到右上  斜方向  " / "
	for (int i = 0; i < 5; i++) {
		if (row + i - 4 >= 0 && row + i < gradeSize &&
			col - i >= 0 && col - i + 4 < gradeSize &&
			chessMap[row + i][col - i] == chessMap[row + i - 1][col - i + 1] &&
			chessMap[row + i][col - i] == chessMap[row + i - 2][col - i + 2] &&
			chessMap[row + i][col - i] == chessMap[row + i - 3][col - i + 3] &&
			chessMap[row + i][col - i] == chessMap[row + i - 4][col - i + 4]) {
			return true;
		}
	}

	//从左上到右下 " \ "
	for (int i = 0; i < 5; i++) {
		if (row - i >= 0 && row - i + 4 < gradeSize &&
			col - i >= 0 && col - i + 4 < gradeSize &&
			chessMap[row - i][col - i] == chessMap[row - i + 1][col - i + 1] &&
			chessMap[row - i][col - i] == chessMap[row - i + 2][col - i + 2] &&
			chessMap[row - i][col - i] == chessMap[row - i + 3][col - i + 3] &&
			chessMap[row - i][col - i] == chessMap[row - i + 4][col - i + 4]) {
			return true;
		}
	}

	return false;
}