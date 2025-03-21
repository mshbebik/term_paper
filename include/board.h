#ifndef BOARD_H
#define BOARD_H

#include "cell.h"

class Board{
	int count;
public:
	int bd[10][10];
	Board() { init_board(); }
	void init_board();
	void print_board();
	void move(Poss start, Poss prev, int number, int nb_count);
	int get_count() { return count; }

};

#endif
