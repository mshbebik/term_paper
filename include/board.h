#ifndef BOARD_H
#define BOARD_H
#include <vector>
#include <string.h>
#include "cell.h"
using namespace std;

class Board{
public:
//	Cell bd[10][10] = {{0, 3, 5, 0, 5, 4, 0, 4, 4, 0},
//	     		  {1, 0, 0, 0, 0, 0, 0, 0, 0, 6},
//	    		  {3, 0, 0, 0, 0, 0, 0, 0, 0, 3},
//	      		  {0, 0, 0, 2, 6, 2, 6, 0, 0, 0},
//	       		  {6, 0, 0, 6, 0, 0, 2, 0, 0, 1},
//	       		  {1, 0, 0, 2, 0, 0, 6, 0, 0, 2},
//	       		  {0, 0, 0, 6, 2, 6, 2, 0, 0, 0},
//	       		  {3, 0, 0, 0, 0, 0, 0, 0, 0, 5},
//	       		  {2, 0, 0, 0, 0, 0, 0, 0, 0, 6},
//	       		  {0, 3, 4, 0, 4, 6, 0, 2, 1, 0}};
	
	vector<vector<Cell>> bd;	
	Board(string file_name);
//	void init_board();
	void init_from_file(string file_name);	
	vector<vector <int>> parse_file(string file_name);
	vector<int> split_by_space_int(string line);
	void print_board();
//	void move(Poss start, Poss prev, int number, int nb_count);

};

#endif
