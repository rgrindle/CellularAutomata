#include "Cell.hpp"

Cell::Cell()
{
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

Cell::Cell(unsigned int size)
{
	this->setCellGroupSize(size);
}

void Cell::setCellGroupSize(unsigned int size)
{
	this->cellGroupSize__ = size;
}

unsigned int Cell::getCellGroupSize()
{
	return this->cellGroupSize__;
}

void Cell::addNewCellGroup()
{
	this->cellState__.push_back(std::vector<unsigned int>(cellGroupSize__));
}

unsigned int Cell::getPrevState(unsigned int index)
{
	return this->cellState__[cellState__.size()-2][index];
}

unsigned int Cell::getState(unsigned int index)
{
	return this->cellState__.back()[index];
}

void Cell::setState(unsigned int index, unsigned int state)
{
	cellState__.back()[index] = state;
}

std::vector<int> Cell::getAlgorthm()
{
	return this->algorithm__;
}

void Cell::setAlgorithm(std::vector<int> algorithm)
{
	this->algorithm__ = algorithm;
}

void Cell::printCellState()
{
	std::vector<unsigned int>::iterator it;
	for (it = this->cellState__.back().begin(); it < this->cellState__.back().end(); ++it) {
		std::cout << *it;
	}
	std::cout << std::endl;
}

unsigned int Cell::size()
{
	return this->cellState__.size();
}

bool Cell::isFinalState()
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