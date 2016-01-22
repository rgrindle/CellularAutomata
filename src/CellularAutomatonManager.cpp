#include "CellularAutomatonManager.hpp"

CellularAutomatonManager::CellularAutomatonManager(unsigned int numberOfCellGroups, unsigned int numberOfCells)
{
	this->cellGroup__.resize(numberOfCellGroups);

	std::vector<Cell>::iterator it;
	for (it = this->cellGroup__.begin(); it < this->cellGroup__.end(); ++it) {
		it->setCellGroupSize(numberOfCells);
		it->addNewCellGroup();
		it->setState(numberOfCells / 2, 1);
	}
}

void CellularAutomatonManager::printCellGroup(unsigned int index)
{
	this->cellGroup__[index].printCellState();
}


void CellularAutomatonManager::iterateCellGroup()
{
	//std::vector<unsigned int> newCell;

	unsigned int  i;
	for (i = 0; i < this->cellGroup__.size(); ++i) {
		if(cellGroup__[i].isFinalState())	{
			this->done__ = true;
			break;
		}
		std::vector<int> al = this->cellGroup__[i].getAlgorthm();
		//std::cout << "Algorithm ";
		//for (int k = 0; k < al.size(); ++k) {
		//	std::cout << al[k] << ',';
		//}
		//std::cout << std::endl;
		this->cellGroup__[i].addNewCellGroup();

		unsigned int j;
		for (j = 0; j < this->cellGroup__[i].getCellGroupSize(); ++j) {
			unsigned int situation = this->getSituationCode(i,j);
			int command = this->getCommand(situation, al);
			performCommand(i,j, command);
			//newCell.push_back(command);
			//std::cout << command;
		}
		//std::cout << std::endl;
	}
}

unsigned int CellularAutomatonManager::getSituationCode(int cellGroupIndex, int cellIndex)
{
	unsigned int situation = 0;

	if (this->cellGroup__[cellGroupIndex].getPrevState(cellIndex)) {
		situation |= 2;
	}

	// check if cell to the left exists
	if (cellIndex - 1 >= 0) {
		if (this->cellGroup__[cellGroupIndex].getPrevState(cellIndex - 1)) {
			situation |= 4;
		}
	}
	else {	// rap around if first cell
		if (this->cellGroup__[cellGroupIndex].getPrevState(this->cellGroup__[cellGroupIndex].getCellGroupSize() - 1)) {
			situation |= 4;
		}
	}

	// check if cell to the right exists
	if (cellIndex + 1 < this->cellGroup__[cellGroupIndex].getCellGroupSize()) {
		if (this->cellGroup__[cellGroupIndex].getPrevState(cellIndex + 1)) {
			situation |= 1;
		}
	}
	else {	// rap around if last cell
		if (this->cellGroup__[cellGroupIndex].getPrevState(0)) {
			situation |= 1;
		}
	}

	return situation;
}

int CellularAutomatonManager::getCommand(unsigned int situation, std::vector<int> algorithm)
{
	return algorithm[situation];
}

void CellularAutomatonManager::performCommand(int cellGroupIndex, int cellIndex, int command)
{
	this->cellGroup__[cellGroupIndex].setState(cellIndex, command);
}

bool CellularAutomatonManager::isDone()
{
	return this->done__;
}