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
	Board nw("C:\\Users\\mbeby\\OneDrive\\Рабочий стол\\test_grid4.txt");
	nw.print_board();
//	nw.parse_areas();
//	if(nw.is_board_solved()) {
//		cout << "solved" << endl;
//	}
//	else{ 
//		cout << "not solved" << endl;
//	}
//	nw.bd[1][1].number = 2;
//	nw.print_possible_moves(Cell(Poss(6, 1), 7, true));
	nw.solve();
//	cout << "dsfsdfsdfd " << endl;
//	nw.parse_areas();
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

//	cout << nw.check_possible_moves(Cell(Poss(4, 4), 3, false)) << endl;


	
//	nw.show_visited_cells();
//	nw.move_on_board(moves[2]);
//	nw.print_board();
//	nw.show_visited_cells();
//	nw.print_possible_moves(Cell(Poss(1, 7), 3, true));
//	nw.print_board();
//	nw.move_on_board(moves[2], false);
//	nw.print_board();
//	nw.show_visited_cells();
	nw.print_board();
	
	return 0;

}

