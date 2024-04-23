#pragma once
#include "Chess.h"

class Man
{
private:
	/* data */
	Chess* chess;
public:
	void init(Chess* chess);
	void go();
};