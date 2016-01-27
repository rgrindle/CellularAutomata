#include "CellularAutomatonManager.hpp"

//=============================================================================
//	Constructor/Desctructor Functions
//=============================================================================

CellularAutomatonManager::CellularAutomatonManager(unsigned int numberOfCellGroups, unsigned int numberOfCells, unsigned int numberOfStates, int algorithm)
{
	// set the population size
	this->cellGroup__.resize(numberOfCellGroups);

	// set the width of the automaton
	std::vector<CellularAutomaton>::iterator it;
	for (it = this->cellGroup__.begin(); it < this->cellGroup__.end(); ++it) {
		it->setCellGroupSize(numberOfCells);
		it->setNumberOfStates(numberOfStates);
		it->setAlgorithm(algorithm);
		it->addNewCellGroup();
		it->setState(numberOfCells / 2, 2);
		it->setState(numberOfCells / 2-1, 2);
		it->setState(numberOfCells / 2+1, 2);
	}
}

//=============================================================================
//	Get Functions
//=============================================================================



//=============================================================================
//	Set Functions
//=============================================================================

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
		this->cellGroup__[i].iterateCellGroup();
	}
}