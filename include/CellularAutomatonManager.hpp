#ifndef CELLULAR_AUTOMATON_MANAGER_HPP
#define CELLULAR_AUTOMATON_MANAGER_HPP

#include "CellularAutomaton.hpp"
#include "Pair.hpp"

#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <string>
#include <chrono>

// define point system and possible commands go here

bool compScore(CellularAutomaton r1, CellularAutomaton r2);	// used to sort individuals by score

class CellularAutomatonManager
{
public:
	CellularAutomatonManager(unsigned int numberOfCellGroups, unsigned int numberOfCells, unsigned int numberOfStates);
	~CellularAutomatonManager();

	// get functions
	int getMax();
	int getMin();
	double getAverage();
	double getMedianScore();

	// set functions
	void changeScore(int ObjectIndex, int changeFactor);
	void changeScore(std::vector<CellularAutomaton>::iterator objectIterator, int changeFactor);
	
	// other functions
	bool isDone();	// check if done
	//void iterateCellGroup();
	void printCellGroup(unsigned int index);
	void rank();
	void mate();
	void saveAlgorithmList();
	void printScore();
	void simulateGeneration(int seasons);

	std::ofstream scoreOut;
	std::ofstream algorithmOut;

private:
	std::vector<CellularAutomaton> cellGroup__;
	bool done__ = false;
	double avg__;
	int min__;
	int max__;
	double median__;
	int generation__;
};

#endif // CELLULAR_AUTOMATON_MANAGER_HPP