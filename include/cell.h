#ifndef CELL_H
#define CELL_H
#include <vector>
using namespace std;

class Pos {
public:
	int x;
	int y;

	Pos(int x, int y);
	Pos();
};

class StackCell {
public:
	bool is_went_back;
	int went_back_near;
	vector<Pos> blocked;
	vector<int> blocked_count;

	StackCell();

};	

class Cell {
public:
	Pos coord;
	int number;
	bool is_visited;
	bool is_counted;
	bool is_in_stack;
	bool is_on_going;
	StackCell in_stack;


	Cell(Pos coord, int number, bool is_visited = false, bool is_in_stack = false);
	Cell();
	bool operator==(const Cell &obj);
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
