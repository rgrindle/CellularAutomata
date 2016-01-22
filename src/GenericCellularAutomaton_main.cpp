#include "CellularAutomatonManager.hpp"

int main(int argc, char * argv[])
{
	CellularAutomatonManager CA(1,100);

	while(!CA.isDone()) {
		CA.printCellGroup(0);
		CA.iterateCellGroup();
	}

	system("pause");
	return 0;
}