#include <iostream>
#include <cstdlib> // for system("clr")
#include <ctype.h>		   
#include "board.h"
using namespace std;


//main menu text
void options() {
	cout << "--- Filomino Solver ---" << endl;
	cout << "Choose how you want to solve the puzzle" << endl;
	cout << "1. Solve yourself. " << endl;
	cout << "2. Solve by program. " << endl;
	cout << "3. Exit. " << endl;
	return;
}	


void type_of_input() {
	cout << "Choose the way you want to initialize your board." << endl;
	cout << "1. Import from file. " << endl;
	cout << "2. Input manually. " << endl;	
	cout << "3. Back to menu. " << endl;
	return;
}		






//function retuns true when board is initialized ant false when user want to go back to main menu.
bool select_input_type(Board &obj) {
	system("cls");
	type_of_input();
	while(cin.get() != '\n') {};
	char choice_tmp = ' ';
	cout << "Enter you choice: ";
	string filename = "";
	//indicates if user decided to go back.
	bool back_status = false;
	//indicates when choosed wrong option.
	bool is_selected_wrong_option = false;
	while(true) {
		choice_tmp = cin.get();
		switch(choice_tmp) {
			case '1':
				system("cls");
				while(cin.get() != '\n') {};
				while(true)  {
					cout << endl << "Example of file: " << endl << endl;
					cout << "1 2 1 0" << endl;
					cout << "0 0 0 0" << endl;
					cout << "0 0 0 1" << endl << endl;
					 
					cout << "Rules: " << endl;
					cout << "1. Each number must to be separated by space." << endl; 
					cout << "2. Empty cells must to be entered as 0." << endl; 
					cout << "3. File must contain only numeric symbols and spaces." << endl << endl;

					cout << "Enter file path or 'b' to go back: ";
					getline(cin, filename);
					if((filename.size() == 1) && (tolower(filename[0]) == 'b')) {
						back_status = true;
						break;
					}		
					try {
						obj.init_from_file(filename);
						break;
					} catch (...) {
						filename.clear();
						continue;	
					}
				}	
				if(back_status == true) break;
				return true;


			case '2':      
				while(cin.get() != '\n') {};
				if(obj.init_from_user() == false) break; 
				
				return true;
			case '3': 
				while(cin.get() != '\n') {};
				return false;


			default:	
				is_selected_wrong_option = true;
				while(cin.get() != '\n') {};
		}	
		system("cls");
		if(is_selected_wrong_option) cout << "\033[31m### Select one of the options 1, 2 or 3. ###\033[0m" << endl;
		type_of_input();
		cout << "Enter your choice: ";

	}	
			
}


bool should_show_steps() {
	while(true) {
		cout << "Do you want to see the program's steps while solving [y/n]: ";
		char ch = ' ';	
		while((tolower((ch = cin.get())) != 'y' || tolower(ch) != 'n') && cin.peek() != '\n') {
			while(cin.get() != '\n') {}
			cout << "\033[31m### Enter 'n' or 'y' and press [Enter]. ###\033[0m" << endl;
			cout << "Do you want to see the program's steps while solving [y/n]: ";
		}	

		while(cin.get() != '\n') {}
		return (ch == 'n') ? false : true;
	}	
	return false;
}	



int main (void){

	Board nw;
//	nw.init_from_user();
//	nw.print_board();	

//	Board nw("C:\\Users\\mbeby\\OneDrive\\Рабочий стол\\test_grid2.txt");
//	nw.parse_areas();
//	if(nw.is_board_solved()) {
//		cout << "solved" << endl;
//	}
//	else{ 
//		cout << "not solved" << endl;
//	}
//	nw.bd[1][1].number = 2;
//	nw.solve();
//	nw.print_possible_moves(Cell(Poss(6, 3), 5, true));
//	nw.solve();
//	cout << "dsfsdfsdfd " << endl;
//	nw.parse_areas();
//	nw.print_board();
//	nw.show_visited_cells();
//	nw.print_possible_moves(Poss(8, 4), Poss(-1, -1), 8, 1);
//	vector<Area> moves = nw.get_possible_moves_itt(Cell(Poss(0, 2), 3, false));
//	int index = 0;
//	for(Area i : moves) {
//		for(Cell k : i.cells) {
//			cout << "[" << index << "]: (" << k.coord.x+1 << ", " << k.coord.y+1 << ")" << endl;
//		}
//		index++;	
//	}	




	
//	nw.show_visited_cells();
//	nw.move_on_board(moves[2]);
//	nw.print_board();
//	nw.show_visited_cells();
//	nw.single_cell_move_on_board(Cell(Poss(0, 0), 3, true));
//	nw.print_possible_moves(Cell(Poss(1, 2), 7, true));
//	nw.print_board();
//	nw.move_on_board(moves[2], false);
//	nw.print_board();
//	nw.show_visited_cells();
//	
//	for(Cell i : nw.select_area_special(Cell(Poss(1, 9), 6, false))) {
//		cout << "(" << i.coord.x+1 << ", " << i.coord.y+1 << ")" << endl; 
//	}	
	
//     	vector<Cell> moves = nw.check_single_cell_moves(Cell(Poss(6, 3), 3, true));
//	for(Cell i : moves) { 
//		cout << "(" << i.coord.x+1 << ", " << i.coord.y+1 << ")" << endl; 
//	}	
//	cout << endl;
//	cout << nw.is_move_blocks_single(Cell(Poss(8, 5), 3, true)) << endl;
//	nw.single_cell_move_on_board(Cell(Poss(8, 5), 3, false));
//	nw.print_possible_moves(Cell(Poss(4, 2), 9, true));
//	vector<Cell> filt_moves = nw.filter_moves_single(moves);
//	for(Cell i : filt_moves) {
//		cout << "(" << i.coord.x+1 << ", " << i.coord.y+1 << ")" << endl; 
//	}
		
	/*	
	vector<Area> moves = nw.get_possible_moves_part(Cell(Poss(1, 6), 7, true));
	int index = 1;
	for(Area i : moves) {
		for(Cell j : i.cells) {
			cout << "[" << index << "] " << "(" << j.coord.x+1 << ", " << j.coord.y+1 << ")" << endl; 
		}	
		index++;
		cout << endl;
	}	
	*/

		

//	cout << nw.is_move_blocks_single(Cell(Poss(5, 7), 7, false));

//	nw.print_board();
	
	//MENU//
	

	



	
	char choice = ' ';
	bool should_shw_steps = false;
	options();	
	cout << "Enter your choice: ";
	while(true) {
		choice = cin.get();
		switch(choice) {
			case '1':
				cout << endl;
				if(select_input_type(nw) == false) break;
				system("cls");
				nw.solve_by_user();

				break;
			case '2':
				cout << endl;
				if(select_input_type(nw) == false) break;
				system("cls");
				should_shw_steps = should_show_steps(); 
				nw.solve(should_shw_steps);
				
				break;

			case '3':
				return 1;	
			default:	
				cout << "\033[31m### Select one of the options 1, 2 or 3. ###\033[0m" << endl;
				while(cin.get() != '\n') {};
		}	
		cout << endl; 
		options();	
		cout << "Enter your choice: ";
	}	


	return 0;

}

