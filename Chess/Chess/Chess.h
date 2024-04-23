#pragma once
#include <graphics.h>
#include <easyx.h>
#include <vector>
#include<graphics.h>
using namespace std;

// 位置：行 列
struct ChessPos
{
	int row;
	int col;

	ChessPos(int r = 0, int c = 0) :row(r), col(c) {}
};

typedef enum
{
	CHESS_WHITE = -1, // 白棋
	CHESS_BLACK = 1   // 黑棋
} chess_kind_t;       // 棋子种类

class Chess
{
public:
	Chess(int gradeSize, int marginX, int marginY, float chessSize);

	void init(); // 棋盘的初始化

	bool clickBoard(int x, int y, ChessPos* pos); // 是否有效点击

	void chessDown(ChessPos* pos, chess_kind_t kind); // 落子

	int getGradeSize(); // 获取棋盘的大小（13，15，19）

	int getChessData(ChessPos* pos);
	int getChessData(int row, int col); // 获取指定位置是黑棋还是白棋还是空白

	bool checkOver(); // 检查是否结束
private:
	IMAGE chessBlackImg; //黑棋棋子
	IMAGE chessWhiteImg; //白棋棋子

	int gradeSize; //棋盘大小（13，15，17，19）
	int margin_x; //棋盘的左侧边界
	int margin_y; //棋盘的顶部边界
	float chessSize;//棋子的大小

	//存储当前棋局的棋子发布数据 0：空白；   1：黑子；   -1：白子；
	//chessMap[3][5] 表示棋盘的第3行第5列的落子情况
	vector<vector<int>> chessMap;

	//表示现在该谁下棋（落子）
	bool playerFlag;//true: 黑子走

	void updataGameMap(ChessPos* pos);
	bool checkWin();//胜负已分，返回true
	ChessPos lastPos;//最后落子点位置

};
