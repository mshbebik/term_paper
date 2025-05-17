#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <algorithm> // for swap() and min() functions
#include <cstdlib> // for system("cls")		     
#include <ctype.h> // for isdigit() and tolower()		  
#include "board.h"
#define LOCK 1200
#define MIN_PATHS 10
using namespace std;



Board::Board() {

}	

//initialize board from file constructor
Board::Board(string file_name) {
	init_from_file(file_name);
}

//initialize board from user input
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
				bd[row].push_back(Cell(Pos(row, column), number));	
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
	return true;
}	

//cheks is every row in vectoc<vector<Cell>> bd has same size
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


//initialize board from file
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
			bd[i].push_back(Cell(Pos(i, k), nums[i][k]));	
		}
	} 
}

//parsing file and returning finished board for further initialization
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

//solve board by user fucntion, user solving board by typing coordinates of cell on the board
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

			int is_valid = check_is_user_move_valid(Cell(Pos(x-1, y-1), number, true));
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
			single_cell_move_on_board(Cell(Pos(x-1, y-1), number, true));
			mark_area_if_ready(Cell(Pos(x-1, y-1), number, true));
			system("cls");
			break;
		}		


	}	
	print_board(); 
	cout << "The grid is solved !!!" << endl;
	bd.clear();

}	

//checks user input by 2 conditions: is move blocks other cells and is move touches another area 
int Board::check_is_user_move_valid(Cell cell) {
	vector<Cell> cell_area = select_area_special(cell);
	//return 3 if move blocks other cells
	if(is_move_blocks_single(cell) == true) return 3;
	//return 2 if move touches another area with same number and they form an area with dimensions that do not correspond to the number of this number
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

//parses board by already completed areas of numbers
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
		//case when board has all cells visited and full completed
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

//selects an area by the passed cell 
void Board::select_area(Cell cell, Pos start, vector<Cell> &area) {
	Pos ps[5] = { start, Pos(start.x-1, start.y), Pos(start.x+1, start.y), Pos(start.x, start.y-1), Pos(start.x, start.y+1) };
	for(int i = 0; i < 5; i++) {
		if(ps[i].x < 0 || (size_t)ps[i].x >= bd.size() || ps[i].y < 0 || (size_t)ps[i].y >= bd[0].size()) continue;
		if(bd[ps[i].x][ps[i].y].number == cell.number && bd[ps[i].x][ps[i].y].is_visited == false) {
			bd[ps[i].x][ps[i].y].is_visited = true;
			area.push_back(bd[ps[i].x][ps[i].y]);
			select_area(cell, Pos(ps[i].x, ps[i].y), area);
		}
	} 
}


//cheks is board solved
bool Board::is_board_solved() {
	//parses complited areas on board
	vector<Area> areas = parse_areas();
	int sum = 0;
	//suming all numbers of complited areas
	for(Area i : areas) {
		sum += i.number;
	}
	// If the sum of the completed areas' numbers equals the height of the board multiplied by its width, then the board is solved
	if((size_t)sum == (bd.size()*bd[0].size())) {
		return true;
	}	
	areas.clear();
	return false;

}

//checks possible moves (completed areas) 
//function retuns number of moves found, usually uses as to check is cell has possible moves
int Board::check_possible_moves(Cell cell) {
	vector<Pos> stack;
	vector<Pos> used_cells;
	int path_counter = 0;
	int cell_counter = 1;
	stack.push_back(cell.coord);
	bd[cell.coord.x][cell.coord.y].is_in_stack = true;	

		
	while(stack.size() > 0) {
		
		// as this function serves as an indicator for valid cell moves. I optimized it by locking the search process once it finds 10 valid paths, at which point it stops.
		if(path_counter > MIN_PATHS) {
			for(Pos l : stack) {
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
		
		//gets coord of neighbour cells
		Pos ps[4] = { Pos(stack[stack.size()-1].x-1, stack[stack.size()-1].y), Pos(stack[stack.size()-1].x+1, stack[stack.size()-1].y),
		       	       Pos(stack[stack.size()-1].x, stack[stack.size()-1].y-1), Pos(stack[stack.size()-1].x, stack[stack.size()-1].y+1) };

		int k;
		for(k = 0; k < 4; k++) {
			if(ps[k].x < 0 || (size_t)ps[k].x >= bd.size() || ps[k].y < 0 || (size_t)ps[k].y >= bd[0].size()) continue; 
			if((bd[ps[k].x][ps[k].y].number == 0 || bd[ps[k].x][ps[k].y].number == cell.number) && bd[ps[k].x][ps[k].y].is_visited == false) {
				//case when cell went to the already cells that in the stack
				if(bd[ps[k].x][ps[k].y].is_in_stack == true) {
					bd[stack.back().x][stack.back().y].is_visited = true;
					bd[stack.back().x][stack.back().y].in_stack.is_went_back = true;
					bd[ps[k].x][ps[k].y].in_stack.blocked.push_back(stack.back());
					bd[ps[k].x][ps[k].y].in_stack.blocked_count.push_back(1);
			       		stack.push_back(Pos(ps[k].x, ps[k].y));
					bd[stack.back().x][stack.back().y].in_stack.went_back_near++;
					break;
					
				}	
				//default case
			       	stack.push_back(Pos(ps[k].x, ps[k].y));
				cell_counter++;
								
				bd[ps[k].x][ps[k].y].is_in_stack = true;
				//default case

				//case path is found
				if(cell_counter == cell.number) {

					cell_counter--;
					path_counter++;
					bd[ps[k].x][ps[k].y].is_visited = true;
					stack.pop_back();
					bd[ps[k].x][ps[k].y].is_in_stack = false;
					bd[stack.back().x][stack.back().y].in_stack.blocked.push_back(Pos(ps[k].x, ps[k].y));
					bd[stack.back().x][stack.back().y].in_stack.blocked_count.back()++;
					continue;
				}
				break;
			}	

		}
		//If all neighboring cells have been visited and the previous cell in the stack went back to the current cell, we must backtrack to the previous cell.
		//So we need to pop current cell out of stack and make it visited so previous cell will not go to this cell and make loop
		if(k == 4 && bd[stack.back().x][stack.back().y].in_stack.went_back_near != 0) {
			//making unvisited all cells that was visited during moves from current cell 
			for(int j = 0; j < bd[stack.back().x][stack.back().y].in_stack.blocked_count.back(); j++) {
				bd[bd[stack.back().x][stack.back().y].in_stack.blocked.back().x][bd[stack.back().x][stack.back().y].in_stack.blocked.back().y].is_visited = false;
				bd[stack.back().x][stack.back().y].in_stack.blocked.pop_back();
			}	
			bd[stack.back().x][stack.back().y].in_stack.blocked_count.pop_back();
			bd[stack.back().x][stack.back().y].in_stack.went_back_near--;
			if(bd[stack.back().x][stack.back().y].in_stack.is_went_back == true) bd[stack.back().x][stack.back().y].in_stack.is_went_back = false;
			bd[stack[stack.size()-2].x][stack[stack.size()-2].y].in_stack.blocked.push_back(stack.back());	
			bd[stack[stack.size()-2].x][stack[stack.size()-2].y].in_stack.blocked_count.back()++;	
			bd[stack.back().x][stack.back().y].is_visited = true;	
			stack.pop_back();
			continue;
		}	
		//If all neighboring cells have been visited and current cell has no  previous cell in the stack that went back to the current cell.
		//In this case we need to pop out of stack and marked as this cell is not in stack anymore, and make it visited so previous cell will not go to this cell and make loop 
		if(k == 4 && bd[stack.back().x][stack.back().y].in_stack.went_back_near == 0) {
			//making unvisited all cells that was visited during moves from current cell 
			for(int j = 0; j < bd[stack.back().x][stack.back().y].in_stack.blocked_count.back(); j++) {
				bd[bd[stack.back().x][stack.back().y].in_stack.blocked.back().x][bd[stack.back().x][stack.back().y].in_stack.blocked.back().y].is_visited = false;
				bd[stack.back().x][stack.back().y].in_stack.blocked.pop_back();
			}	
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
	vector<Pos> stack;
	vector<Pos> used_cells;
	int path_counter = 0;
	int cell_counter = 1;
	moves.push_back(Area(cell.number));
	moves[path_counter].cells.push_back(cell);
	stack.push_back(cell.coord);
	bd[cell.coord.x][cell.coord.y].is_in_stack = true;	

		
	while(stack.size() > 0) {
		//lock for finding, if pathes that found is more than lock value we exit from func returning already found moves
		if(path_counter > LOCK) {
			for(Pos l : stack) {
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
			
		//gets coord of neighbour cells
		Pos ps[4] = { Pos(stack[stack.size()-1].x-1, stack[stack.size()-1].y), Pos(stack[stack.size()-1].x+1, stack[stack.size()-1].y),
		       	       Pos(stack[stack.size()-1].x, stack[stack.size()-1].y-1), Pos(stack[stack.size()-1].x, stack[stack.size()-1].y+1) };

		
		int k;
		for(k = 0; k < 4; k++) {
			if(ps[k].x < 0 || (size_t)ps[k].x >= bd.size() || ps[k].y < 0 || (size_t)ps[k].y >= bd[0].size()) continue; 
			if((bd[ps[k].x][ps[k].y].number == 0 || bd[ps[k].x][ps[k].y].number == cell.number) && bd[ps[k].x][ps[k].y].is_visited == false) {
				//case when cell went to the already cells that in the stack
				if(bd[ps[k].x][ps[k].y].is_in_stack == true) {
					bd[stack.back().x][stack.back().y].is_visited = true;
					bd[stack.back().x][stack.back().y].in_stack.is_went_back = true;
					bd[ps[k].x][ps[k].y].in_stack.blocked.push_back(stack.back());
					bd[ps[k].x][ps[k].y].in_stack.blocked_count.push_back(1);
			       		stack.push_back(Pos(ps[k].x, ps[k].y));
					bd[stack.back().x][stack.back().y].in_stack.went_back_near++;
					break;
					
				}	
				//default
			       	stack.push_back(Pos(ps[k].x, ps[k].y));
				cell_counter++;
				
				//checks whether this cell has already had a value other than zero, -
				//in which case we mark it by creating an object of the Cell class with the is_visited field true, -
				//so that when testing moves, we do not accidentally erase this cell from the board 
				if(bd[ps[k].x][ps[k].y].number == cell.number) { 
					moves[path_counter].cells.push_back(Cell(Pos(ps[k].x, ps[k].y), cell.number, true));
				}
				else{
					moves[path_counter].cells.push_back(Cell(Pos(ps[k].x, ps[k].y), cell.number, false));
				}	

				bd[ps[k].x][ps[k].y].is_in_stack = true;
				//default
				
				//case path is found
				if(cell_counter == cell.number) {
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
					bd[stack.back().x][stack.back().y].in_stack.blocked.push_back(Pos(ps[k].x, ps[k].y));
					bd[stack.back().x][stack.back().y].in_stack.blocked_count.back()++;
					continue;
				}
				break;
			}	


		}
		//If all neighboring cells have been visited and the previous cell in the stack went back to the current cell, we must backtrack to the previous cell.
		//So we need to pop current cell out of stack and make it visited so previous cell will not go to this cell and make loop
		if(k == 4 && bd[stack.back().x][stack.back().y].in_stack.went_back_near != 0) {
			//making unvisited all cells that was visited during moves from current cell 
			for(int j = 0; j < bd[stack.back().x][stack.back().y].in_stack.blocked_count.back(); j++) {
				bd[bd[stack.back().x][stack.back().y].in_stack.blocked.back().x][bd[stack.back().x][stack.back().y].in_stack.blocked.back().y].is_visited = false;
				bd[stack.back().x][stack.back().y].in_stack.blocked.pop_back();
			}	
			bd[stack.back().x][stack.back().y].in_stack.blocked_count.pop_back();
			bd[stack.back().x][stack.back().y].in_stack.went_back_near--;
			if(bd[stack.back().x][stack.back().y].in_stack.is_went_back == true) bd[stack.back().x][stack.back().y].in_stack.is_went_back = false;
			bd[stack[stack.size()-2].x][stack[stack.size()-2].y].in_stack.blocked.push_back(stack.back());	
			bd[stack[stack.size()-2].x][stack[stack.size()-2].y].in_stack.blocked_count.back()++;	
			bd[stack.back().x][stack.back().y].is_visited = true;	
			stack.pop_back();
			continue;
		}
		//If all neighboring cells have been visited and current cell has no  previous cell in the stack that went back to the current cell.
		//In this case we need to pop out of stack and marked as this cell is not in stack anymore, and make it visited so previous cell will not go to this cell and make loop 
		if(k == 4 && (bd[stack.back().x][stack.back().y].in_stack.went_back_near == 0)) {
			//making unvisited all cells that was visited during moves from current cell 
			for(int j = 0; j < bd[stack.back().x][stack.back().y].in_stack.blocked_count.back(); j++) {
				bd[bd[stack.back().x][stack.back().y].in_stack.blocked.back().x][bd[stack.back().x][stack.back().y].in_stack.blocked.back().y].is_visited = false;
				bd[stack.back().x][stack.back().y].in_stack.blocked.pop_back();
			}	
			bd[stack.back().x][stack.back().y].in_stack.blocked_count.back() = 0;
			bd[stack.back().x][stack.back().y].in_stack.is_went_back = false;
			if(stack.size() > 1) bd[stack[stack.size()-2].x][stack[stack.size()-2].y].in_stack.blocked.push_back(stack.back());	
			if(stack.size() > 1) bd[stack[stack.size()-2].x][stack[stack.size()-2].y].in_stack.blocked_count.back()++;	
			bd[stack.back().x][stack.back().y].is_in_stack = false;
			moves[path_counter].cells.pop_back();
			cell_counter--;
			bd[stack.back().x][stack.back().y].is_visited = true;
			stack.pop_back();
			
		}	

	}
	if(moves.size() > 0) moves.pop_back();	
	bd[cell.coord.x][cell.coord.y].is_visited = false;
	return moves;
}		

//this is auxiliary function for testing
void Board::print_possible_moves(Cell cell) {
	vector<Area> moves = get_possible_moves(cell);
	int counter = 0;
	cout << moves.size() << endl;
	for(Area temp : moves) {
		if(is_touch_same_number(temp) == true) continue;
		for(Cell k : temp.cells) {
			cout << "[" << counter << "]: " << "pos(" << k.coord.x+1 << ", " << k.coord.y+1 << ")" << endl;  	
			bd[k.coord.x][k.coord.y].number = moves[0].number;
		}	
		print_board();	
		for(Cell k : temp.cells) {
			if(k.is_visited == false) bd[k.coord.x][k.coord.y].number = 0;
		}	
		counter++;
	}		



}	


//function that making moves base on algorithm
bool Board::make_move(Cell cell) {
	//gets every possible move for current cell (moves that is areas)
	vector<Area> moves = get_possible_moves(cell);
	//if we have 1 possible move for cell we must invoke move_on_board() to make this move
	//if we have 2 possible moves for cell we need to check if this two moves making the same area
	//Example:
	//There are two identical 6 number areas, but the order cells was entered could be different, so we -
	// - have to check is area(shape) identical in those 2 moves
	//
	//0 0 0 0              0 0 0 0
	//0 6 6 0              0 6 6 0
	//0 6 6 0              0 6 6 0
	//0 6 6 0              0 6 6 0
	//
	if(((int)moves.size() == 1 || (int)moves.size() == 2) && is_touch_same_number(moves[0]) == false) {
		bool is_moves_identical = true;
		if((int)moves.size() == 2) {
			//invokes func to check is two moves identical
		       	if(check_is_moves_identical(moves[0], moves[1]) == false) is_moves_identical = false; 
		}
		if(is_moves_identical) {	
			move_on_board(moves[0]);
			
			return true;	
		}		
	}
	//gets every possible single cell move for current cell
	vector<Cell> possible_moves = check_single_cell_moves(cell);
	//if we have one possible move, we invoke single_cell_move_on_board() to make move on the board
	if((int)possible_moves.size() == 1 && is_move_blocks_single(possible_moves[0]) == false) {
		single_cell_move_on_board(possible_moves[0]);	
		//mark_area_if_readt() checks is area of cell become completed after we made move on the board
		mark_area_if_ready(cell);	
		return true;
	}
	

	//in case if we have more than 1 possible single cell moves for current cell -
	// - we invoking filter_moves_single() to filter moves that simply impossible by the rules or to find special moves
	vector<Cell> filt_moves = filter_moves_single(possible_moves);	
	//if we invoked filter function and got one possible move we need to make this move on the board
	if((int)filt_moves.size() == 1) {
	       	single_cell_move_on_board(filt_moves[0]);
		//mark_area_if_readt() checks is area of cell become completed after we made move on the board
		mark_area_if_ready(cell);	
		return true;
	}	
	

		
	return false;	
	
}

//this is auxiliary function for testing
void Board::print_moves(vector<Area> moves) {
	int counter = 0;
	cout << moves.size() << endl;
	for(Area temp : moves) {
		for(Cell k : temp.cells) {
			cout << "[" << counter << "]: " << "poss(" << k.coord.x+1 << ", " << k.coord.y+1 << ")" << endl;  	
			bd[k.coord.x][k.coord.y].number = moves[0].number;
		}	
		print_board();	
		for(Cell k : temp.cells) {
			if(k.is_visited == false) bd[k.coord.x][k.coord.y].number = 0;
		}	
		counter++;
	}		
}	

//function that make moves on board (moves that is areas)
//if bool status == true we need to move on the board
//if bool status == false, revert move from board
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


//For every cell from the board if this cell is not 0 and not visited, function invokes make_move() for current cell
//showe steps is a parameter that indicates whether to show the board step by step after each move
void Board::solve(bool show_steps) {
	while(!is_board_solved()) {
		// is_main_cells_filled becomes true when all initial board cells are covered by completed areas
		bool is_main_cells_filled = true;
		//counter to check if any move was made by the cycle
		int  moves_was_made = 0;
		for(size_t i = 0; i < bd.size(); i++) {
			for(size_t k = 0; k < bd[0].size(); k++) {
				if(bd[i][k].number != 0 && bd[i][k].is_visited == false) {
					is_main_cells_filled = false;
					if(make_move(Cell(Pos(i, k), bd[i][k].number, true))){
					       	moves_was_made++;
						if(show_steps) print_board();	
					}
				}
			}		
		}
		//if all initial board cells are covered by completed areas, we have to fill empty space (empty space means cells with number 0) if it left on the board
		if(is_main_cells_filled == true) {
			fill_free_space(); 
		}
		//case when program made 0 move by the whole cycle and there are cells that not covered by area, so it means our program couldn't solve board
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
}	

//this function applies move that passed in and looks for cell that doesn't have possible moves after applying our move
//if there are cells that does not have possible moves, func retunrs true otherwise false
bool Board::check_is_any_cells_blocked(Area move) {
	move_on_board(move);
	for(Cell i : move.cells) {
		Pos ps[4] = { Pos(i.coord.x-1, i.coord.y), Pos(i.coord.x+1, i.coord.y), Pos(i.coord.x, i.coord.y-1), Pos(i.coord.x, i.coord.y+1) };
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


//this is auxiliary function for testing
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



//function applies move that is passed in and checks whether this move touches other areas with the same number
bool Board::is_touch_same_number(Area move) {
	for(Cell i : move.cells) {
		Pos ps[4] = { Pos(i.coord.x-1, i.coord.y), Pos(i.coord.x+1, i.coord.y), Pos(i.coord.x, i.coord.y-1), Pos(i.coord.x, i.coord.y+1) };
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




//retuns vector<Cell> with possible moves for cell
vector<Cell> Board::check_single_cell_moves(Cell cell, bool status) {
	vector<Cell> possible_moves;
	vector<Cell> counted_cells;
	//selects current cell's area
	vector<Cell> area = select_area_special(cell);
	//status to decide is func need to check is area is completed or no 
	if(status == true) {
	//if selected area equals number of cells it means area is completed that it marks it as visited and retunrs	
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
	//finds possible moves
	for(Cell i : area) { 
		Pos ps[4] = { Pos(i.coord.x-1, i.coord.y), Pos(i.coord.x+1, i.coord.y), Pos(i.coord.x, i.coord.y-1), Pos(i.coord.x, i.coord.y+1) };
		for(int k = 0; k < 4; k++) {
			if(ps[k].x >= 0 && (size_t)ps[k].x < bd.size() && ps[k].y >= 0 && (size_t)ps[k].y < bd[0].size()) {
				if(bd[ps[k].x][ps[k].y].number == 0 && bd[ps[k].x][ps[k].y].is_counted == false) {
					// Checks if the current move would block other cells.
					// If blocking is detected, the move is skipped.
					if(is_move_blocks_single(Cell(Pos(ps[k].x, ps[k].y), cell.number, false)) == true) {
						continue;
					}	
					int cells_in_area = 0;
					// Checks if the current move would touch completed area with same number.
					// If touching is detected, the move is skipped.
					cells_in_area = is_touch_same_number_single(Cell(Pos(ps[k].x, ps[k].y), cell.number, false), area);
					if(cells_in_area != 0) {
						if((int)area.size()+cells_in_area >= cell.number) continue;	
					}	
					possible_moves.push_back(Cell(Pos(ps[k].x, ps[k].y), cell.number, false));
					bd[ps[k].x][ps[k].y].is_counted = true;
					counted_cells.push_back(Cell(Pos(ps[k].x, ps[k].y), cell.number, false));
				}		
			}
		
		}		
	}
	for(Cell i : counted_cells) {
		bd[i.coord.x][i.coord.y].is_counted = false;
	}	
	return possible_moves;
}	
//func that cheks if single move blocks other cells
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

//special func to select area by passed in cell
vector<Cell> Board::select_area_special(Cell cell) {
	vector<Cell> stack;
	vector<Cell> area;
	stack.push_back(cell);
	bd[stack.back().coord.x][stack.back().coord.y].is_counted = true;
	area.push_back(bd[stack.back().coord.x][stack.back().coord.y]);
	while(stack.size() > 0) {
		Pos ps[4] = {Pos(stack.back().coord.x-1, stack.back().coord.y), Pos(stack.back().coord.x+1, stack.back().coord.y),
		       	Pos(stack.back().coord.x, stack.back().coord.y-1), Pos(stack.back().coord.x, stack.back().coord.y+1) };
		
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

//absolutely identical to move_on_board() but this func makes single cell moves
void Board::single_cell_move_on_board(Cell move, bool status) {
	//Move on the board
	if(status == true) {
		bd[move.coord.x][move.coord.y].number = move.number;
		bd[move.coord.x][move.coord.y].is_on_going = true;
		return;
	}
	//Remove move from the boad
	bd[move.coord.x][move.coord.y].number = 0;
	bd[move.coord.x][move.coord.y].is_on_going = false;
}

//cheks is move touchs completed area with the same number, but for single cell moves
int Board::is_touch_same_number_single(Cell cell, vector<Cell> current_cell_area) {
		Pos ps[4] = { Pos(cell.coord.x-1, cell.coord.y), Pos(cell.coord.x+1, cell.coord.y), Pos(cell.coord.x, cell.coord.y-1), Pos(cell.coord.x, cell.coord.y+1) };
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
						temp_area = select_area_special(Cell(Pos(ps[k].x, ps[k].y), cell.number, false));
						return (int)temp_area.size();
					}	
				}	
			}	
		}	
	return 0;
}

//funtion that filters passed in moves or finds unique move
vector<Cell> Board::filter_moves_single(vector<Cell> moves) {
	vector<Cell> filt_moves;

	//for every move from vector with moves
	for(Cell i : moves) {
		//making move on board
		single_cell_move_on_board(i);
		//geting all neighbours
		vector<Cell> neighb = get_neighbours(i);
			
		bool temp = false;
		//for every neighbours cell we looking for cells with number 1 or 0
		for(Cell k : neighb) {
			if(k.number == 0 || k.number == 1) temp = true;
		}

		//selecting current move are
		vector<Cell> move_area = select_area_special(i);
		//in case if we found that we don't have neighbours with cell number 1 or 0 and current move area is not completed
		if(temp == false && (int)move_area.size() != i.number) {
			//for every cell from move area
			for(Cell k : move_area) {
				//for every move area cells' neighbours 
				for(Cell h : get_neighbours(k)) {
					//if one of the neighbours is cell with number 0
					if(h.number == 0) temp = true;	
				}
			}
			//if one of the move cells' neighbour is 0 and possible moves that passed in the func is only 2	
			if(temp	== true && moves.size() == 2) {
				//skips current move and choose second one
				for(Cell l : moves) {
					if(l == i) continue; 
					filt_moves.push_back(l);
				}	
				//remove current move from board
				single_cell_move_on_board(i, false);
				//returns second move 
				return filt_moves; 
			}	
		}	
		bool hit_glob = false;
		bool clear_move_found = false;	
		for(Cell k : neighb) {
			bool hit = false;
			//if we found that move have neighbour cell with value 1 we have to check is any other cells can expand to this cell
			// Example:
			// Here's an example: The center cell with # symbol is our move that we check.
			// Suppose we decide not to go here and go up to the cell with * symbol, then we will have cell with value 1 and  empty cell naear to it, -
			// and all around the cells is already part of the filled area, -
			// so nobody can go there and we have two 1's next to each other, which can not be.
			//
			// 0 0 * 0 0 
			// 5 5 3 3 1
			// 5 5 # 1 4
			// 5 1 4 4 4
			//
			if(k.number == 1) {
				vector<Cell> able_to_move; 
				for(Cell h : neighb) {
					if(h.number != i.number && bd[h.coord.x][h.coord.y].is_visited == false) {
						able_to_move.push_back(h);
					}	
				}	
				//if we did't find any areas that can move to this cell we have this mobe is 
				if(able_to_move.size() == 0) {
					clear_move_found = true;
					break;
				}	
			}

			// case if we have empty space as our move's neighbour		
			if(k.number == 0) {
				int count_same_num = 0;
				//selecting empty space
				vector<Cell> free_space = select_area_special(k);	
				vector<Cell> fneighb;
				for(Cell n : free_space) {
					//selecting every areas that is next to the empty cells
					for(Cell m : get_neighbours(n)) {
						if(m.number != 0) fneighb.push_back(m);
					}		
				}	
				vector<Cell> able_to_move;
				//checking how many of areas that is next to the empty cells is incompleted, areas that are not completed their cells are placed to able_to_move
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
				//if all cells that next to the empty space is completed, we need to check if one of the current move's neighbours -
				//can expands to all this free space 	
				if(able_to_move.size() == 0) {
					for(Cell n : fneighb) {
						if(n.number == (int)free_space.size()) {
							hit = true;
							hit_glob = true;

							break;
						}
					}
				}
				
				if(hit == true) break;
				//in case if we have one of the area or cell that can expands to all this free space
				if(able_to_move.size() == 1 && (int)(select_area_special(able_to_move[0]).size() + free_space.size()) == able_to_move[0].number) {
					hit = true;	
					clear_move_found = true;

				}
				if(hit == true) break;		

				//in case if our move creates number of empty cells that mathces to the other areas completed area with same number -
				//so we have to recursive check if we can complete current move's area to avoid it.
				for(Cell l : fneighb) {
					if((int)free_space.size() == l.number) {
						int able_ways = 0;
						//checks it curent move has other free space where he can extend	
						for(Cell w : get_neighbours(i)) {
							if(w.number == 0) able_ways++; 
						}

						if(able_ways > 1) break;	
						//if current move does not have other free space to extend we need to check possible moves if we can make completed area
						vector<Cell> filt_moves_t = filter_moves_single(check_single_cell_moves(i, false));
						if((int)filt_moves_t.size() == 1) {
							hit = true;
							clear_move_found = true;
						}	
					}	
				}

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
	//checks is final moves blocks other cells
	for(Cell i : filt_moves) {
		if(is_move_blocks_single(i) == false) filt_moves_final.push_back(i);
	}	
	return filt_moves_final;
}	


//funtions that returns vector with all neighbour cells for that cell that passed in
vector<Cell> Board::get_neighbours(Cell cell) {
	vector<Cell> neighbours;	
	Pos ps[4] = { Pos(cell.coord.x-1, cell.coord.y), Pos(cell.coord.x+1, cell.coord.y), Pos(cell.coord.x, cell.coord.y-1), Pos(cell.coord.x, cell.coord.y+1) };
	for(int k = 0; k < 4; k++) {
		if(ps[k].x >= 0 && (size_t)ps[k].x < bd.size() && ps[k].y >= 0 && (size_t)ps[k].y < bd[0].size()) {
			neighbours.push_back(Cell(Pos(ps[k].x, ps[k].y), bd[ps[k].x][ps[k].y].number, bd[ps[k].x][ps[k].y].is_visited));
				
		}				
	}	
			
	return neighbours;	
}	


//this is auxiliary function for testing
vector<Area> Board::get_possible_moves_part(Cell cell) {
	vector<Cell> current_cells = select_area_special(cell);
	int cell_shortage = cell.number - (int)current_cells.size();
	bd[cell.coord.x][cell.coord.y].number = cell_shortage+1;
	vector<Area> temp = get_possible_moves(Cell(Pos(cell.coord.x, cell.coord.y), cell_shortage+1, true));
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
			moves[h].cells.push_back(Cell(Pos(j.coord.x, j.coord.y), cell.number, j.is_visited));
		}
		h++;	
	}		
	return moves;
}	


//pareses area by cell passed in and marks if this area has enough cells to become completed
void Board::mark_area_if_ready(Cell cell) {
	vector<Cell> area = select_area_special(cell);
	if((int)area.size() == cell.number) {
		for(Cell i : area) {
			if(bd[i.coord.x][i.coord.y].is_visited == false) bd[i.coord.x][i.coord.y].is_visited = true;
		}	
	}	
}	


//fincition that invokes fill_area() to all free space on board that left
void Board::fill_free_space() {
	for(size_t i = 0; i < bd.size(); i++) {
		for(size_t j = 0; j < bd[0].size(); j++) {
			if(bd[i][j].number == 0) {
				fill_area(select_area_special(Cell(Pos(i, j), 0, false))); 
			}	
		}	
	}
}

//filling free space that passed in
//the function checks the possible options for filling the free area with other areas, and so that they are not the same number as the neighboring areas
void Board::fill_area(vector<Cell> area) {
	vector<vector<int>> partitions = partition_number((int)area.size());
	for(vector<int> partition : partitions) {
		for(int number : partition) { 
			bd[area.back().coord.x][area.back().coord.y].number = number;
			vector<Area> moves = get_possible_moves(Cell(Pos(area.back().coord.x, area.back().coord.y), number, true));
			for(Area move : moves) {
				if(is_touch_same_number(move) == false) {
					move_on_board(move);
					return;
				}	
			}
		}
	}	
				


}	
//finds possible ways to split number by additives
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

//returns number splited in all possible ways by additive
vector<vector<int>> Board::partition_number(int number) {
	vector<vector<int>> result;
	vector<int> current;

	if(number <= 0) {
		return result;
	}
	
	find_combinations(number, number, current, result);
	return result;	
}	


//checks if two moves (areas) is identic by the shape
bool Board::check_is_moves_identical(Area move1, Area move2) {
	int coincidence = 0;
	vector<Pos> used_cells;
	for(Cell i : move1.cells) {
		for(Cell k : move2.cells) {
			if(bd[k.coord.x][k.coord.y].is_counted == true) continue;
			if(i == k) {
				coincidence++;
				bd[k.coord.x][k.coord.y].is_counted = true;
				used_cells.push_back(Pos(k.coord.x, k.coord.y));
			}	
		}	
	}
	for(Pos i : used_cells) {
		bd[i.x][i.y].is_counted = false;
	}		
	if(coincidence == (int)move1.cells.size()) return true;
	return false;
}	

