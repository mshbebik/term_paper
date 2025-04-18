#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <fstream>
#include <algorithm> // for swap() function
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
/*
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
*/
/*
int Board::check_possible_moves(Cell cell) {
	bool hit_status = false;
	int path_counter = 0;
	int cells_counter = 1;
	vector<Poss> stack;
	vector<Poss> used_cells;
	used_cells.push_back(cell.coord);
	bd[cell.coord.x][cell.coord.y].is_visited = true;
	stack.push_back(cell.coord);
	for(int i = 0; stack.size() > 0; ) {
		if(cells_counter == cell.number) {
			stack.pop_back();
			i--;
			path_counter++;
			cells_counter--;
			continue;
		}	
		Poss ps[4] = { Poss(stack[i].x-1, stack[i].y), Poss(stack[i].x+1, stack[i].y), Poss(stack[i].x, stack[i].y-1), Poss(stack[i].x, stack[i].y+1) };
		for(int k = 0; k < 4; k++) {
			if(ps[k].x < 0 || (size_t)ps[k].x >= bd.size() || ps[k].y < 0 || (size_t)ps[k].y >= bd[0].size()) continue; 
			if((bd[ps[k].x][ps[k].y].number == 0 || bd[ps[k].x][ps[k].y].number == cell.number) && bd[ps[k].x][ps[k].y].is_visited == false) {
				//cout << "poss(" << ps[k].x+1 << ", " << ps[k].y+1 << ")" << endl;
				//cout << path_counter << endl; 
				//cout << "//" << endl; 
				//for(Poss i : stack) {
					
			//		cout << "(" << i.x+1 << ", " << i.y+1 << ")" << endl;
			//	}	
				
			//	cout << "//" << endl; 

				used_cells.push_back(Poss(ps[k].x, ps[k].y));
				stack.push_back(Poss(ps[k].x, ps[k].y));
				bd[ps[k].x][ps[k].y].is_visited = true;
				cells_counter++;
				i++;
				hit_status = true;
				break;
			}	
		}
		if(hit_status == false) {	
			stack.pop_back();
			i--;
			cells_counter--;
		}
		hit_status = false;
	}
	if(used_cells.size() > 0) {
		for(Poss i : used_cells) {
			bd[i.x][i.y].is_visited = false;
		}	
	}	
	return path_counter;
}	
*/

int Board::check_possible_moves(Cell cell) {
	vector<Poss> stack;
	vector<Poss> used_cells;
	int path_counter = 0;
	int cell_counter = 1;
	stack.push_back(cell.coord);
	bd[cell.coord.x][cell.coord.y].is_in_stack = true;	

		
	while(stack.size() > 0) {
		
		if(path_counter > 10) {
			for(Poss l : stack) {
				bd[l.x][l.y].is_in_stack = false;
				bd[l.x][l.y].in_stack.went_back_near = 0;
				for(int p = 0; bd[l.x][l.y].in_stack.blocked.size() > 0; p++) {
					bd[bd[l.x][l.y].in_stack.blocked[p].x][bd[l.x][l.y].in_stack.blocked[p].y].is_visited = false;
					bd[l.x][l.y].in_stack.blocked.pop_back();
				}	
				while(bd[l.x][l.y].in_stack.blocked_count.size() > 0) {
					bd[l.x][l.y].in_stack.blocked_count.pop_back();
				}	
				bd[l.x][l.y].in_stack.blocked_count.push_back(0);
				bd[l.x][l.y].is_visited = false;

			}	
		       	break;
		}
		

		Poss ps[4] = { Poss(stack[stack.size()-1].x-1, stack[stack.size()-1].y), Poss(stack[stack.size()-1].x+1, stack[stack.size()-1].y),
		       	       Poss(stack[stack.size()-1].x, stack[stack.size()-1].y-1), Poss(stack[stack.size()-1].x, stack[stack.size()-1].y+1) };

	//	for(Poss p : stack) {
	//		cout << "(" << p.x+1 << ", " << p.y+1 << ")" << " ";
	//	}	
	//	cout << endl;
	//	cout << path_counter << endl;
		int k;
		for(k = 0; k < 4; k++) {
			if(ps[k].x < 0 || (size_t)ps[k].x >= bd.size() || ps[k].y < 0 || (size_t)ps[k].y >= bd[0].size()) continue; 
			if((bd[ps[k].x][ps[k].y].number == 0 || bd[ps[k].x][ps[k].y].number == cell.number) && bd[ps[k].x][ps[k].y].is_visited == false) {
				if(bd[ps[k].x][ps[k].y].is_in_stack == true) {
					bd[stack.back().x][stack.back().y].is_visited = true;
					bd[stack.back().x][stack.back().y].in_stack.is_went_back = true;
					bd[ps[k].x][ps[k].y].in_stack.blocked.push_back(stack.back());
					bd[ps[k].x][ps[k].y].in_stack.blocked_count.push_back(1);
			       		stack.push_back(Poss(ps[k].x, ps[k].y));
					bd[stack.back().x][stack.back().y].in_stack.went_back_near++;
					break;
					
				}	
				//default
			       	stack.push_back(Poss(ps[k].x, ps[k].y));
				cell_counter++;
								
				bd[ps[k].x][ps[k].y].is_in_stack = true;
				//default
				if(cell_counter == cell.number) {

		//			cout << "== (" << ps[k].x+1 << ", " << ps[k].y+1 << ")" << endl; 
					cell_counter--;
					path_counter++;
					bd[ps[k].x][ps[k].y].is_visited = true;
					stack.pop_back();
					bd[ps[k].x][ps[k].y].is_in_stack = false;
					bd[stack.back().x][stack.back().y].in_stack.blocked.push_back(Poss(ps[k].x, ps[k].y));
					bd[stack.back().x][stack.back().y].in_stack.blocked_count.back()++;
					continue;
				}
				break;
			}	
		//	cout << "xx (" << ps[k].x+1 << ", " << ps[k].y+1 << ")" << endl; 

		}

		if(k == 4 && bd[stack.back().x][stack.back().y].in_stack.went_back_near != 0) {
		//	for(Poss j : bd[stack.back().x][stack.back().y].in_stack.blocked) {
		//		bd[j.x][j.y].is_visited = false;
		//	}	
		//	cout << "!=0: (" << stack.back().x+1 << ", " << stack.back().y+1 << ")" << endl; 
			for(int j = 0; j < bd[stack.back().x][stack.back().y].in_stack.blocked_count.back(); j++) {
				bd[bd[stack.back().x][stack.back().y].in_stack.blocked[bd[stack.back().x][stack.back().y].in_stack.blocked.size()-j-1].x][bd[stack.back().x][stack.back().y].in_stack.blocked[bd[stack.back().x][stack.back().y].in_stack.blocked.size()-j-1].y].is_visited = false;
			}	
			for(int j = 0; j < bd[stack.back().x][stack.back().y].in_stack.blocked_count.back(); j++) {
				bd[stack.back().x][stack.back().y].in_stack.blocked.pop_back();
			}	
			bd[stack.back().x][stack.back().y].in_stack.blocked_count.pop_back();
		//	bd[stack.back().x][stack.back().y].in_stack.blocked.clear();
			bd[stack.back().x][stack.back().y].in_stack.went_back_near--;
			if(bd[stack.back().x][stack.back().y].in_stack.is_went_back == true) bd[stack.back().x][stack.back().y].in_stack.is_went_back = false;
			bd[stack[stack.size()-2].x][stack[stack.size()-2].y].in_stack.blocked.push_back(stack.back());	
			bd[stack[stack.size()-2].x][stack[stack.size()-2].y].in_stack.blocked_count.back()++;	
			bd[stack.back().x][stack.back().y].is_visited = true;	
			stack.pop_back();
			continue;
		}	
		if(k == 4 && bd[stack.back().x][stack.back().y].in_stack.went_back_near == 0) {
		//	for(Poss j : bd[stack.back().x][stack.back().y].in_stack.blocked) {
		//		bd[j.x][j.y].is_visited = false;
		//	}	
		//	cout << "==0: (" << stack.back().x+1 << ", " << stack.back().y+1 << ")" << endl; 
			for(int j = 0; j < bd[stack.back().x][stack.back().y].in_stack.blocked_count.back(); j++) {
				bd[bd[stack.back().x][stack.back().y].in_stack.blocked[bd[stack.back().x][stack.back().y].in_stack.blocked.size()-j-1].x][bd[stack.back().x][stack.back().y].in_stack.blocked[bd[stack.back().x][stack.back().y].in_stack.blocked.size()-j-1].y].is_visited = false;
			}	
			for(int j = 0; j < bd[stack.back().x][stack.back().y].in_stack.blocked_count.back(); j++) {
				bd[stack.back().x][stack.back().y].in_stack.blocked.pop_back();
			}	
		//	bd[stack.back().x][stack.back().y].in_stack.blocked.clear();
			bd[stack.back().x][stack.back().y].in_stack.blocked_count.back() = 0;
			if(bd[stack.back().x][stack.back().y].in_stack.is_went_back == true) bd[stack.back().x][stack.back().y].in_stack.is_went_back = false;
			if(stack.size() > 1) bd[stack[stack.size()-2].x][stack[stack.size()-2].y].in_stack.blocked.push_back(stack.back());	
			if(stack.size() > 1) bd[stack[stack.size()-2].x][stack[stack.size()-2].y].in_stack.blocked_count.back()++;	
			bd[stack.back().x][stack.back().y].is_visited = true;
			bd[stack.back().x][stack.back().y].is_in_stack = false;
			cell_counter--;
			stack.pop_back();
			
		}	

	}	
	bd[cell.coord.x][cell.coord.y].is_visited = false;
	return path_counter;
}

vector<Area> Board::get_possible_moves(Cell cell) {
	vector<Area>moves;
	vector<Poss> stack;
	vector<Poss> used_cells;
	vector<Poss> went_back;
	int path_counter = 0;
	int cell_counter = 1;
//	int went_trail = 0;
//	bool status = false;
	moves.push_back(Area(cell.number));
	moves[path_counter].cells.push_back(cell);
	stack.push_back(cell.coord);
	bd[cell.coord.x][cell.coord.y].is_in_stack = true;	

		
	while(stack.size() > 0) {
		
		if(path_counter > 200) {
			for(Poss l : stack) {
				bd[l.x][l.y].is_in_stack = false;
				bd[l.x][l.y].in_stack.went_back_near = 0;
				for(int p = 0; bd[l.x][l.y].in_stack.blocked.size() > 0; p++) {
					bd[bd[l.x][l.y].in_stack.blocked[p].x][bd[l.x][l.y].in_stack.blocked[p].y].is_visited = false;
					bd[l.x][l.y].in_stack.blocked.pop_back();
				}	
				while(bd[l.x][l.y].in_stack.blocked_count.size() > 0) {
					bd[l.x][l.y].in_stack.blocked_count.pop_back();
				}	
				bd[l.x][l.y].in_stack.blocked_count.push_back(0);
				bd[l.x][l.y].is_visited = false;

			}	
		       	break;
		}
			
		Poss ps[4] = { Poss(stack[stack.size()-1].x-1, stack[stack.size()-1].y), Poss(stack[stack.size()-1].x+1, stack[stack.size()-1].y),
		       	       Poss(stack[stack.size()-1].x, stack[stack.size()-1].y-1), Poss(stack[stack.size()-1].x, stack[stack.size()-1].y+1) };

	//	for(Poss p : stack) {
	//		cout << "(" << p.x+1 << ", " << p.y+1 << ")" << " ";
	//	}	
	//	cout << endl;
	//	cout << path_counter << endl;
		
		int k;
		for(k = 0; k < 4; k++) {
			if(ps[k].x < 0 || (size_t)ps[k].x >= bd.size() || ps[k].y < 0 || (size_t)ps[k].y >= bd[0].size()) continue; 
			if((bd[ps[k].x][ps[k].y].number == 0 || bd[ps[k].x][ps[k].y].number == cell.number) && bd[ps[k].x][ps[k].y].is_visited == false) {
				if(bd[ps[k].x][ps[k].y].is_in_stack == true) {
					bd[stack.back().x][stack.back().y].is_visited = true;
					bd[stack.back().x][stack.back().y].in_stack.is_went_back = true;
					went_back.push_back(Poss(stack.back().x, stack.back().y));
					bd[ps[k].x][ps[k].y].in_stack.blocked.push_back(stack.back());
					bd[ps[k].x][ps[k].y].in_stack.blocked_count.push_back(1);
			       		stack.push_back(Poss(ps[k].x, ps[k].y));
					bd[stack.back().x][stack.back().y].in_stack.went_back_near++;
					break;
					
				}	
				//default
			       	stack.push_back(Poss(ps[k].x, ps[k].y));
				cell_counter++;
				if(bd[ps[k].x][ps[k].y].number == cell.number) { 
					moves[path_counter].cells.push_back(Cell(Poss(ps[k].x, ps[k].y), cell.number, true));
				}
				else{
					moves[path_counter].cells.push_back(Cell(Poss(ps[k].x, ps[k].y), cell.number, false));
				}	

				bd[ps[k].x][ps[k].y].is_in_stack = true;
				//default
				if(cell_counter == cell.number) {

				//	cout << "== (" << ps[k].x+1 << ", " << ps[k].y+1 << ")" << endl; 
					cell_counter--;
					path_counter++;
					moves.push_back(Area(cell.number));
					for(Cell tmp : moves[path_counter-1].cells) {
						moves[path_counter].cells.push_back(tmp);
					}
					moves[path_counter].cells.pop_back();	
					bd[ps[k].x][ps[k].y].is_visited = true;
					stack.pop_back();
					bd[ps[k].x][ps[k].y].is_in_stack = false;
					bd[stack.back().x][stack.back().y].in_stack.blocked.push_back(Poss(ps[k].x, ps[k].y));
					bd[stack.back().x][stack.back().y].in_stack.blocked_count.back()++;
					continue;
				}
				break;
			}	
//			cout << "xx (" << ps[k].x+1 << ", " << ps[k].y+1 << ")" << endl; 


		}
		if(k == 4 && bd[stack.back().x][stack.back().y].in_stack.went_back_near != 0 /*&& went_trail == 0*/) {
		//	for(Poss j : bd[stack.back().x][stack.back().y].in_stack.blocked) {
		//		bd[j.x][j.y].is_visited = false;
		//	}	
//			cout << "!=0: (" << stack.back().x+1 << ", " << stack.back().y+1 << ")" << endl; 
			for(int j = 0; j < bd[stack.back().x][stack.back().y].in_stack.blocked_count.back(); j++) {
				bd[bd[stack.back().x][stack.back().y].in_stack.blocked[bd[stack.back().x][stack.back().y].in_stack.blocked.size()-j-1].x][bd[stack.back().x][stack.back().y].in_stack.blocked[bd[stack.back().x][stack.back().y].in_stack.blocked.size()-j-1].y].is_visited = false;
			}	
			for(int j = 0; j < bd[stack.back().x][stack.back().y].in_stack.blocked_count.back(); j++) {
				bd[stack.back().x][stack.back().y].in_stack.blocked.pop_back();
			}	
			bd[stack.back().x][stack.back().y].in_stack.blocked_count.pop_back();
		//	bd[stack.back().x][stack.back().y].in_stack.blocked.clear();
			if(went_back.size() > 0) went_back.pop_back();
			bd[stack.back().x][stack.back().y].in_stack.went_back_near--;
			if(bd[stack.back().x][stack.back().y].in_stack.is_went_back == true) bd[stack.back().x][stack.back().y].in_stack.is_went_back = false;
			bd[stack[stack.size()-2].x][stack[stack.size()-2].y].in_stack.blocked.push_back(stack.back());	
			bd[stack[stack.size()-2].x][stack[stack.size()-2].y].in_stack.blocked_count.back()++;	
			bd[stack.back().x][stack.back().y].is_visited = true;	
			stack.pop_back();
			continue;
		}	
		if(k == 4 && went_back.size() != 0 && (stack.back().x != went_back.back().x || stack.back().y != went_back.back().y)) {
		//	cout << went_back.size() << endl;
			bd[went_back.back().x][went_back.back().y].is_visited = false;
			bd[stack.back().x][stack.back().y].is_visited = true;
			stack.push_back(Poss(went_back.back().x, went_back.back().y));
			bd[went_back.back().x][went_back.back().y].in_stack.blocked_count.push_back(0);
			bd[went_back.back().x][went_back.back().y].in_stack.is_jumped_back = true;
			went_back.pop_back();
//			went_trail++;
//			status = true;
			continue;


		}	
		if(k == 4 && (bd[stack.back().x][stack.back().y].in_stack.went_back_near == 0 /*|| status == true*/)) {
		//	for(Poss j : bd[stack.back().x][stack.back().y].in_stack.blocked) {
		//		bd[j.x][j.y].is_visited = false;
		//	}	
		//	
//			cout << "==0: (" << stack.back().x+1 << ", " << stack.back().y+1 << "),   " << went_back.size() << endl; 
		//	cout << "ww: (" << went_back.back().x+1 << ", " << went_back.back().y+1 << "),   " << went_back.size() << endl; 
			for(int j = 0; j < bd[stack.back().x][stack.back().y].in_stack.blocked_count.back(); j++) {
				bd[bd[stack.back().x][stack.back().y].in_stack.blocked[bd[stack.back().x][stack.back().y].in_stack.blocked.size()-j-1].x][bd[stack.back().x][stack.back().y].in_stack.blocked[bd[stack.back().x][stack.back().y].in_stack.blocked.size()-j-1].y].is_visited = false;
			}	
			for(int j = 0; j < bd[stack.back().x][stack.back().y].in_stack.blocked_count.back(); j++) {
				bd[stack.back().x][stack.back().y].in_stack.blocked.pop_back();
			}	
		//	bd[stack.back().x][stack.back().y].in_stack.blocked.clear();
			if(bd[stack.back().x][stack.back().y].in_stack.is_jumped_back == true) bd[stack.back().x][stack.back().y].in_stack.blocked_count.pop_back(); 
			else bd[stack.back().x][stack.back().y].in_stack.blocked_count.back() = 0;
			if(bd[stack.back().x][stack.back().y].in_stack.is_went_back == true && bd[stack.back().x][stack.back().y].in_stack.is_jumped_back == false) bd[stack.back().x][stack.back().y].in_stack.is_went_back = false;
			if(bd[stack.back().x][stack.back().y].in_stack.is_jumped_back == false) {
				if(stack.size() > 1) bd[stack[stack.size()-2].x][stack[stack.size()-2].y].in_stack.blocked.push_back(stack.back());	
				if(stack.size() > 1) bd[stack[stack.size()-2].x][stack[stack.size()-2].y].in_stack.blocked_count.back()++;	
				bd[stack.back().x][stack.back().y].is_in_stack = false;
				moves[path_counter].cells.pop_back();
				cell_counter--;
			}
			bd[stack.back().x][stack.back().y].is_visited = true;
			if(bd[stack.back().x][stack.back().y].in_stack.is_jumped_back == true) bd[stack.back().x][stack.back().y].in_stack.is_jumped_back = false; 
		//	bd[stack[stack.size()-1-went_trail].x][stack[stack.size()-1-went_trail].y].is_visited = false;
//			if(went_trail > 0) went_trail--;
//			if(went_trail == 0) status = false;
			stack.pop_back();
			
		}	

	}
	if(moves.size() > 0) moves.pop_back();	
	bd[cell.coord.x][cell.coord.y].is_visited = false;
	return moves;
}		




/*
vector<Area> Board::get_possible_moves(Cell cell) {
	bool hit_status = false;
	int path_counter = 0;
	int cells_counter = 1;
	vector<Area> moves;
	vector<Cell> stack;
	vector<Poss> used_cells;
	cell.is_visited = true;
	moves.push_back(Area(cell.number));
	moves[path_counter].cells.push_back(cell);	
	used_cells.push_back(cell.coord);
	bd[cell.coord.x][cell.coord.y].is_visited = true;
	stack.push_back(cell);
	for(int i = 0; stack.size() > 0; ) {
		if(cells_counter == cell.number) {
			stack.pop_back();
			i--;
			path_counter++;
			moves.push_back(Area(cell.number));
			for(Cell j : stack) {
				moves[path_counter].cells.push_back(Cell(j.coord, j.number, j.is_visited));
			}
			cells_counter--;
			continue;
		}	
		if(stack.size() == 1) {
		//	cout << "sdfsdfsdf " << endl; 
			while((int)moves[path_counter].cells.size() > 1 ) {
				moves[path_counter].cells.pop_back();
			}	
		}
		if(stack.size() == 2) {
		//	cout << "sdfsdfsdf " << endl; 
			while((int)moves[path_counter].cells.size() > 2 ) {
				moves[path_counter].cells.pop_back();
			}	
		}

		
		Poss ps[4] = { Poss(stack[i].coord.x-1, stack[i].coord.y), Poss(stack[i].coord.x+1, stack[i].coord.y), Poss(stack[i].coord.x, stack[i].coord.y-1), Poss(stack[i].coord.x, stack[i].coord.y+1) };
		for(int k = 0; k < 4; k++) {
			if(ps[k].x < 0 || (size_t)ps[k].x >= bd.size() || ps[k].y < 0 || (size_t)ps[k].y >= bd[0].size()) continue; 
			if((bd[ps[k].x][ps[k].y].number == 0 || bd[ps[k].x][ps[k].y].number == cell.number) && bd[ps[k].x][ps[k].y].is_visited == false) {

				if(bd[ps[k].x][ps[k].y].number == cell.number) {
					moves[path_counter].cells.push_back(Cell(Poss(ps[k].x, ps[k].y), cell.number, true));	
					stack.push_back(Cell(Poss(ps[k].x, ps[k].y), cell.number, true));
				}
				else {
					moves[path_counter].cells.push_back(Cell(Poss(ps[k].x, ps[k].y), cell.number, false));	
					stack.push_back(Cell(Poss(ps[k].x, ps[k].y), cell.number, false));
				}	
				used_cells.push_back(Poss(ps[k].x, ps[k].y));
				bd[ps[k].x][ps[k].y].is_visited = true;
				cells_counter++;
				i++;
				hit_status = true;
				break;
			}	
		}
		if(hit_status == false) {	
			stack.pop_back();
			i--;
			cells_counter--;
		}
		hit_status = false;
	}
	if(used_cells.size() > 0) {
		for(Poss i : used_cells) {
			bd[i.x][i.y].is_visited = false;
		}	
	}
	if(used_cells.size() == 1) moves.pop_back();
	else { moves.pop_back(); }	
	return moves;
}
*/

void Board::print_possible_moves(Cell cell) {
	vector<Area> moves = get_possible_moves(cell);
	int counter = 0;
	cout << moves.size() << endl;
	for(Area temp : moves) {
//		if(temp.cells.back().coord.x == 9 && temp.cells.back().coord.y == 5) {
		for(Cell k : temp.cells) {
			cout << "[" << counter << "]: " << "poss(" << k.coord.x+1 << ", " << k.coord.y+1 << ")" << endl;  	
			bd[k.coord.x][k.coord.y].number = moves[0].number;
		}	
		print_board();	
		for(Cell k : temp.cells) {
			bd[k.coord.x][k.coord.y].number = 0;
		}	
//		}
		counter++;
	}		



}	

void Board::make_move(Cell cell) {
	vector<Area> moves = get_possible_moves(cell);
//	cout << "Number [" << moves[0].number << "]: " << moves.size() << endl; 

	
//	int cnt = 0;
//	for(Area i : moves) {
//		for(Cell k : i.cells) {
//			cout << cnt << " [" << k.coord.x+1 << ", " << k.coord.y+1 << "]" << endl;
//		}
//		cnt++;	
//	}	

	if((int)moves.size() == 1 && is_touch_same_number(moves[0]) == false) {
		move_on_board(moves[0]);
		return;		
	}
	
	int not_blocked_moves_counter = 0;
	vector<Area> not_blocked_move;
	for(Area i : moves) {
		if(check_is_any_cells_blocked(i) == false) {
			not_blocked_move.push_back(i);
			not_blocked_moves_counter++;	
		}
	}
	if(not_blocked_moves_counter == 1 && is_touch_same_number(not_blocked_move[0]) == false) {
		move_on_board(not_blocked_move[0]);
		return;
	}

	Cell possible_move;
	if(check_single_cell_moves(cell, possible_move) == 1) {
		single_cell_move_on_board(possible_move);	
	}	

	
	vector<Area> mvs;
	vector<int> free_space;
	for(Area i : not_blocked_move) {
		if(is_piercies(i) >= 2 && is_touch_same_number(i) == false) {
			//cout << "Block >= 2: " << moves[0].number << endl;
			mvs.push_back(i);
			free_space.push_back(check_free_space(i));
		//	if(check_is_any_cells_blocked_deep(i) == false) {
			      // 	move_on_board(i);
		//	}	
		}	
	}
	if(mvs.size() == 0) return;
	int temp = 0;
	Area move;	
	for(size_t i = 0; i < mvs.size(); i++) {
		if(free_space[i] > temp) {
			temp = free_space[i];
			move = mvs[i];
		}	
	}
	cout << temp << "," << move.number << endl;
	move_on_board(move);	
//	vector<Area> groat;
//	vector<int> diff;
//	for(Area i : not_blocked_move) {
//		int difference = 0;
//		if(is_groat_move(i, difference) == true && is_touch_same_number(i) == false) {
//			groat.push_back(i);
//			diff.push_back(difference);
//		}		
//	}	
//	
//	int tmp = 9;
//	Area move;
//	for(size_t i = 0; i < groat.size(); i++) {
//		if(diff[i] < tmp) {
//			tmp = diff[i];
//			move = groat[i];
//		}	
//	}	
//	move_on_board(move);
	
	
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
	for(Cell i : move.cells) {
		if(i.is_visited == true) {
			bd[i.coord.x][i.coord.y].is_visited = false;
			continue;
		}	
		bd[i.coord.x][i.coord.y].number = 0;
		bd[i.coord.x][i.coord.y].is_visited = false;
	}	
}	


void Board::solve() {
	parse_areas();
//	while(is_board_solved()) {
	for(size_t j = 0; j < 1; j++) {

		for(size_t i = 0; i < bd.size(); i++) {
			for(size_t k = 0; k < bd[0].size(); k++) {
				if(bd[i][k].number != 0 && bd[i][k].is_visited == false) {
					make_move(Cell(Poss(i, k), bd[i][k].number, true));
				//	print_board();
				//	show_visited_cells();
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
					int path_counter = check_possible_moves(Cell(ps[k], bd[ps[k].x][ps[k].y].number, false));	
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

bool Board::check_is_any_cells_blocked_deep(Area move) {
	move_on_board(move);
	for(Cell i : move.cells) {
		Poss ps[4] = { Poss(i.coord.x-1, i.coord.y), Poss(i.coord.x+1, i.coord.y), Poss(i.coord.x, i.coord.y-1), Poss(i.coord.x, i.coord.y+1) };
		for(int k = 0; k < 4; k++) {
			if(ps[k].x >= 0 && (size_t)ps[k].x < bd.size() && ps[k].y >= 0 && (size_t)ps[k].y < bd[0].size()) {
				if(bd[ps[k].x][ps[k].y].number != 0 && bd[ps[k].x][ps[k].y].number != move.number && bd[ps[k].x][ps[k].y].is_visited == false) {
					vector<Area>moves = get_possible_moves(Cell(ps[k], bd[ps[k].x][ps[k].y].number, true));	
					for(Area mv : moves) {
						if(check_is_any_cells_blocked(mv) == true) {
							move_on_board(move, false);	
						       	return true;
						}
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



bool Board::is_touch_same_number(Area move) {
	for(Cell i : move.cells) {
		Poss ps[4] = { Poss(i.coord.x-1, i.coord.y), Poss(i.coord.x+1, i.coord.y), Poss(i.coord.x, i.coord.y-1), Poss(i.coord.x, i.coord.y+1) };
		for(int k = 0; k < 4; k++) {
			if(ps[k].x >= 0 && (size_t)ps[k].x < bd.size() && ps[k].y >= 0 && (size_t)ps[k].y < bd[0].size()) {
				if(bd[ps[k].x][ps[k].y].number == move.number) {
					bool status = false;
					for(Cell j : move.cells) {
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


int Board::is_piercies(Area move) {
	int count = 0;
	vector<Cell> tmp;
	for(Cell i : move.cells) {
		if(i.is_visited == true) {
			tmp.push_back(i);
		}
	}
	
	if(tmp.size() <= 2) return (int)tmp.size();	
	for(size_t i = 0; i < tmp.size()-1; i++) {
		if(is_stand_near(tmp[i], tmp[i+1]) == false) {
			count++;
		}	
	}
	if(is_stand_near(tmp[0], tmp[tmp.size()-1]) == false) {
		count++;
	}		
	return count;
}



//int Board::is_piercies(Area move) {
//
//	int count = 0;
//	for(Cell i : move.cells) {
//		if(i.is_visited == true && is_stand_near(move.cells[0], i) == false) {
//		//	cout << i.number << " [" << i.coord.x+1 << ", " << i.coord.y+1 << "]" << endl;
//			count++;
//		}
//		
//	}	
//	return count;
//}	

	


bool Board::is_stand_near(Cell start, Cell check) {
	if(check.coord.x == start.coord.x && check.coord.y == start.coord.y) return false;
	if(check.coord.x == start.coord.x - 1 && check.coord.y == start.coord.y) return true;
	if(check.coord.x == start.coord.x + 1 && check.coord.y == start.coord.y) return true;
	if(check.coord.x == start.coord.x && check.coord.y == start.coord.y - 1) return true;
	if(check.coord.x == start.coord.x && check.coord.y == start.coord.y + 1) return true;
	return false;
}	


bool Board::is_groat_move(Area move, int &difference) {
	int blocked = 0;
	int all = 0;
	for(Cell i : move.cells) {
		Poss ps[4] = { Poss(i.coord.x-1, i.coord.y), Poss(i.coord.x+1, i.coord.y), Poss(i.coord.x, i.coord.y-1), Poss(i.coord.x, i.coord.y+1) };
		for(int k = 0; k < 4; k++) {
			if(ps[k].x >= 0 && (size_t)ps[k].x < bd.size() && ps[k].y >= 0 && (size_t)ps[k].y < bd[0].size()) {
				if(bd[ps[k].x][ps[k].y].number != move.number && bd[ps[k].x][ps[k].y].number != 0) {
					blocked++;
				}	
				if(bd[ps[k].x][ps[k].y].number == move.number) {
					bool is_move_cell = false;
					for(Cell j : move.cells) {
						if(ps[k].x == j.coord.x && ps[k].y == j.coord.y) {
							is_move_cell = true;	
							break;
						}	
					}
					if(is_move_cell == false) blocked++;	
				}	
			}
			else {
				blocked++;
			}	
		}	
	}
	if(move.cells.size() == 2) all = 6;
	if(move.cells.size() > 2) all = 6 + ((move.number-2) * 2);
	difference = all - blocked;
	return ((all - 1 == blocked) || (all-2 == blocked)) ? true : false;
}	



int Board::check_free_space(Area move) {
	int free_space = 0;
	for(Cell i : move.cells) {
		Poss ps[4] = { Poss(i.coord.x-1, i.coord.y), Poss(i.coord.x+1, i.coord.y), Poss(i.coord.x, i.coord.y-1), Poss(i.coord.x, i.coord.y+1) };
		for(int k = 0; k < 4; k++) {
			if(ps[k].x >= 0 && (size_t)ps[k].x < bd.size() && ps[k].y >= 0 && (size_t)ps[k].y < bd[0].size()) {
				if(bd[ps[k].x][ps[k].y].number == move.number) {
					bool status = false;
					for(Cell j : move.cells) {
						if(ps[k].x == j.coord.x && ps[k].y == j.coord.y) {
							status = true;	
							break;
						}	
					}
					if(status == true) continue;	
					free_space++;
				}	
				else{
					free_space++;
				}	
			}	
			else {
				free_space++;
			}	
		}	
	}	
	return free_space;
}	


int Board::check_single_cell_moves(Cell cell, Cell &possible_move) {
	int moves = 0;
	vector<Cell> area;
	select_area_special(cell, area);
	if((int)area.size() == cell.number) {
		for(Cell i : area) {
			if(bd[i.coord.x][i.coord.y].is_on_going == true) {
				bd[i.coord.x][i.coord.y].is_on_going = false;
			}	
			if(bd[i.coord.x][i.coord.y].is_visited == false) {

				bd[i.coord.x][i.coord.y].is_visited = true;
			}	
		}	
		return 0;
	}	
	Poss ps[4] = { Poss(cell.coord.x-1, cell.coord.y), Poss(cell.coord.x+1, cell.coord.y), Poss(cell.coord.x, cell.coord.y-1), Poss(cell.coord.x, cell.coord.y+1) };
	for(int k = 0; k < 4; k++) {
		if(moves > 1) return moves;
		if(ps[k].x >= 0 && (size_t)ps[k].x < bd.size() && ps[k].y >= 0 && (size_t)ps[k].y < bd[0].size()) {
			if(bd[ps[k].x][ps[k].y].number == 0) {
				possible_move = Cell(Poss(ps[k].x, ps[k].y), cell.number, false);
				moves++;
			}		
		}
	}		
	return moves;
}	

void Board::select_area_special(Cell cell, vector<Cell> &area) {
	vector<Cell> stack;
	stack.push_back(cell);
	bd[stack.back().coord.x][stack.back().coord.y].is_visited;
	area.push_back(bd[stack.back().coord.x][stack.back().coord.y]);
	while(stack.size() > 0) {
		Poss ps[4] = {Poss(stack.back().coord.x-1, stack.back().coord.y), Poss(stack.back().coord.x+1, stack.back().coord.y), Poss(stack.back().coord.x, stack.back().coord.y-1), Poss(stack.back().coord.x, stack.back().coord.y+1) };

		int i = 0;
		for(i = 0; i < 4; i++) {
			if(ps[i].x < 0 || (size_t)ps[i].x >= bd.size() || ps[i].y < 0 || (size_t)ps[i].y >= bd[0].size()) continue;
			if(bd[ps[i].x][ps[i].y].number == cell.number && bd[ps[i].x][ps[i].y].is_visited == false) {
				bd[ps[i].x][ps[i].y].is_visited = true;
				area.push_back(bd[ps[i].x][ps[i].y]);
				stack.push_back(bd[ps[i].x][ps[i].y]);
				break;	
			}
		}
       		if(i == 4) stack.pop_back();	
	}
	for(Cell i : area) {
		bd[i.coord.x][i.coord.y].is_visited = false;
	}	
	area.pop_back();
}

void Board::single_cell_move_on_board(Cell move, bool status) {
	//Move on board
	if(status == true) {
		bd[move.coord.x][move.coord.y].number = move.number;
		bd[move.coord.x][move.coord.y].is_on_going = true;
		return;
	}
	//Remove move on boad
	if(move.is_visited == true) {
		bd[move.coord.x][move.coord.y].is_visited = false;
		return;
	}	
	bd[move.coord.x][move.coord.y].number = 0;
	bd[move.coord.x][move.coord.y].is_on_going = false;
}
