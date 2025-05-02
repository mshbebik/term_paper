#ifndef BOARD_H
#define BOARD_H
#include <vector>
#include <string.h>
#include "cell.h"
using namespace std;

class Board{
public:
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
	void make_move(Cell cell);
//	void check_possible_moves(Poss start, Poss prev, int number, int nb_count, vector<Area> &moves, int &path_counter, bool status = false, int index = 0);
	void print_possible_moves(Cell cell);
	void solve();
	bool check_is_any_cells_blocked(Area move);
	void move_on_board(Area move, bool status = true);
	void show_visited_cells();
	bool is_touch_same_number(Area move);
	int check_possible_moves(Cell cell);
	vector<Area> get_possible_moves(Cell cell);
	int is_piercies(Area move);
	bool is_stand_near(Cell start, Cell check);
	bool check_is_any_cells_blocked_deep(Area move);
	bool is_groat_move(Area move, int &difference);
	int check_free_space(Area move); 
	vector<Cell> check_single_cell_moves(Cell cell, bool status = true);
	void single_cell_move_on_board(Cell move, bool status = true);
	vector<Cell> select_area_special(Cell cell);
	int is_touch_same_number_single(Cell cell, vector<Cell> current_cell_area);
	int check_straightness(Area move);	
	vector<Cell> get_neighbours(Cell cell);
	vector<Cell> filter_moves_single(vector<Cell> moves);
	bool is_move_blocks_single(Cell cell);
	vector<Area> get_possible_moves_part(Cell cell);
	void mark_area_if_ready(Cell cell);

};

#endif
