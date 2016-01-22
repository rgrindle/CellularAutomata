#ifndef CELLULAR_AUTOMATON_MANAGER_HPP
#define CELLULAR_AUTOMATON_MANAGER_HPP

#include "CellularAutomaton.hpp"

#include <iostream>

class CellularAutomatonManager
{
public:
	CellularAutomatonManager(unsigned int numberOfCellGroups, unsigned int numberOfCells);

	void printCellGroup(unsigned int index);
	void iterateCellGroup();

	unsigned int getSituationCode(int cellGroupIndex, int cellIndex);
	int getCommand(unsigned int situation, std::vector<int> algorithm);
	void performCommand(int cellGroupIndex, int cellIndex, int command);
	
	bool isDone();

private:
	std::vector<CellularAutomaton> cellGroup__;
	bool done__ = false;
};

#endif // CELLULAR_AUTOMATON_MANAGER_HPP