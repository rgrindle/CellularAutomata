#ifndef CELLULAR_AUTOMATON_MANAGER_HPP
#define CELLULAR_AUTOMATON_MANAGER_HPP

#include "CellularAutomaton.hpp"

#include <iostream>

class CellularAutomatonManager
{
public:
	CellularAutomatonManager(unsigned int numberOfCellGroups, unsigned int numberOfCells);

	void performCommand(int cellGroupIndex, int cellIndex, int command);

	// get functions
	unsigned int getSituationCode(int cellGroupIndex, int cellIndex);
	int getCommand(unsigned int situation, std::vector<int> algorithm);
	
	// other functions
	bool isDone();	// check if done
	void iterateCellGroup();
	void printCellGroup(unsigned int index);

private:
	std::vector<CellularAutomaton> cellGroup__;
	bool done__ = false;
};

#endif // CELLULAR_AUTOMATON_MANAGER_HPP