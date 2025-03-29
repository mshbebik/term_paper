#include <iostream>
#include "board.h"
using namespace std;

int main (void){
	Board nw;
	nw.print_board();	
	nw.move(Poss(9, 1), Poss(-1, -1), 3, 1);
	nw.print_board();	
	cout << "Total variants: " << nw.get_count() << endl;
	return 0;
}

