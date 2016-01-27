#include "CellularAutomaton.hpp"

int main(int argc, char * argv[])
{
	if (argc != 4) {
		std::cout << "Bad cmd line arguments. Enter as follows." << std::endl;
		std::cout << "CellularAutomaton <numberOfCellsPerRow> <numberOfStates> <rule>" << std::endl;
		//return 1;
	}

	CellularAutomaton CA(100,2,85);	// generate a cellular automaton with a width of 100 cells

	// while there is no repeated row state keep iterating
	while(!CA.isFinalState()) {
		CA.iterateCellGroup();	// iterate to get next row
		CA.printCellState();	// print the current states
	}
	// before finishing, row number that match the last row is printed to the screen
	system("pause");
	return 0;
}