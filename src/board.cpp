#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <fstream>
#include "board.h"
using namespace std;

Board::Board(string file_name) {
	init_from_file(file_name);
}

void Board::init_from_file(string file_name) {
	vector<vector<int>> nums = parse_file(file_name);
	for(size_t i = 0; i < nums.size(); i++) {
		bd.push_back(vector<Cell>());
		for(size_t k = 0; k < nums[0].size(); k++) {
			bd[i].push_back(Cell(Poss(i, k), nums[i][k]));	
		}
	} 
}

vector<vector <int>> Board::parse_file(string file_name) {
	ifstream file(file_name);
	
	if(!file.is_open()) {
		cerr << "Error: Can't open \"" << file_name << "\" file for parsing." << endl;
		exit(1);
	}
	vector<vector <int>> nums;
	string line;
	while(getline(file, line)){
		nums.push_back(split_by_space_int(line));
	}	
	file.close();
	size_t width = nums[0].size();
	for(size_t i = 0; i < nums.size(); i++) {
		if(nums[i].size() != width) {
			cerr << "Error: Inccorrect input in \"" << file_name << "\"" << endl; 
			exit(1);
		}
	}	
	return nums;	
}

vector<int> Board::split_by_space_int(string line) {
	vector<int> splited;
	for(size_t i = 0; i < line.size(); i++) {
		if(line[i] != ' ') {
			splited.push_back(line[i] - '0');		
		}		
	}
	return splited;
}





void Board::print_board() {
	cout << "    ";
	for(size_t i = 0; i < bd[0].size(); i++) {
		cout << i+1 << " ";
	}
	cout << endl;
	cout << "   ";
	for(size_t i = 0; i < 2*bd[0].size(); i++) {
		cout << "─";
	}
	cout << endl;
	for(size_t i = 0; i < bd.size(); i++) {
		if(i+1 == 10) cout << i+1 << "│ ";
		else { cout << i+1 << " │ "; }

		for(size_t a = 0; a < bd[0].size(); a++) {
			cout << bd[i][a].number << " ";
		}
		cout << endl;
	}
	cout << endl;

}

void Board::parse_areas() {
	vector<Cell> area;
	for(size_t i = 0; i < bd.size(); i++) {
		for(size_t k = 0; k < bd[0].size(); k++) {
			if(bd[i][k].is_visited == false && bd[i][k].number != 0) {
				select_area(bd[i][k], bd[i][k].coord, area);
				if((size_t)bd[i][k].number != area.size()) {
					for(Cell temp : area) {
						bd[temp.coord.x][temp.coord.y].is_visited = false;
					}
					area.clear();
				}
				else {
					areas.push_back(Area(bd[i][k].number, area));
					area.clear();
				}
			}
		}
	}
}

void Board::select_area(Cell cell, Poss start, vector<Cell> &area) {
	Poss ps[5] = { start, Poss(start.x-1, start.y), Poss(start.x+1, start.y), Poss(start.x, start.y-1), Poss(start.x, start.y+1) };
	for(int i = 0; i < 5; i++) {
		if(ps[i].x < 0 || (size_t)ps[i].x >= bd.size() || ps[i].y < 0 || (size_t)ps[i].y >= bd[0].size()) continue;
		if(bd[ps[i].x][ps[i].y].number == cell.number && bd[ps[i].x][ps[i].y].is_visited == false) {
			bd[ps[i].x][ps[i].y].is_visited = true;
			area.push_back(bd[ps[i].x][ps[i].y]);
			select_area(cell, Poss(ps[i].x, ps[i].y), area);
		}
	} 
}


bool Board::is_board_solved() {
	int sum;
	for(Area i : areas) {
		sum += i.number;
	}
	return ((size_t)sum == (bd.size()*bd[0].size()));

}

void Board::check_possible_moves(Poss start, Poss prev, int number, int nb_count, vector<Area> &moves, int &path_counter, bool status, int index) {
	if (start.x < 0 || (size_t)start.x >= bd.size() || start.y < 0 || (size_t)start.y >= bd[0].size()) {
		return;
	}
	Poss ps[4] = { Poss(start.x-1, start.y), Poss(start.x+1, start.y), Poss(start.x, start.y-1), Poss(start.x, start.y+1) };
        for (int i = 0; i < 4; i++) {
		if (ps[i].x < 0 || (size_t)ps[i].x >= bd.size() || ps[i].y < 0 || (size_t)ps[i].y >= bd[0].size()) continue; 
		if (nb_count == number) {
			if (status == true) {
				moves.push_back(Area());
				if ((size_t)path_counter < moves.size() - 1) {
					moves[path_counter + 1] = moves[path_counter];
				}
			}
			path_counter++;
			return;

		}
		if (bd[ps[i].x][ps[i].y].number == 0 && (ps[i].x != prev.x || ps[i].y != prev.y)) {
			if (status == true) {
				if ((size_t)path_counter >= moves.size()) {
					moves.push_back(Area(number));
				}
				if ((size_t)index >= moves[path_counter].cells.size()) {
					moves[path_counter].cells.push_back(Cell());
				}
				moves[path_counter].cells[index] = Cell(Poss(ps[i].x, ps[i].y), number);
			}
			check_possible_moves(Poss(ps[i].x, ps[i].y), start, number, nb_count + 1, moves, path_counter, status, index + 1);
		}
	}
}


void Board::print_possible_moves(Poss start, Poss prev, int number, int nb_count) {
	vector<Area> moves;
	int path_counter = 0;
	check_possible_moves(start, prev, number, nb_count, moves, path_counter, true);
	moves.pop_back();
	for(Area &i : moves) {
		i.cells.insert(i.cells.begin(), Cell(start, number));
	}	
	int counter = 0;
	cout << moves.size() << endl;
	for(Area temp : moves) {
		for(Cell k : temp.cells) {
			cout << "[" << counter << "]: " << "poss(" << k.coord.x+1 << ", " << k.coord.y+1 << ")" << endl;  	
		}	
		counter++;
	}		


}	

void Board::make_move(Poss start, Poss prev, int number, int nb_count) {
	vector<Area> moves;
	int path_counter = 0;
	check_possible_moves(start, prev, number, nb_count, moves, path_counter, true);
	if(moves.size() > 0) {
	       	moves.pop_back();
	}


	for(Area &i : moves) {
		i.cells.insert(i.cells.begin(), Cell(start, number));
	}	
	if(path_counter == 1 && is_touch_same_number(moves[0]) == false) {
		move_on_board(moves[0]);
		return;		
	}
	
	int not_blocked_moves_counter = 0;
	Area not_blocked_move;
	for(Area i : moves) {
		if(check_is_any_cells_blocked(i) == false) {
			not_blocked_move = i;
			not_blocked_moves_counter++;	
		}
	}
	if(not_blocked_moves_counter == 1 && is_touch_same_number(not_blocked_move) == false) {
		move_on_board(not_blocked_move);
	}

	
}

void Board::move_on_board(Area move, bool status) {
	//Move on board
	if(status == true) {
		for(Cell k : move.cells) {
			bd[k.coord.x][k.coord.y].number = move.number;
			bd[k.coord.x][k.coord.y].is_visited = true;
			
		}
		return;
	}
	//Remove move on boad
	for(size_t i = 1; i < move.cells.size(); i++) {
		bd[move.cells[i].coord.x][move.cells[i].coord.y].number = 0;
		bd[move.cells[i].coord.x][move.cells[i].coord.y].is_visited = false;
	}	
	if(move.cells.size() > 0) bd[move.cells[0].coord.x][move.cells[0].coord.y].is_visited = false;
}	


void Board::solve() {
	parse_areas();
//	while(is_board_solved()) {
	for(size_t j = 0; j < 10; j++) {
		for(size_t i = 0; i < bd.size(); i++) {
			for(size_t k = 0; k < bd[0].size(); k++) {
				if(bd[i][k].number != 0 && bd[i][k].is_visited == false) {
					make_move(Poss(i, k), Poss(-1, -1), bd[i][k].number, 1);
				///	print_board();
				}
			}		
		}
	}	
	//	parse_areas();
//	}	
}	

bool Board::check_is_any_cells_blocked(Area move) {
	move_on_board(move);
	for(Cell i : move.cells) {
		Poss ps[4] = { Poss(i.coord.x-1, i.coord.y), Poss(i.coord.x+1, i.coord.y), Poss(i.coord.x, i.coord.y-1), Poss(i.coord.x, i.coord.y+1) };
		for(int k = 0; k < 4; k++) {
			if(ps[k].x >= 0 && (size_t)ps[k].x < bd.size() && ps[k].y >= 0 && (size_t)ps[k].y < bd[0].size()) {
				if(bd[ps[k].x][ps[k].y].number != 0 && bd[ps[k].x][ps[k].y].number != move.number && bd[ps[k].x][ps[k].y].is_visited == false) {
					int path_counter = 0;
					vector<Area> not_used;
					check_possible_moves(ps[k], Poss(-1, -1), bd[ps[k].x][ps[k].y].number, 1, not_used, path_counter);	

					if(path_counter == 0) {
						move_on_board(move, false);	
					       	return true;
					}       	
				}	
			}	
		}	
	}
	move_on_board(move, false);	
	return false;
}	


void Board::show_visited_cells() {
	for(size_t i = 0; i < bd.size(); i++) {
		for(size_t k = 0; k < bd[0].size(); k++) {
			if(bd[i][k].is_visited == true) {
				bd[i][k].number = -1;
			}	
		}	
	}	

}	



bool Board::is_touch_same_number(const Area move) {
	for(const Cell i : move.cells) {
		Poss ps[4] = { Poss(i.coord.x-1, i.coord.y), Poss(i.coord.x+1, i.coord.y), Poss(i.coord.x, i.coord.y-1), Poss(i.coord.x, i.coord.y+1) };
		for(int k = 0; k < 4; k++) {
			if(ps[k].x >= 0 && (size_t)ps[k].x < bd.size() && ps[k].y >= 0 && (size_t)ps[k].y < bd[0].size()) {
				if(bd[ps[k].x][ps[k].y].number == move.number) {
					bool status = false;
					for(const Cell j : move.cells) {
						if(ps[k].x == j.coord.x && ps[k].y == j.coord.y) {
							status = true;	
							break;
						}	
					}
					if(status == false) return true;	
				}	
			}	
		}	
	}
	return false;
}	



	





