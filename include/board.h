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
	vector<Area> areas;
	Board(string file_name);
//	void init_board();
	void init_from_file(string file_name);	
	vector<vector <int>> parse_file(string file_name);
	vector<int> split_by_space_int(string line);
	void parse_areas();
	void select_area(Cell cell, Poss start, vector<Cell> &area);
	bool is_board_solved();
	void print_board();
	void make_move(Poss start, Poss prev, int number, int nb_count);
	void check_possible_moves(Poss start, Poss prev, int number, int nb_count, vector<Area> &moves, int &path_counter, bool status = false, int index = 0);
	void print_possible_moves(Poss start, Poss prev, int number, int nb_count);
	void solve();
	bool check_is_any_cells_blocked(Area move);
	void move_on_board(Area move, bool status = true);
	void show_visited_cells();
	bool is_touch_same_number(Area move);

};

#endif
