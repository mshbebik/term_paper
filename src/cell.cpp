#include <iostream>
#include "cell.h"
using namespace std;


Poss::Poss(int x, int y) {
	this->x = x;
	this->y = y;

}

Poss::Poss() {
	x = -1;
	y = -1;
}

Cell::Cell(Poss coord, int number, bool is_visited) {
	this->coord = coord;
	this->number = number;
	this->is_visited = is_visited;
}

Cell::Cell() {
	coord = Poss();
	number = 0;
	is_visited = false;	
}

Area::Area(int number, vector<Cell> cells) {
	this->number = number;
	this->cells = cells;
}

Area::Area() {
	this->number = 0;
}

