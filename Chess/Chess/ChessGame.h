#pragma once
#include "Man.h"
#pragma once
#include "Man.h"
#include "AI.h"
#include "Chess.h"

class ChessGame
{
private:
	/* data */
	Man* man;
	AI* ai;
	Chess* chess;

public:
	void play(); // 开始对局

	ChessGame(Man* man, AI* ai, Chess* chess);
};

