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

class StackCell {
public:
	bool is_went_back;
	int went_back_near;
	bool is_jumped_back;
	vector<Poss> blocked;
	vector<int> blocked_count;

	StackCell();

};	

class Cell {
public:
	Poss coord;
	int number;
	bool is_visited;
	bool is_in_stack;
	StackCell in_stack;


	Cell(Poss coord, int number, bool is_visited = false, bool is_in_stack = false);
	Cell();
};


class Area {
public:
	int number;
	vector<Cell> cells;

	Area(int number, vector<Cell> cells);
	Area(int number);
	Area();
};



#endif
