#ifndef CELLULAR_AUTOMATON_HPP
#define CELLULAR_AUTOMATON_HPP

#include <vector>
#include <iostream>
#include <math.h> 

class CellularAutomaton
{
public:
	CellularAutomaton();
	CellularAutomaton(unsigned int size, unsigned int numberOfStates, unsigned int algorithm);

	// set functions
	void setCellGroupSize(unsigned int size);
	void setNumberOfStates(unsigned int numberOfStates);
	void setState(unsigned int index, unsigned int state);
	void setAlgorithm(std::vector<int> algorithm);
	void setAlgorithm(int algorithm);

	// get functions
	unsigned int getCellGroupSize();
	unsigned int getState(unsigned int index);
	unsigned int getPrevState(unsigned int index);
	std::vector<int> getAlgorthm();
	unsigned int size();	// get the number of rows of cell states
	unsigned int getSituationCode(int cellIndex);
	int getCommand(unsigned int situation);

	// other functions
	bool isFinalState();	// check for repeat states
	void addNewCellGroup();	// push back a new row of cells
	void printCellState();	// print the current row's cell states
	unsigned int cvtStringToNumber(std::vector<unsigned int> numberString);
	void performCommand(int cellIndex, int command);
	void iterateCellGroup();

private:
	std::vector<int> algorithm__;

	int numberOfStates__;
	std::vector<std::vector<unsigned int> > cellState__;
	unsigned int cellGroupSize__;
};

#endif // CELLULAR_AUTOMATON_HPP