#pragma once
#include "vector"

class Table {
public:
	Table(std::vector<std::vector<int>> t);


private:
	int line;
	int column;
	std::vector<std::vector<int>> table;
};