#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <fstream>
#include "board.h"
using namespace std;


/*
void Board::init_board() {
	for(int i = 0; i < 10; i++) {
		for(int k = 0; k < 10; k++) {
			bd[i][k] = 0;
		}
	}
	count = 0;
}
*/


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


void Board::check_possible_moves(Poss start, Poss prev, int number, int nb_count, vector<Area> &moves, int &path_counter, int index) {
	if(start.x < 0 || start.x >= 10 || start.y < 0 || start.y >= 10) {
	 	return;
	}
	Poss ps[4] = { Poss(start.x-1, start.y), Poss(start.x+1, start.y), Poss(start.x, start.y-1), Poss(start.x, start.y+1) };
	for(int i = 0; i < 4; i++) {
		if(ps[i].x < 0 || ps[i].x >= 10 || ps[i].y < 0 || ps[i].y >= 10) continue;
		if(nb_count == number) {
			moves.push_back(Area());
			moves[path_counter+1] = moves[path_counter];
			path_counter++;
			return;
		}
		if(bd[ps[i].x][ps[i].y].number == 0 && (ps[i].x != prev.x || ps[i].y != prev.y)) {
			if((size_t)path_counter >= moves.size()) {
				moves.push_back(Area());
			}
			if((size_t)index >= moves[path_counter].cells.size()) {
				moves[path_counter].cells.push_back(Cell());
			}			
			moves[path_counter].cells[index] = Cell(Poss(ps[i].x, ps[i].y), number);
			check_possible_moves(Poss(ps[i].x, ps[i].y), start, number, nb_count+1, moves, path_counter, index+1);
		}
	} 
}

void Board::print_possible_moves(Poss start, Poss prev, int number, int nb_count) {
	vector<Area> moves;
	int path_counter = 0;
	check_possible_moves(start, prev, number, nb_count, moves, path_counter);
	moves.pop_back();
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
	check_possible_moves(start, prev, number, nb_count, moves, path_counter);
	moves.pop_back();
	if(path_counter == 1) {
		for(Area temp : moves) {
			for(Cell k : temp.cells) {
				bd[k.coord.x][k.coord.x].number = number;
			}
		}
		return;		
	}		
}


void Board::solve() {
	parse_areas();
//	while(is_board_solved()) {
		for(size_t i = 0; i < bd.size(); i++) {
			for(size_t k = 0; k < bd[0].size(); k++) {
				if(bd[i][k].number != 0 && bd[i][k].is_visited == false) {
					make_move(Poss(i, k), Poss(-1, -1), bd[i][k].number, 1);
				}
			}		
		}
	//	parse_areas();
//	}	
}	

//void Board::check_blocked_cells() {
		

//}	
	


	





