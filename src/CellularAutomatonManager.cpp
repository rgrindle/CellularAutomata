#include "CellularAutomatonManager.hpp"

//=============================================================================
//	Constructor/Desctructor Functions
//=============================================================================

CellularAutomatonManager::CellularAutomatonManager(unsigned int numberOfCellGroups, unsigned int numberOfCells)
{
	// set the population size
	this->cellGroup__.resize(numberOfCellGroups);

	// set the width of the automaton
	std::vector<CellularAutomaton>::iterator it;
	for (it = this->cellGroup__.begin(); it < this->cellGroup__.end(); ++it) {
		it->setCellGroupSize(numberOfCells);
		it->addNewCellGroup();
		it->setState(numberOfCells / 2, 1);
	}
}

//=============================================================================
//	Get Functions
//=============================================================================

// Based on the state of three cells (left, right, and itself) get a number representing situation in [0,7].
unsigned int CellularAutomatonManager::getSituationCode(int cellGroupIndex, int cellIndex)
{
	// initial situation to zero that way no bits need to be cleared
	unsigned int situation = 0;

	// if cell is on
	if (this->cellGroup__[cellGroupIndex].getPrevState(cellIndex)) {
		situation |= 2;	// set the second bit
	}

	// check if cell to the left exists
	if (cellIndex - 1 >= 0) {
		// if cell to the left is on
		if (this->cellGroup__[cellGroupIndex].getPrevState(cellIndex - 1)) {
			situation |= 4;	// set the third bit
		}
	}
	else {	// rap around if first cell
		// if rightmost cell is on
		if (this->cellGroup__[cellGroupIndex].getPrevState(this->cellGroup__[cellGroupIndex].getCellGroupSize() - 1)) {
			situation |= 4;	// set the third bit
		}
	}

	// check if cell to the right exists
	if (cellIndex + 1 < this->cellGroup__[cellGroupIndex].getCellGroupSize()) {
		// if cell to the right is on
		if (this->cellGroup__[cellGroupIndex].getPrevState(cellIndex + 1)) {
			situation |= 1;	// set the first bit
		}
	}
	else {	// rap around if last cell
		// if leftmost cell is on
		if (this->cellGroup__[cellGroupIndex].getPrevState(0)) {
			situation |= 1;	// set the first bit
		}
	}

	return situation;
}

// Situation acts as the index to algorithm.
// The number stored at that location is the command.
int CellularAutomatonManager::getCommand(unsigned int situation, std::vector<int> algorithm)
{
	return algorithm[situation];
}

//=============================================================================
//	Set Functions
//=============================================================================

// The command is the state to wich the cell will be set.
void CellularAutomatonManager::performCommand(int cellGroupIndex, int cellIndex, int command)
{
	this->cellGroup__[cellGroupIndex].setState(cellIndex, command);
}

bool CellularAutomatonManager::isDone()
{
	// Loop through the automata.
	unsigned int i;
	for (i = 0; i < this->cellGroup__.size(); ++i) {
		// If any automaton is not done.
		if (!cellGroup__[i].isFinalState()) {
			return false;	// return false, meaning not all automata are done.
		}
	}

	// if it gets this far all automata are done.
	return true;
}

// Print the current row of the automaton referenced by index.
void CellularAutomatonManager::printCellGroup(unsigned int index)
{
	this->cellGroup__[index].printCellState();
}

// Build the next row of cell states.
void CellularAutomatonManager::iterateCellGroup()
{
	// Loop through all automata.
	unsigned int  i;
	for (i = 0; i < this->cellGroup__.size(); ++i) {
		if (cellGroup__[i].isFinalState()) {	// check if the current automaton is finished.
			continue;	// Skip finished automaton.
		}
		std::vector<int> al = this->cellGroup__[i].getAlgorthm();
		this->cellGroup__[i].addNewCellGroup();	// Add a new row of cell stat before trying to edit it.

		// Loop through each cell state in the current row to create the next row.
		unsigned int j;
		for (j = 0; j < this->cellGroup__[i].getCellGroupSize(); ++j) {
			unsigned int situation = this->getSituationCode(i, j);	// get the situation
			int command = this->getCommand(situation, al);	// use the situation to get the command
			performCommand(i, j, command);	// preform the command
		}
	}
}