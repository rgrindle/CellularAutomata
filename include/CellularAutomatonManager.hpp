#ifndef CELLULAR_AUTOMATON_MANAGER_HPP
#define CELLULAR_AUTOMATON_MANAGER_HPP

#include "CellularAutomaton.hpp"

#include <iostream>

class CellularAutomatonManager
{
public:
	CellularAutomatonManager(unsigned int numberOfCellGroups, unsigned int numberOfCells, unsigned int numberOfStates, int algorithm);

	// get functions

	
	// other functions
	bool isDone();	// check if done
	void iterateCellGroup();
	void printCellGroup(unsigned int index);

private:
	std::vector<CellularAutomaton> cellGroup__;
	bool done__ = false;
};

#endif // CELLULAR_AUTOMATON_MANAGER_HPP