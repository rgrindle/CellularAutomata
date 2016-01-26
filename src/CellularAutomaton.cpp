#include "CellularAutomaton.hpp"

//=============================================================================
//	Constructor/Destructor Functions
//=============================================================================

CellularAutomaton::CellularAutomaton()
{
	// set algorithm, this should be different
	std::vector<int> al;
	al.push_back(0);
	al.push_back(1);
	al.push_back(1);
	al.push_back(0);
	al.push_back(1);
	al.push_back(1);
	al.push_back(1);
	al.push_back(0);
	//al.push_back(0);
	//al.push_back(0);
	//al.push_back(0);
	//al.push_back(0);
	//al.push_back(0);
	//al.push_back(0);
	//al.push_back(0);
	//al.push_back(0);

	this->setAlgorithm(al);
}

CellularAutomaton::CellularAutomaton(unsigned int size)
{
	this->setCellGroupSize(size);
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

//=============================================================================
//	Set Functions
//=============================================================================

void CellularAutomaton::setCellGroupSize(unsigned int size)
{
	this->cellGroupSize__ = size;
}

void CellularAutomaton::setState(unsigned int index, unsigned int state)
{
	cellState__.back()[index] = state;
}

void CellularAutomaton::setAlgorithm(std::vector<int> algorithm)
{
	this->algorithm__ = algorithm;
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
			std::cout << i << std::endl;
			return true;
		}
	}

	return false;
}