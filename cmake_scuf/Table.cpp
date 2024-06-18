#include "Table.h"

Table::Table(std::vector<std::vector<int>> t)
{
	table = t;
	line = t.size();
	column = t[0].size();
}
