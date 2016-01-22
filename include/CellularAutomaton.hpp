#ifndef CELL_HPP
#define CELL_HPP

#include <vector>
#include <iostream>

class Cell
{
public:
	Cell();
	Cell(unsigned int size);
	void addNewCellGroup();
	void setCellGroupSize(unsigned int size);
	unsigned int getCellGroupSize();

	unsigned int getState(unsigned int index);
	unsigned int getPrevState(unsigned int index);
	void setState(unsigned int index, unsigned int state);

	std::vector<int> getAlgorthm();
	void setAlgorithm(std::vector<int> algorithm);

	bool isFinalState();

	void printCellState();

	unsigned int size();

private:
	int numberOfStates__;
	std::vector<std::vector<unsigned int> > cellState__;
	//std::vector<unsigned int> prevCellState__;
	//std::vector<unsigned int> firstCellState__;
	std::vector<int> algorithm__;
	unsigned int cellGroupSize__;
};

#endif // CELL_HPP