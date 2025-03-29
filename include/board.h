#ifndef BOARD_H
#define BOARD_H
#include <vector>
#include "cell.h"
using namespace std;

class Board{
public:
	int count;
	int bd[10][10] = {{0, 3, 5, 0, 5, 4, 0, 4, 4, 0},
	     		  {1, 0, 0, 0, 0, 0, 0, 0, 0, 6},
	      		  {3, 0, 0, 0, 0, 0, 0, 0, 0, 3},
	      		  {0, 0, 0, 2, 6, 2, 6, 0, 0, 0},
	       		  {6, 0, 0, 6, 0, 0, 2, 0, 0, 1},
	       		  {1, 0, 0, 2, 0, 0, 6, 0, 0, 2},
	       		  {0, 0, 0, 6, 2, 6, 2, 0, 0, 0},
	       		  {3, 0, 0, 0, 0, 0, 0, 0, 0, 5},
	       		  {2, 0, 0, 0, 0, 0, 0, 0, 0, 6},
	       		  {0, 3, 4, 0, 4, 6, 0, 2, 1, 0}};

	vector<Cell> cells;	
	
	
	Board() { count = 0; }
	void init_board();
	void print_board();
	void move(Poss start, Poss prev, int number, int nb_count);
	int get_count() { return count; }

};

#endif
