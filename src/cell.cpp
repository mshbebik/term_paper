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

Cell::Cell(int indef, Poss start, bool status) {
	this->indef = indef;
	this->start = start;
	this->status = status;
}

Cell::Cell() {
	indef = -1;
	start = Poss();
	status = false;
}

