#include <iostream>
using namespace std;


class Poss {
public:
	int x;
	int y;
	Poss(int x, int y){
		this->x = x;
		this->y = y;
	}
};


class Board {
public:
	int count = 0;
	int bd[10][10] = {0};
	void print_board();
	//void move(int x, int y, int number, int nb_count);
	void move(Poss start, Poss prev, int number, int nb_count);
};

void Board::print_board(){
	cout << "   ";
	for(int i = 0; i < 10; i++){
		cout << i+1 << " ";
	}
	cout << endl << endl;
	for(int i = 0; i < 10; i++){
		if(i+1 == 10) cout << i+1 << " ";
		else{cout << i+1 << "  ";}

		for(int a = 0; a < 10; a++){
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


void Board::move(Poss start, Poss prev, int number, int nb_count){
	if(start.x < 0 || start.x >= 10 || start.y < 0 || start.y >= 10){
	 	return;
	}
	cout << "Dot: " << start.x << ", " << start.y << endl; 
	Poss ps[4] = { Poss(start.x-1, start.y), Poss(start.x+1, start.y), Poss(start.x, start.y-1), Poss(start.x, start.y+1) };
	for(int i = 0; i < 4; i++){
		if(ps[i].x < 0 || ps[i].x >= 10 || ps[i].y < 0 || ps[i].y >= 10) return;
		if(nb_count == number){
			count++;
			return;
		}
		if(this->bd[ps[i].x][ps[i].y] != number && (ps[i].x != prev.x || ps[i].y != prev.y)){
			this->print_board();
			this->bd[ps[i].x][ps[i].y] = number;
			cout << "Total variants: " << this->count << endl;
			this->move(Poss(ps[i].x, ps[i].y), start, number, nb_count+1);
		}
	} 
}




int main (void){
	Board nw;
	nw.bd[3][3] = 4;
	nw.print_board();	
	//nw.move(3, 3, 3, 1);
	nw.move(Poss(3, 3), Poss(-1, -1), 4, 1);
	nw.print_board();	
	cout << "Total variants: " << nw.count << endl;
	return 0;
}

