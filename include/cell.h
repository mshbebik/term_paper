#ifndef CELL_H
#define CELL_H
#include <vector>
using namespace std;

class Poss {
public:
	int x;
	int y;

	Poss(int x, int y);
	Poss();
};

class Cell {
public:
	Poss start;
	int indef;
	bool status;
	vector<Poss> field;

	Cell(int indef, Poss start, bool status = false);
	Cell();
	
};
	

#endif
