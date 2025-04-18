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

Cell::Cell(Poss coord, int number, bool is_visited, bool is_in_stack) {
	this->coord = coord;
	this->number = number;
	this->is_visited = is_visited;
	this->is_in_stack = is_in_stack;
}

Cell::Cell() {
	coord = Poss();
	number = 0;
	is_visited = false;
	is_in_stack = false;	
	is_on_going = false;
}

Area::Area(int number, vector<Cell> cells) {
	this->number = number;
	this->cells = cells;
}

Area::Area() {
	number = 0;
	cells = vector<Cell>();
}

Area::Area(int number) {
	this->number = number;
}	

StackCell::StackCell() {
	is_went_back = false;
	is_jumped_back = false;
	went_back_near = 0;
	blocked_count.push_back(0);

}	




