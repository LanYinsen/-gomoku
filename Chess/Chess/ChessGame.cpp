#include "ChessGame.h"

ChessGame::ChessGame(Man* man, AI* ai, Chess* chess)
{
	this->man = man;
	this->ai = ai;
	this->chess = chess;

	man->init(chess);
	ai->init(chess);
}
//对局开始
void ChessGame::play()
{
	chess->init();
	while (1)
	{
		// 先由棋手走
		man->go();
		if (chess->checkOver())//是否结束
		{
			chess->init(); // 棋局结束，下一局初始化
			continue;
		}
		// AI走
		ai->go();
		if (chess->checkOver())
		{
			chess->init();
			continue;
		}
	}
} // 开始对局
