#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <fstream>
#include <algorithm> // for swap() and min() functions
#include <cstdlib> // for system("cls")		     
#include <ctype.h> // for isdigit() and tolower()		  
#include "board.h"
using namespace std;



Board::Board() {

}	

//initialize grid from file constructor
Board::Board(string file_name) {
	init_from_file(file_name);
}


bool Board::init_from_user() {
	system("cls");	
	cout << "Enter the board row by row. Example: [1 3 0 4 5 6 \\n].\n" <<
		 "After finishing a row, press Enter (as shown in the example).\n" <<
		 "When the entire board is entered, press Enter again to submit.\n" << endl;

	//to track char symbols to decide when the user is done with input
	vector<char> track;
	int row = 0;
	while(true) {
		bd.push_back(vector<Cell>());
		cout << "Enter 'b' to go back." << endl;
		cout << "Enter [" << row+1 <<"] row or Enter button to submit: ";
		int column = 0;
		bool is_empty_row = true;
		bool rows_is_not_equal = false;
		bool board_is_empty = false;
		while(true) {
			char next_symbol = cin.peek();
			if(next_symbol == ' ' || next_symbol == '\n' || tolower(next_symbol) == 'b') {
				next_symbol = cin.get();
				track.push_back(next_symbol);	
				if(next_symbol == '\n' || next_symbol == 'b') { 
					if(track.size() == 1 && track.back() == '\n') {
						bd.pop_back();
						system("cls");	
						if(bd.size() == 0) {
							board_is_empty = true;
							track.clear();
							cout << "Enter the board row by row. Example: [1 3 0 4 5 6 \\n].\n" <<
							 "After finishing a row, press Enter (as shown in the example).\n" <<
							 "When the entire board is entered, press Enter again to submit.\n" << endl;
							break;
						}		
						return true;
					}
					if(track.size() == 1 && tolower(track[0]) == 'b') {
						bd.clear();
						while(cin.get() != '\n') {};
						return false;
					}	
					if((int)bd.size() > 1 && is_all_rows_equal() != true) {
						rows_is_not_equal = true;
						break;
					}	
					row++;
					track.clear();
				       	break;
				}	
				continue;
					
			}
			if(isdigit(next_symbol)) {
				is_empty_row = false;
				int number = 0;
				cin >> number;
				track.push_back(next_symbol);	
				bd[row].push_back(Cell(Poss(row, column), number));	
				column++;
			}
			else {
				next_symbol = cin.get();
				track.push_back(next_symbol);	
				continue;
			}	
		}
		if(board_is_empty) {
			continue;
		}	
		if(rows_is_not_equal) {
			if(fix_rows_size() == 1) continue;
			row = 0;
			continue;
		}	
		if(is_empty_row == true) {
			bd.pop_back();
			row--;
			system("cls");	
			cout << "Enter the board row by row. Example: [1 3 0 4 5 6 \\n].\n" <<
				 "After finishing a row, press Enter (as shown in the example).\n" <<
				 "When the entire board is entered, press Enter again to submit.\n" << endl;
			cout << endl;
			cout << "\033[31m### Enter row as shown in the example, all non digit symbols will be igonered! ###\033[0m" << endl;
			continue;
		}	
		system("cls");	
		print_board();
	}	
//	bd.pop_back();
	return true;
}	

bool Board::is_all_rows_equal() {
	int size = (int)bd[0].size();
	for(vector<Cell> i : bd) {
		if(size != (int)i.size()) return false;	
	}
	return true;	
}	

//to fix accident when user entered not equal number of symbols in rows.
//function return 1 when last 'wrong' row need to be rewrited and 2 when user choosed to rewrite entire board.
int Board::fix_rows_size() {
	cout << "\033[31m### Every row has to contain equal number of numbers! ###\033[0m" << endl;
	cout << "Enter 1 to rewrite last row, or 2 to rewrite whole board." << endl;
	char choice = ' ';
	cout << "Your choice [1/2]: ";
	while((choice = cin.get()) && (choice != '1' && choice != '2')) {
		while(cin.get() != '\n') {};
		cout << "\033[31m### Enter one of the option. ###\033[0m" << endl;
		cout << "Your choice [1/2]: ";
	}	
	if(choice == '1') {
		while(cin.get() != '\n') {};
		bd.pop_back();
		return 1;	
	}
	if(choice == '2') {
		while(cin.get() != '\n') {};
		bd.clear();
		return 2;	
	}		
	return 0;
}	


//initialize grid from file
void Board::init_from_file(string file_name) {
	vector<vector<int>> nums;
	try {
		nums = parse_file(file_name);
	} catch (...) {
		throw 0;
	}	
	for(size_t i = 0; i < nums.size(); i++) {
		bd.push_back(vector<Cell>());
		for(size_t k = 0; k < nums[0].size(); k++) {
			bd[i].push_back(Cell(Poss(i, k), nums[i][k]));	
		}
	} 
}

//parsing file and reterning finished board for further initialization
vector<vector <int>> Board::parse_file(string file_name) {
	ifstream file(file_name);
	
	try {
		if(!file.is_open()) {
			cerr << "\033[31mError: Can't open \"" << file_name << "\" file for parsing.\033[0m" << endl;
			throw 0;	
		}
	} catch (...) {
		throw 0;
	}	
	vector<vector <int>> nums;
	string line;
	while(getline(file, line)){
		try {
		nums.push_back(split_by_space_int(line));
		} catch (...) {
			cerr << endl << "\033[31mError: \"" << file_name << "\" contains non-numeric symbols.\033[0m" << endl;
			cerr << "\033[31mError: File must contain only digits separeted by spaces, as shown in the example\033[0m" << endl;
			throw 0;
		}	

	}	
	file.close();
	size_t width = nums[0].size();
	for(size_t i = 0; i < nums.size(); i++) {
		try {
			if(nums[i].size() != width) {
				cerr << endl << "\033[31mError: Inccorrect input in \"" << file_name << "\"\033[0m" << endl; 
				cerr << "\033[31mError: Rows has different lenght!\033[0m" << endl;
				throw 0;	
			}
		} catch (...) {
			throw 0;
		}	
	}	
	return nums;	
}

//gets lines from file and returning vectors with numbers
vector<int> Board::split_by_space_int(string line) {
	vector<int> splited;
	for(size_t i = 0; i < line.size(); i++) {
		if(isdigit(line[i])) {
			int number = 0;
			number += (line[i] - '0');
			while(isdigit(line[i+1])) {
				number *= 10;
				number += (line[++i] - '0');
			}		
			splited.push_back(number);		
			continue;
		}		
		if(line[i] != ' ') {
			throw 0;	
		}
	}
	return splited;
}
/*
void Board::solve() {
	//just fo print_board() when user input already solved board.
	bool is_grid_already_solved = true;
	while(!is_board_solved()) {
		is_grid_already_solved = false;
//	for(size_t j = 0; j < 11; j++) {
		bool is_main_cells_filled = true;
		int  moves_was_made = 0;
		for(size_t i = 0; i < bd.size(); i++) {
			for(size_t k = 0; k < bd[0].size(); k++) {
				if(bd[i][k].number != 0 && bd[i][k].is_visited == false) {
					is_main_cells_filled = false;
					if(make_move(Cell(Poss(i, k), bd[i][k].number, true))) moves_was_made++;
				//	show_visited_cells();
				}
			}		
		}
		if(is_main_cells_filled == true) {
			fill_free_space(); 
		}
		if(moves_was_made == 0 && is_main_cells_filled == false) {
			print_board();	
			cout << "I could'nt solve your grid :( " << endl;
			bd.clear();
			return;

		}	
		print_board();	
		
	}	
	if(is_grid_already_solved == true) print_board(); 
	cout << "The grid is solved !!!" << endl;
	bd.clear();
	//	parse_areas();
//	}	
//
}	
*/

void Board::solve_by_user() {
	system("cls");
	while(!is_board_solved()) {
		while(true) {
			print_board();
			// first number is coordinate x, second is y and last one is cell number.
			int storage[3];
		        for(int i = 0; i < 3; i++) {
				storage[i] = 0;
			}	
			cout << "Enter coordinates of cell and value of number you want to put separated with space. Example: 1 1 3" << endl;
			cout << "First number means row number, second column number and third value that will be put in cell." << endl;
			cout << "Enter 'b' to go back to menu. " << endl;
			cout << "Enter cell coordinates and number: ";
			char ch = ' ';
			int index = 0;
			bool is_non_numeric_symbol = false;
			while((ch = cin.get()) != '\n') {
				if(tolower(ch) == 'b' && cin.peek() == '\n') {
				       	cin.get();
					bd.clear();
					return;
				}	
				int value = 0;
				if(isdigit(ch)) { 
					value += (ch - '0');
					while(isdigit(ch = cin.peek())) {
						ch = cin.get();	
						value *= 10;
						value += (ch - '0');
					}		

				       		
					storage[index++] = value;
					continue;
				}
				if(ch != ' ') {
					is_non_numeric_symbol = true;
					while(cin.get() != '\n') {}
					break;
				}	
			}	

			if(is_non_numeric_symbol) {
				cout << endl << "\033[31mEnter only numeric symbols separeted by spaces.\033[0m" << endl;
				continue;
			}	
			if(index > 3) {
				cout << endl << "\033[31mEnter only 3 number as shown in the example\033[0m" << endl;
				continue;
			}	
			
			int x = storage[0];
			int y = storage[1];
			int number = storage[2];

		//	if(isdigit(x) && isdigit(y) && isdigit(number)) {
			if(x <= 0 || x > (int)bd.size() || y <= 0 || y > (int)bd[0].size()) {
				cout << endl << "\033[31mError: there's no cell on the board with such coordinates. \033[0m" << endl;
				continue;
			}
			if(number < 0 ) {
				cout << endl << "\033[31mError: value can't be negative.\033[0m" << endl;
				continue;
			}	
			if(number > (int)(bd.size() * bd[0].size())) {
				cout << endl << "\033[31mError: the board has'nt enough cells for this value.\033[0m" << endl;
				continue;
			}
		//	}	
				 
		//	else {
		//		cout << "Enter coordinates as shown in the example with numeric symbols." << endl;
		//		continue;	
		//	}	
			int is_valid = check_is_user_move_valid(Cell(Poss(x-1, y-1), number, true));
			if(is_valid == 2) {
				cout << endl << "\033[31mYou can't put cell with that number there.\033[0m" << endl;
				cout << "\033[31mIt form an area with dimensions that do not correspond to the number of this number.\033[0m" << endl;
				continue;
			}	
			if(is_valid == 3) {
				cout << endl << "\033[31mYou can't put cell with that number there.\033[0m" << endl;
				cout << "\033[31mIt blocks other cells.\033[0m" << endl;
				continue;
			}	
			single_cell_move_on_board(Cell(Poss(x-1, y-1), number, true));
			mark_area_if_ready(Cell(Poss(x-1, y-1), number, true));
			system("cls");
			break;
		}		


	}	
	print_board(); 
	cout << "The grid is solved !!!" << endl;
	bd.clear();

}	

int Board::check_is_user_move_valid(Cell cell) {
	vector<Cell> cell_area = select_area_special(cell);
	//return 3 if move blocks other cells
	if(is_move_blocks_single(cell) == true) return 3;
	//return 2 if move touches another area with same number and they form an area with dimensions that do not correspond to the number of this number
//	if(is_touch_same_number(Area(cell.number, cell_area)) == true) return 2; 
	if(cell.number > 0 && (int)cell_area.size() > cell.number) return 2;

	//return 1, if move is valid
	return 1;
}



//prints board
void Board::print_board() {
	
	cout << endl << "    ";
	for(size_t i = 0; i < bd[0].size(); i++) {
		cout << i+1 << " ";
	}
	cout << endl;
	cout << "  ┼";
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

//
vector<Area> Board::parse_areas() {
	vector<Area> areas;
	while(true) {
		vector<Cell> area;
		int is_all_cells_visited = true;
		int cells_that_not_filled = 0;
		for(size_t i = 0; i < bd.size(); i++) {
			for(size_t k = 0; k < bd[0].size(); k++) {
				if(bd[i][k].number == 0) cells_that_not_filled++;
				if(bd[i][k].is_visited == false && bd[i][k].number != 0) {
					is_all_cells_visited = false;
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
		if(is_all_cells_visited == true && cells_that_not_filled == 0) {
			for(vector<Cell> &i : bd) {
				for(Cell &j : i) {
					j.is_visited = false;
				}
			}
			areas.clear();
			continue;	
		}
		break;
	}	
	return areas;
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
	vector<Area> areas = parse_areas();
	int sum = 0;
	for(Area i : areas) {
		sum += i.number;
	}
	if((size_t)sum == (bd.size()*bd[0].size())) {
		return true;
	}	
	areas.clear();
	return false;

}

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

//function returns completed variants of moves for cell that we specified
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
		
		if(path_counter > 1200) {
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

		//			cout << "== (" << ps[k].x+1 << ", " << ps[k].y+1 << ")" << endl; 
			//		if(path_counter == 63) {
			//			for(Poss n : stack) {
			//				cout << "(" << n.x << ", " << n.y << ")" << endl;
			//			}	
			//		}	
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
		//	cout << "!=0: (" << stack.back().x+1 << ", " << stack.back().y+1 << ")" << endl; 
			for(int j = 0; j < bd[stack.back().x][stack.back().y].in_stack.blocked_count.back(); j++) {
				bd[bd[stack.back().x][stack.back().y].in_stack.blocked.back().x][bd[stack.back().x][stack.back().y].in_stack.blocked.back().y].is_visited = false;
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
	/*		
		if(k == 4 && went_back.size() != 0 && (stack.back().x != went_back.back().x || stack.back().y != went_back.back().y)) {
		//	cout << went_back.size() << endl;
			cout << "went_back != 0: (" << stack.back().x+1 << ", " << stack.back().y+1 << ")" << endl; 
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
	*/	
		if(k == 4 && (bd[stack.back().x][stack.back().y].in_stack.went_back_near == 0 /*|| status == true*/)) {
		//	for(Poss j : bd[stack.back().x][stack.back().y].in_stack.blocked) {
		//		bd[j.x][j.y].is_visited = false;
		//	}	
		//	
	//		cout << "==0: (" << stack.back().x+1 << ", " << stack.back().y+1 << "),   " << went_back.size() << endl; 
		//	cout << "ww: (" << went_back.back().x+1 << ", " << went_back.back().y+1 << "),   " << went_back.size() << endl; 
			for(int j = 0; j < bd[stack.back().x][stack.back().y].in_stack.blocked_count.back(); j++) {
				bd[bd[stack.back().x][stack.back().y].in_stack.blocked.back().x][bd[stack.back().x][stack.back().y].in_stack.blocked.back().y].is_visited = false;
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


void Board::print_possible_moves(Cell cell) {
	vector<Area> moves = get_possible_moves(cell);
	int counter = 0;
	cout << moves.size() << endl;
	for(Area temp : moves) {
		if(is_touch_same_number(temp) == true) continue;
		for(Cell k : temp.cells) {
			cout << "[" << counter << "]: " << "poss(" << k.coord.x+1 << ", " << k.coord.y+1 << ")" << endl;  	
			bd[k.coord.x][k.coord.y].number = moves[0].number;
		}	
		print_board();	
		for(Cell k : temp.cells) {
			if(k.is_visited == false) bd[k.coord.x][k.coord.y].number = 0;
		}	
//		}
		counter++;
	}		



}	


//function that making moves base on algorithm
bool Board::make_move(Cell cell) {
	vector<Area> moves = get_possible_moves(cell);
//	cout << "Number [" << moves[0].number << "]: " << moves.size() << endl; 

	
//	int cnt = 0;
//	for(Area i : moves) {
//		for(Cell k : i.cells) {
//			cout << cnt << " [" << k.coord.x+1 << ", " << k.coord.y+1 << "]" << endl;
//		}
//		cnt++;	
//	}	
	if(((int)moves.size() == 1 || (int)moves.size() == 2) && is_touch_same_number(moves[0]) == false) {
		bool is_moves_identical = true;
		if((int)moves.size() == 2) {
		       	if(check_is_moves_identical(moves[0], moves[1]) == false) is_moves_identical = false; 
		}
		if(is_moves_identical) {	
			move_on_board(moves[0]);
			
			return true;	
		}		
	}
	/*
	if((int)moves.size() == 1 && is_touch_same_number(moves[0]) == false) {
		bool hit = false;
		for(Cell i : moves[0].cells) {
			if(is_move_blocks_single(i) == true) hit = true;
		}

		if(hit == false) {
		       	move_on_board(moves[0]);
			return;
		}	
	}	
	*/
 	/*	
	vector<Area> solid_moves;
	for(Area i : moves) {
		if(is_touch_same_number(i) == false) {
			solid_moves.push_back(i);
		}	
	}
	if(solid_moves.size() == 1) {
		move_on_board(solid_moves[0]);
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
//	if(not_blocked_moves_counter == 1 && is_touch_same_number(not_blocked_move[0]) == false) {
//		move_on_board(not_blocked_move[0]);
//		return;
//	}
	*/
	vector<Cell> possible_moves = check_single_cell_moves(cell);
	if((int)possible_moves.size() == 1 && is_move_blocks_single(possible_moves[0]) == false) {
		single_cell_move_on_board(possible_moves[0]);	
		mark_area_if_ready(cell);	
		return true;
	}
	


	vector<Cell> filt_moves = filter_moves_single(possible_moves);	
	if((int)filt_moves.size() == 1) {
	       	single_cell_move_on_board(filt_moves[0]);
		mark_area_if_ready(cell);	
		return true;
	}	
	

//	vector<Area> not_blocked_move;
//	for(Area i : moves) {
//		if(check_is_any_cells_blocked(i) == false && is_touch_same_number(i) == false) {
//			not_blocked_move.push_back(i);
//		}
//	}

	/*
		
	vector<Area> mvs;
//	vector<int> free_space;
	vector<int> number_pierced;
	for(Area i : moves) {
		if(is_piercies(i) >= 2 && is_touch_same_number(i) == false) {
			//cout << "Block >= 2: " << moves[0].number << endl;
			mvs.push_back(i);
			number_pierced.push_back(is_piercies(i));
		//	free_space.push_back(check_straightness(i));
		//	if(check_is_any_cells_blocked_deep(i) == false) {
			     //  	move_on_board(i);

		//		return;
		//	}	
		}	
	}
	print_moves(mvs);
	*/
	/*	
	if(mvs.size() == 0) return;
	int temp = 0;
	Area move;	
	for(size_t i = 0; i < mvs.size(); i++) {
		if(free_space[i] > temp) {
			temp = free_space[i];
			move = mvs[i];
		}	
	}
	*/
//	cout << temp << "," << move.number << endl;
	/*
	if(mvs.size() == 0) return;
	int min = 0;
	Area move;
	for(int i = 0; i < (int)number_pierced.size(); i++) {
		if(min < number_pierced[i]) {
			move = mvs[i];
			min = number_pierced[i];
		}	
	}	
	move_on_board(move);
	*/
		
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
	return false;	
	
}

void Board::print_moves(vector<Area> moves) {
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
			if(k.is_visited == false) bd[k.coord.x][k.coord.y].number = 0;
		}	
//		}
		counter++;
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
	for(Cell i : move.cells) {
		if(i.is_visited == true) {
			bd[i.coord.x][i.coord.y].is_visited = false;
			continue;
		}	
		bd[i.coord.x][i.coord.y].number = 0;
		bd[i.coord.x][i.coord.y].is_visited = false;
	}	
}	

void Board::solve(bool show_steps) {
	//just fo print_board() when user input already solved board.
	while(!is_board_solved()) {
//	for(size_t j = 0; j < 11; j++) {
		bool is_main_cells_filled = true;
		int  moves_was_made = 0;
		for(size_t i = 0; i < bd.size(); i++) {
			for(size_t k = 0; k < bd[0].size(); k++) {
				if(bd[i][k].number != 0 && bd[i][k].is_visited == false) {
					is_main_cells_filled = false;
					if(make_move(Cell(Poss(i, k), bd[i][k].number, true))){
					       	moves_was_made++;
						if(show_steps) print_board();	
					}
				//	show_visited_cells();
				}
			}		
		}
		if(is_main_cells_filled == true) {
			fill_free_space(); 
		}
		if(moves_was_made == 0 && is_main_cells_filled == false) {
			print_board();	
			cout << "I could'nt solve your grid :( " << endl;
			bd.clear();
			return;

		}	
		
	}	
	print_board();
	cout << "The grid is solved !!!" << endl;
	bd.clear();
	//	parse_areas();
//	}	
//
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
	cout << endl << "    ";
	for(size_t i = 0; i < bd[0].size(); i++) {
		cout << i+1 << " ";
	}
	cout << endl;
	cout << "  ┼";
	for(size_t i = 0; i < 2*bd[0].size(); i++) {
		cout << "─";
	}
	cout << endl;
	for(size_t i = 0; i < bd.size(); i++) {
		if(i+1 == 10) cout << i+1 << "│ ";
		else { cout << i+1 << " │ "; }

		for(size_t a = 0; a < bd[0].size(); a++) {
			if(bd[i][a].is_visited == true) cout << -1 << " ";
			else { cout << bd[i][a].number << " "; } 
		}
		cout << endl;
	}
	cout << endl;

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


/*
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
*/


int Board::check_free_space(Area move) {
	int free_space = 0;
	vector<Poss> visited;
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
					if(bd[ps[k].x][ps[k].y].is_visited == false)  {
						free_space++;
						bd[ps[k].x][ps[k].y].is_visited = true;
						visited.push_back(Poss(ps[k].x, ps[k].y));
					}	
				}	
				else{
					if(bd[ps[k].x][ps[k].y].is_visited == false)  {
						free_space++;
						bd[ps[k].x][ps[k].y].is_visited = true;
						visited.push_back(Poss(ps[k].x, ps[k].y));
					}	
				}	
			}	
			else {
				free_space++;
			}	
		}	
	}	
	
	for(Poss i : visited) bd[i.x][i.y].is_visited = false;
	

	return free_space;
}	

int Board::check_straightness(Area move) {
	int count_x = 0;
	int count_y = 0;
	Cell mv = move.cells.back();
	while(move.cells.size() > 0) {	
		move.cells.pop_back();
		for(size_t k = 0; k < move.cells.size(); k++) {
			if(mv.coord.x == move.cells[k].coord.x) count_x++;
			if(mv.coord.x == move.cells[k].coord.y) count_y++;	
		}
		if(move.cells.size() > 0) mv = move.cells.back();
	}	
	return ((count_x - count_y) ? count_x - count_y : (count_x-count_y)*-1);
}	


vector<Cell> Board::check_single_cell_moves(Cell cell, bool status) {
	vector<Cell> possible_moves;
	vector<Cell> counted_cells;
	vector<Cell> area = select_area_special(cell);
	if(status == true) {
	if((int)area.size() == cell.number) {
		for(Cell i : area) {
			if(bd[i.coord.x][i.coord.y].is_on_going == true) {
				bd[i.coord.x][i.coord.y].is_on_going = false;
			}	
			if(bd[i.coord.x][i.coord.y].is_visited == false) {

				bd[i.coord.x][i.coord.y].is_visited = true;
			}	
		}	
		return possible_moves;
	}
	}	
	for(Cell i : area) { 
		Poss ps[4] = { Poss(i.coord.x-1, i.coord.y), Poss(i.coord.x+1, i.coord.y), Poss(i.coord.x, i.coord.y-1), Poss(i.coord.x, i.coord.y+1) };
		for(int k = 0; k < 4; k++) {
			if(ps[k].x >= 0 && (size_t)ps[k].x < bd.size() && ps[k].y >= 0 && (size_t)ps[k].y < bd[0].size()) {
				if(bd[ps[k].x][ps[k].y].number == 0 && bd[ps[k].x][ps[k].y].is_counted == false) {
					if(is_move_blocks_single(Cell(Poss(ps[k].x, ps[k].y), cell.number, false)) == true) {
						continue;
					}	
					int cells_in_area = 0;
					cells_in_area = is_touch_same_number_single(Cell(Poss(ps[k].x, ps[k].y), cell.number, false), area);
					if(cells_in_area != 0) {
						if((int)area.size()+cells_in_area >= cell.number) continue;	
					}	
					possible_moves.push_back(Cell(Poss(ps[k].x, ps[k].y), cell.number, false));
					bd[ps[k].x][ps[k].y].is_counted = true;
					counted_cells.push_back(Cell(Poss(ps[k].x, ps[k].y), cell.number, false));
				}		
			}
		
		}		
	}
	for(Cell i : counted_cells) {
		bd[i.coord.x][i.coord.y].is_counted = false;
	}	
	return possible_moves;
}	

bool Board::is_move_blocks_single(Cell cell) {
	single_cell_move_on_board(cell);
	for(Cell i : get_neighbours(cell)) {
		if(i.number > 1 && bd[i.coord.x][i.coord.y].is_visited == false && (int)get_possible_moves(i).size() == 0) {
			single_cell_move_on_board(cell, false);
		       	return true;
		}	
		if(i.number == 0) {
			vector<Cell> space = select_area_special(i);
			if(space.size() < 10) {
				for(Cell j : space) {
					for(Cell h : get_neighbours(j)) {
						if(i.number > 1 && bd[i.coord.x][i.coord.y].is_visited == false && (int)get_possible_moves(i).size() == 0) {
							single_cell_move_on_board(cell, false);
		 				      	return true;
						}	
					}		
				}
			}	
		}	
	}
	single_cell_move_on_board(cell, false);
	return false;
}	

/*
bool Board::is_move_blocks_single(Cell cell) {
	single_cell_move_on_board(cell);
	for(Cell i : get_neighbours(cell)) {
		if(i.number > 1 && bd[i.coord.x][i.coord.y].is_visited == false) {
			vector<Area> moves;
			for(Area l : get_possible_moves_part(i)) {
				if(is_touch_same_number(l) == false) moves.push_back(l); 
			}	
			if(moves.size() == 0) {
				single_cell_move_on_board(cell, false);
		       		return true;
			}
		}	
	}
	single_cell_move_on_board(cell, false);
	return false;
}	
*/

/*
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
*/


vector<Cell> Board::select_area_special(Cell cell) {
	vector<Cell> stack;
	vector<Cell> area;
	stack.push_back(cell);
	bd[stack.back().coord.x][stack.back().coord.y].is_counted = true;
	area.push_back(bd[stack.back().coord.x][stack.back().coord.y]);
	while(stack.size() > 0) {
		Poss ps[4] = {Poss(stack.back().coord.x-1, stack.back().coord.y), Poss(stack.back().coord.x+1, stack.back().coord.y), Poss(stack.back().coord.x, stack.back().coord.y-1), Poss(stack.back().coord.x, stack.back().coord.y+1) };
		
		int i = 0;
		for(i = 0; i < 4; i++) {
			if(ps[i].x < 0 || (size_t)ps[i].x >= bd.size() || ps[i].y < 0 || (size_t)ps[i].y >= bd[0].size()) continue;
			if(bd[ps[i].x][ps[i].y].number == cell.number && bd[ps[i].x][ps[i].y].is_counted == false) {
				bd[ps[i].x][ps[i].y].is_counted = true;
				area.push_back(bd[ps[i].x][ps[i].y]);
				stack.push_back(bd[ps[i].x][ps[i].y]);
				break;	
			}
		}
       		if(i == 4) stack.pop_back();	
	}
	for(Cell i : area) {
		bd[i.coord.x][i.coord.y].is_counted = false;
	}	
	return area;
}

void Board::single_cell_move_on_board(Cell move, bool status) {
	//Move on board
	if(status == true) {
		bd[move.coord.x][move.coord.y].number = move.number;
		bd[move.coord.x][move.coord.y].is_on_going = true;
		return;
	}
	//Remove move on boad
	bd[move.coord.x][move.coord.y].number = 0;
	bd[move.coord.x][move.coord.y].is_on_going = false;
}

int Board::is_touch_same_number_single(Cell cell, vector<Cell> current_cell_area) {
		Poss ps[4] = { Poss(cell.coord.x-1, cell.coord.y), Poss(cell.coord.x+1, cell.coord.y), Poss(cell.coord.x, cell.coord.y-1), Poss(cell.coord.x, cell.coord.y+1) };
		for(int k = 0; k < 4; k++) {
			vector<Cell> temp_area;
			if(ps[k].x >= 0 && (size_t)ps[k].x < bd.size() && ps[k].y >= 0 && (size_t)ps[k].y < bd[0].size()) {
				if(bd[ps[k].x][ps[k].y].number == cell.number) {
					bool status = false;
					for(Cell j : current_cell_area) {
						if(ps[k].x == j.coord.x && ps[k].y == j.coord.y) {
							status = true;	
							break;
						}	
					}
					if(status == false) {
						temp_area = select_area_special(Cell(Poss(ps[k].x, ps[k].y), cell.number, false));
						return (int)temp_area.size();
					}	
				}	
			}	
		}	
	return 0;
}


vector<Cell> Board::filter_moves_single(vector<Cell> moves) {
	vector<Cell> filt_moves;
	for(Cell i : moves) {
		single_cell_move_on_board(i);
		vector<Cell> neighb = get_neighbours(i);
			
		bool temp = false;
		for(Cell k : neighb) {
			if(k.number == 0 || k.number == 1) temp = true;
		}
		vector<Cell> move_area = select_area_special(i);
		if(temp == false && (int)move_area.size() != i.number) {
			for(Cell k : move_area) {
				for(Cell h : get_neighbours(k)) {
					if(h.number == 0) temp = true;	
				}
			}	
			if(temp	== true && moves.size() == 2) {
				for(Cell l : moves) {
					if(l == i) continue; 
					filt_moves.push_back(l);
				}	

				single_cell_move_on_board(i, false);
			//	cout << "1" << endl;
				return filt_moves; 
			}	
		}	
		bool hit_glob = false;
		bool clear_move_found = false;	
		for(Cell k : neighb) {
			bool hit = false;
			if(k.number == 1) {
				vector<Cell> able_to_move; 
				for(Cell h : neighb) {
					if(h.number != i.number && bd[h.coord.x][h.coord.y].is_visited == false) {
						able_to_move.push_back(h);
					}	
				}	
				if(able_to_move.size() == 0) {
					clear_move_found = true;
			//		cout << "2" << endl;
					break;
				}	
			}		
			if(k.number == 0) {
				int count_same_num = 0;
				vector<Cell> free_space = select_area_special(k);	
				vector<Cell> fneighb;
				for(Cell n : free_space) {
					for(Cell m : get_neighbours(n)) {
						if(m.number != 0) fneighb.push_back(m);
					}		
				}	
				vector<Cell> able_to_move;
				for(Cell n : fneighb) {
					vector<Cell> area = select_area_special(n);
					if((int)area.size() != n.number) {
						if(n.number == i.number) count_same_num += (int)area.size(); 
						bool the_same_cell = false;	
					       	for(Cell h : able_to_move) if(n.coord.x == h.coord.x && n.coord.y == h.coord.y) the_same_cell = true;
						if(the_same_cell == true) continue;
						
						able_to_move.push_back(n);

					}	
				}
				
				if(able_to_move.size() == 0) {
					for(Cell n : fneighb) {
						if(n.number == (int)free_space.size()) {
							hit = true;
							hit_glob = true;
						//	cout << "3" << endl;

							break;
						}
					}
				}
				
			//	if(able_to_move.size() > 1) print_board();
				if(hit == true) break;
				if(able_to_move.size() == 1 && (int)(select_area_special(able_to_move[0]).size() + free_space.size()) == able_to_move[0].number) {
					hit = true;	
					clear_move_found = true;

				//	cout << "4" << endl;
				}
				if(hit == true) break;		


				//

				/*
				single_cell_move_on_board(i, false);
				Cell actual_number_cell;
				for(Cell l : get_neighbours(i)) {
					if(l.number == i.number) actual_number_cell = l;
				}	
				vector<Cell> actual_number_area = select_area_special(l);
				for(

				*/
				//		

				for(Cell l : fneighb) {
					if((int)free_space.size() == l.number) {
						int able_ways = 0;
					
						for(Cell w : get_neighbours(i)) {
							if(w.number == 0) able_ways++; 
						}

						if(able_ways > 1) break;	
						vector<Cell> filt_moves_t = filter_moves_single(check_single_cell_moves(i, false));
						if((int)filt_moves_t.size() == 1) {
							hit = true;
							clear_move_found = true;
					//		cout << "5" << endl;
						}	
					}	
				}

				//
				//
				//


				//
				//
				//

				if(hit == true) break;		
					
				
			}
		}
		single_cell_move_on_board(i, false);
		if(hit_glob == true) continue; 
		if(clear_move_found == true) filt_moves.clear();
		filt_moves.push_back(i);
		if(clear_move_found == true) break;
			
			
	}	
	vector<Cell> filt_moves_final;
	for(Cell i : filt_moves) {
		if(is_move_blocks_single(i) == false) filt_moves_final.push_back(i);
	}	
	return filt_moves_final;
//	return filt_moves;
}	



vector<Cell> Board::get_neighbours(Cell cell) {
	vector<Cell> neighbours;	
	Poss ps[4] = { Poss(cell.coord.x-1, cell.coord.y), Poss(cell.coord.x+1, cell.coord.y), Poss(cell.coord.x, cell.coord.y-1), Poss(cell.coord.x, cell.coord.y+1) };
	for(int k = 0; k < 4; k++) {
		if(ps[k].x >= 0 && (size_t)ps[k].x < bd.size() && ps[k].y >= 0 && (size_t)ps[k].y < bd[0].size()) {
			neighbours.push_back(Cell(Poss(ps[k].x, ps[k].y), bd[ps[k].x][ps[k].y].number, bd[ps[k].x][ps[k].y].is_visited));
				
		}				
	}	
			
	return neighbours;	
}	


vector<Area> Board::get_possible_moves_part(Cell cell) {
	vector<Cell> current_cells = select_area_special(cell);
	int cell_shortage = cell.number - (int)current_cells.size();
	bd[cell.coord.x][cell.coord.y].number = cell_shortage+1;
	vector<Area> temp = get_possible_moves(Cell(Poss(cell.coord.x, cell.coord.y), cell_shortage+1, true));
	bd[cell.coord.x][cell.coord.y].number = cell.number;
	vector<Area> moves;	
	int h = 0;
       	for(Area i : temp) {
		moves.push_back(Area(cell.number));
		for(Cell j : current_cells) {
			moves[h].cells.push_back(j);		
		}		
		int skip = 0;
		for(Cell j : i.cells) {
			if(skip++ == 0) continue;
			moves[h].cells.push_back(Cell(Poss(j.coord.x, j.coord.y), cell.number, j.is_visited));
		}
		h++;	
	}		
	return moves;
}	



void Board::mark_area_if_ready(Cell cell) {
	vector<Cell> area = select_area_special(cell);
	if((int)area.size() == cell.number) {
		for(Cell i : area) {
			if(bd[i.coord.x][i.coord.y].is_visited == false) bd[i.coord.x][i.coord.y].is_visited = true;
		}	
	}	
}	



void Board::fill_free_space() {
	for(size_t i = 0; i < bd.size(); i++) {
		for(size_t j = 0; j < bd[0].size(); j++) {
			if(bd[i][j].number == 0) {
				fill_area(select_area_special(Cell(Poss(i, j), 0, false))); 
			}	
		}	
	}
}


void Board::fill_area(vector<Cell> area) {
	vector<vector<int>> partitions = partition_number((int)area.size());
	for(vector<int> partition : partitions) {
		for(int number : partition) { 
			bd[area.back().coord.x][area.back().coord.y].number = number;
			vector<Area> moves = get_possible_moves(Cell(Poss(area.back().coord.x, area.back().coord.y), number, true));
			for(Area move : moves) {
				if(is_touch_same_number(move) == false) {
					move_on_board(move);
					return;
				}	
			}
		}
	}	
				


}	

void Board::find_combinations(int n, int max_num, vector<int> &current, vector<vector<int>> &result) {
	if(n == 0) {
		result.push_back(current);
		return;
	}

	for(int i = min(max_num, n); i >= 1; --i) {
		current.push_back(i);
		find_combinations(n-i, i, current, result);
		current.pop_back();
	}	
}

vector<vector<int>> Board::partition_number(int number) {
	vector<vector<int>> result;
	vector<int> current;

	if(number <= 0) {
		return result;
	}
	
	find_combinations(number, number, current, result);
	return result;	
}	

bool Board::check_is_moves_identical(Area move1, Area move2) {
	int coincidence = 0;
	vector<Poss> used_cells;
	for(Cell i : move1.cells) {
		for(Cell k : move2.cells) {
			if(bd[k.coord.x][k.coord.y].is_counted == true) continue;
			if(i == k) {
				coincidence++;
				bd[k.coord.x][k.coord.y].is_counted = true;
				used_cells.push_back(Poss(k.coord.x, k.coord.y));
			}	
		}	
	}
	for(Poss i : used_cells) {
		bd[i.x][i.y].is_counted = false;
	}		
	if(coincidence == (int)move1.cells.size()) return true;
	return false;
}	

