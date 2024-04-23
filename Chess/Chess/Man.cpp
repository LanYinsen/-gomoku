#include "Man.h"

void Man::init(Chess* chess)
{
	this->chess = chess;
}

void Man::go()
{
	MOUSEMSG msg;
	ChessPos pos;

	while (1)
	{
		//��ȡ�������Ϣ
		msg = GetMouseMsg();

		//ͨ��chess���������� �ж������Ƿ���Ч���Լ����ӹ���
		if (msg.uMsg == WM_LBUTTONDOWN && chess->clickBoard(msg.x, msg.y, &pos)) {
			break;
		}

	}
	printf("%d,%d\n", pos.row, pos.col);
	//����
	chess->chessDown(&pos, CHESS_BLACK);
	//msg.x, msg.y;

}