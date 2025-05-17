#ifndef BOARD_H
#define BOARD_H
#include <vector>
#include <string.h>
#include "cell.h"
using namespace std;

class Board{
	vector<vector<Cell>> bd;	
public:
	Board();
	Board(string file_name);
	void init_from_file(string file_name);	
	bool init_from_user();
	bool is_all_rows_equal();
	int fix_rows_size();
	void solve_by_user();
	int check_is_user_move_valid(Cell cell);
	vector<vector <int>> parse_file(string file_name);
	vector<int> split_by_space_int(string line);
	vector<Area> parse_areas();
	void select_area(Cell cell, Pos start, vector<Cell> &area);
	bool is_board_solved();
	void print_board();
	bool make_move(Cell cell);
	void print_possible_moves(Cell cell);
	void solve(bool show_steps);
	bool check_is_any_cells_blocked(Area move);
	void move_on_board(Area move, bool status = true);
	void show_visited_cells();
	bool is_touch_same_number(Area move);
	int check_possible_moves(Cell cell);
	vector<Area> get_possible_moves(Cell cell);
	vector<Cell> check_single_cell_moves(Cell cell, bool status = true);
	void single_cell_move_on_board(Cell move, bool status = true);
	vector<Cell> select_area_special(Cell cell);
	int is_touch_same_number_single(Cell cell, vector<Cell> current_cell_area);
	vector<Cell> get_neighbours(Cell cell);
	vector<Cell> filter_moves_single(vector<Cell> moves);
	bool is_move_blocks_single(Cell cell);
	vector<Area> get_possible_moves_part(Cell cell);
	void mark_area_if_ready(Cell cell);

	void print_moves(vector<Area> moves);
	bool check_is_moves_identical(Area move1, Area move2);
	void fill_free_space();
	void find_combinations(int n, int max_num, vector<int> &current, vector<vector<int>> &result);
	vector<vector<int>> partition_number(int number);
	void fill_area(vector<Cell> area);




};

#endif
