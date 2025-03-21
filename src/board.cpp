#include <iostream>
#include "board.h"
using namespace std;



void Board::init_board() {
	for(int i = 0; i < 10; i++) {
		for(int k = 0; k < 10; k++) {
			bd[i][k] = 0;
		}
	}
	count = 0;
}


void Board::print_board() {
	cout << "   ";
	for(int i = 0; i < 10; i++) {
		cout << i+1 << " ";
	}
	cout << endl << endl;
	for(int i = 0; i < 10; i++) {
		if(i+1 == 10) cout << i+1 << " ";
		else { cout << i+1 << "  "; }

		for(int a = 0; a < 10; a++) {
			cout << bd[i][a] << " ";
		}
		cout << endl;
	}

}

//void Board::move(int x, int y, int number, int nb_count){
//	Poss ps[4] = { Poss(x-1, y), Poss(x+1, y), Poss(x, y-1), Poss(x, y+1) };
//	for(int i = 0; i < 4; i++){
//		if(nb_count == number){
//			count++;
//			return;
//		}
//		if(this->bd[ps[i].x][ps[i].y] != number){
//			this->bd[ps[i].x][ps[i].y] = number;
//			this->move(ps[i].x, ps[i].y, number, nb_count+1);
//		}
//	} 
//}


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
		if(this->bd[ps[i].x][ps[i].y] != number && (ps[i].x != prev.x || ps[i].y != prev.y)) {
			this->print_board();
			this->bd[ps[i].x][ps[i].y] = number;
			cout << "Total variants: " << get_count() << endl;
			this->move(Poss(ps[i].x, ps[i].y), start, number, nb_count+1);
		}
	} 
}
	



	





