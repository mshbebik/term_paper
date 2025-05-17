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

					cout << "┌─────────────────────────┐" << endl;
					cout << "│ yourfile.txt    ─  ■  X │" << endl;
					cout << "├─────────────────────────┤" << endl;
					cout << "│ 1 2 1 0                 │" << endl;
					cout << "│ 0 0 0 0                 │" << endl;
					cout << "│ 0 0 0 1                 │" << endl;
					cout << "│                         │" << endl;
					cout << "│                         │" << endl;
					cout << "└─────────────────────────┘" << endl << endl;
					 
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


	//Menu	
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

