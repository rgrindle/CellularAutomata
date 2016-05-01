#ifndef CELLULAR_AUTOMATON_HPP
#define CELLULAR_AUTOMATON_HPP

#include "opencv2\opencv.hpp"

#include <string>
#include <vector>
#include <iostream>
#include <algorithm>
#include <math.h> 
#include <ctime> 
#include <random>

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
	void setRandomAlgorithm();
	void setRandomStates();
	void setScore(int score);
	void changeScore(int changeFactor);
	void reset();	// start over with cellState

	// get functions
	unsigned int getCellGroupSize();
	unsigned int getState(unsigned int index);
	unsigned int getPrevState(unsigned int index);
	std::vector<int> getAlgorithm();
	unsigned int size();	// get the number of rows of cell states
	unsigned int getSituationCode(int cellIndex);
	int getCommand(unsigned int situation);
	int getScore();

	// other functions
	bool isFinalState();	// check for repeat states
	bool isFinalStateQuick();	// check only last two rows for repeat
	void addNewCellGroup();	// push back a new row of cells
	void printCellState();	// print the current row's cell states
	unsigned int cvtStringToNumber(std::vector<unsigned int> numberString);
	void performCommand(int cellIndex, int command);
	void iterateCellGroup();
	bool isCorrect();	// did the algorithm work

	void drawCellularAutomata();
#ifdef __OPENCV_ALL_HPP__
	void drawSituation(unsigned int situation);
	void drawSituation(unsigned int situation, cv::Point upperLeftCorner, cv::Mat & situationMat);
	void drawRule(unsigned int rule);
	void saveCellularAutomata(std::string filename);
#endif

private:
	std::vector<int> algorithm__;

	int numberOfStates__;
	std::vector<std::vector<unsigned int> > cellState__;
	unsigned int cellGroupSize__;
	int score__;
	int mostCommonState__;

#ifdef __OPENCV_ALL_HPP__
	cv::Mat image__;
	unsigned int cellLength__ = 8;
#endif
};

#endif // CELLULAR_AUTOMATON_HPP