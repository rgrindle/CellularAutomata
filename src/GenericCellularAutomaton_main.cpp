#include "CellularAutomatonManager.hpp"

int main(int argc, char * argv[])
{
	CellularAutomatonManager CA(1,100);	// generate a cellular automaton with a width of 100 cells

	// while there is no repeated automaton state keep iterating
	while(!CA.isDone()) {
		CA.printCellGroup(0);	// print the current states
		CA.iterateCellGroup();	// iterate
	}
	// before finishing, row number that match the last row is printed to the screen
	system("pause");
	return 0;
}