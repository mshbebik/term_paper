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
	cout << "   ";
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

		for(int a = 0; a < 10; a++) {
			cout << bd[i][a].number << " ";
		}
		cout << endl;
	}
	cout << endl;

}
/*
void Board::move(Poss start, Poss prev, int number, int nb_count) {
	if(start.x < 0 || start.x >= 10 || start.y < 0 || start.y >= 10) {
	 	return;
	}
	cout << "Dot: " << start.x << ", " << start.y << endl; 
	Poss ps[4] = { Poss(start.x-1, start.y), Poss(start.x+1, start.y), Poss(start.x, start.y-1), Poss(start.x, start.y+1) };
	for(int i = 0; i < 4; i++) {
		if(ps[i].x < 0 || ps[i].x >= 10 || ps[i].y < 0 || ps[i].y >= 10) return;
		if(nb_count == number) {
			count++;
			return;
		}
		if(bd[ps[i].x][ps[i].y] == 0 && (ps[i].x != prev.x || ps[i].y != prev.y)) {
			print_board();
			bd[ps[i].x][ps[i].y] = number;
			cout << "Total variants: " << get_count() << endl;
			move(Poss(ps[i].x, ps[i].y), start, number, nb_count+1);
		}
	} 
}
	

void Board::parse_board() {
	for(int i = 0; i < 10; i++) {
		for(int k = 0; k < 10; k++) {
			if(bd[i][k] != 0) {
				cells.push_back(Cell(bd[i][k], Poss(i, k));
			}
		}
	}


}
*/


	





