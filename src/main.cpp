#include <iostream>
#include "board.h"
using namespace std;

int main (void){
 	/*
	Board nw;
	nw.print_board();	
	nw.move(Poss(9, 1), Poss(-1, -1), 3, 1);
	nw.print_board();	
	cout << "Total variants: " << nw.get_count() << endl;
	*/
	Board nw("C:\\Users\\mbeby\\OneDrive\\Рабочий стол\\test_grid.txt");
	nw.print_board();
	nw.parse_areas();
	if(nw.is_board_solved()) {
		cout << "solved" << endl;
	}
	else{ 
		cout << "not solved" << endl;
	}
	nw.bd[1][1].number = 2;
	nw.move(Poss(0, 2), Poss(-1, -1), 3, 1);
	cout << "Total variants: " << nw.count << endl;
	return 0;
}

