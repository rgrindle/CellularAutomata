#include "CellularAutomaton.hpp"

//=============================================================================
//	Constructor/Destructor Functions
//=============================================================================

CellularAutomaton::CellularAutomaton()
{

}

CellularAutomaton::CellularAutomaton(unsigned int size, unsigned int numberOfStates, unsigned int algorithm)
{
	this->setCellGroupSize(size);
	this->addNewCellGroup();
	this->setState(size / 2, 1);
	this->setState(size / 2 - 1, 1);
	this->setState(size / 2 + 1, 1);
	this->setNumberOfStates(numberOfStates);
	this->setAlgorithm(algorithm);
	this->printCellState();
}

//=============================================================================
//	Get Functions
//=============================================================================

unsigned int CellularAutomaton::getCellGroupSize()
{
	return this->cellGroupSize__;
}

unsigned int CellularAutomaton::getPrevState(unsigned int index)
{
	return this->cellState__[cellState__.size() - 2][index];
}

unsigned int CellularAutomaton::getState(unsigned int index)
{
	return this->cellState__.back()[index];
}

std::vector<int> CellularAutomaton::getAlgorthm()
{
	return this->algorithm__;
}

// Get the number of rows of cell states.
unsigned int CellularAutomaton::size()
{
	return this->cellState__.size();
}

// Based on the state of three cells (left, right, and itself) get a number representing situation in [0,7].
unsigned int CellularAutomaton::getSituationCode(int cellIndex)
{
	std::vector<unsigned int> situationString(3);

	// check if cell to the right exists
	if (cellIndex + 1 < this->cellState__[0].size()) {
		// get state of cell to the right
		situationString[0] = this->getPrevState(cellIndex + 1);
	}
	else {	// rap around if last cell
			// get leftmost cell state
		situationString[0] = this->getPrevState(0);
	}

	// get state of itself
	situationString[1] = this->getPrevState(cellIndex);

	// check if cell to the left exists
	if (cellIndex - 1 >= 0) {
		// get state of cell to the left
		situationString[2] = this->getPrevState(cellIndex - 1);
	}
	else {	// rap around if first cell
			// get rightmost cell state
		situationString[2] = this->getPrevState(this->cellState__[0].size() - 1);
	}

	// convert to number
	return this->cvtStringToNumber(situationString);
}

// Situation acts as the index to algorithm.
// The number stored at that location is the command.
int CellularAutomaton::getCommand(unsigned int situation)
{
	if (situation > pow(this->numberOfStates__, 3) - 1) {
		std::cout << std::endl;
	}
	return this->algorithm__[situation];
}

//=============================================================================
//	Set Functions
//=============================================================================

void CellularAutomaton::setCellGroupSize(unsigned int size)
{
	this->cellGroupSize__ = size;
}

void CellularAutomaton::setNumberOfStates(unsigned int numberOfStates)
{
	this->numberOfStates__ = numberOfStates;
}

void CellularAutomaton::setState(unsigned int index, unsigned int state)
{
	cellState__.back()[index] = state;
}

void CellularAutomaton::setAlgorithm(std::vector<int> algorithm)
{
	this->algorithm__ = algorithm;
}

// set algorithm based on the number of state (that is the base of the number) and a base 10 number representing the algorithm
void CellularAutomaton::setAlgorithm(int algorithm)
{
	if (this->numberOfStates__ > 3) {
		std::cout << "Error from CellularAutomaton::setAlgorithm(int): This function only works for CellularAutomaton::numberOfStates__ <= 3." <<
							"Either lower the number of states or use CellularAutomaton::setAlgorithm(std::vector<int>)." << std::endl;
		return;
	}

	int numberOfSituations = (unsigned int) pow(this->numberOfStates__,3);
	std::vector<int> algVector(numberOfSituations);

	int exp;
	for (exp = numberOfSituations-1; exp >= 0 ; --exp) {
		int digit;
		for (digit = this->numberOfStates__-1; digit >= 0; --digit) {
			long long unsigned int nextNumber = (int) digit*pow(this->numberOfStates__, exp);
			if (algorithm >= nextNumber) {
				algVector[exp] = digit;
				algorithm -= nextNumber;
				break;
			}
		}
	}

	std::cout << "Algorithm entered: ";
	for (exp = algVector.size()-1; exp >= 0; --exp)
		std::cout << algVector[exp];

	std::cout << std::endl;
	this->setAlgorithm(algVector);
}

//=============================================================================
//	Other Functions
//=============================================================================

// Add a new row of cells.
void CellularAutomaton::addNewCellGroup()
{
	this->cellState__.push_back(std::vector<unsigned int>(cellGroupSize__));
}

// Print the current row of cells' states.
void CellularAutomaton::printCellState()
{
	std::vector<unsigned int>::iterator it;
	for (it = this->cellState__.back().begin(); it < this->cellState__.back().end(); ++it) {
		std::cout << *it;
	}
	std::cout << std::endl;
}

// Check if there have been any repeated rows.
// If so, done.
bool CellularAutomaton::isFinalState()
{
	unsigned int i;
	for (i = 0; i < this->cellState__.size()-1; ++i) {
		if (cellState__.back() == cellState__[i]) {
			std::cout << "Match found at index " << i << std::endl;
			return true;
		}
	}

	return false;
}

unsigned int CellularAutomaton::cvtStringToNumber(std::vector<unsigned int> numberString)
{
	unsigned int number = 0;

	unsigned int i;
	for (i = 0; i < numberString.size(); ++i) {
		number += (unsigned int) (numberString[i] * pow(this->numberOfStates__, i));
	}
	return number;
}

// The command is the state to wich the cell will be set.
void CellularAutomaton::performCommand(int cellIndex, int command)
{
	this->setState(cellIndex, command);
}

// Build the next row of cell states.
void CellularAutomaton::iterateCellGroup()
{
	if (this->isFinalState()) {	// check if the current automaton is finished.
		return;	// Skip finished automaton.
	}
	this->addNewCellGroup();	// Add a new row of cell states before trying to edit it.

	// Loop through each cell state in the current row to create the next row.
	unsigned int j;
	for (j = 0; j < this->cellState__[0].size(); ++j) {
		unsigned int situation = this->getSituationCode(j);	// get the situation
		int command = this->getCommand(situation);	// use the situation to get the command
		performCommand(j, command);	// preform the command
	}
}