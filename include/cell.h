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
	Poss coord;
	int number;
	bool is_visited;


	Cell(Poss coord, int number, bool is_visited = false);
	Cell();
	
};


class Area {
public:
	int number;
	vector<Cell> cells;

	Area(int number, vector<Cell> cells);
	Area();

};
	

#endif
