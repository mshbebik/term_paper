#include <iostream>
#include "board.h"
using namespace std;

int main (void){
	Board nw;
	nw.bd[3][3] = 4;
	nw.print_board();	
	//nw.move(3, 3, 3, 1);
	nw.move(Poss(3, 3), Poss(-1, -1), 4, 1);
	nw.print_board();	
	cout << "Total variants: " << nw.get_count() << endl;
	return 0;
}

