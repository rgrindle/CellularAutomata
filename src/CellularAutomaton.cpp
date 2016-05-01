#include "CellularAutomaton.hpp"

//=============================================================================
//	Constructor/Destructor Functions
//=============================================================================

CellularAutomaton::CellularAutomaton()
{
	//srand((unsigned int)time(0));
}

CellularAutomaton::CellularAutomaton(unsigned int size, unsigned int numberOfStates, unsigned int algorithm)
	:CellularAutomaton()
{
	this->setCellGroupSize(size);
	this->setNumberOfStates(numberOfStates);
	this->addNewCellGroup();
	this->setAlgorithm(algorithm);
	this->setRandomStates();
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

std::vector<int> CellularAutomaton::getAlgorithm()
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
	if (cellIndex >= cellState__[0].size() || cellIndex < 0) {
		std::cout << std::endl;
	}
	//std::vector<unsigned int> situationString(3);
	int situation = 0;

	// check if cell to the right exists
	if (cellIndex + 1 < this->cellState__[0].size()) {
		// get state of cell to the right
		//situationString[0] = this->getPrevState(cellIndex + 1);
		situation |= 1 & this->getPrevState(cellIndex + 1);
	}
	else {	// rap around if last cell
			// get leftmost cell state
		//situationString[0] = this->getPrevState(0);
		situation |= 1 & this->getPrevState(0);
	}

	// get state of itself
	//situationString[1] = this->getPrevState(cellIndex);
	situation |= 2 & this->getPrevState(cellIndex);

	// check if cell to the left exists
	if (cellIndex - 1 >= 0) {
		// get state of cell to the left
		//situationString[2] = this->getPrevState(cellIndex - 1);
		situation |= 4 & this->getPrevState(cellIndex - 1);
	}
	else {	// rap around if first cell
			// get rightmost cell state
		//situationString[2] = this->getPrevState(this->cellState__[0].size() - 1);
		situation |= 4 & this->getPrevState(this->cellState__[0].size() - 1);
	}

	// convert to number
	//return this->cvtStringToNumber(situationString);
	return situation;
}

// Situation acts as the index to algorithm.
// The number stored at that location is the command.
int CellularAutomaton::getCommand(unsigned int situation)
{
	//if (situation > pow(this->numberOfStates__, 3) - 1) {
	//	std::cout << std::endl;
	//}
	//if (situation > algorithm__.size() - 1) {
	//	std::cout << std::endl;
	//}
	return this->algorithm__[situation];
}

int CellularAutomaton::getScore()
{
	return this->score__;
}

//=============================================================================
//	Set Functions
//=============================================================================

void CellularAutomaton::setCellGroupSize(unsigned int size)
{
	this->cellGroupSize__ = size;

#ifdef __OPENCV_ALL_HPP__
	this->image__ = cv::Mat(this->cellLength__ * 2 * size, this->cellLength__*size, CV_8UC1, 125);
#endif
}

void CellularAutomaton::setNumberOfStates(unsigned int numberOfStates)
{
	this->numberOfStates__ = numberOfStates;
	this->algorithm__.resize(pow(numberOfStates__, 3));
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

	std::cout << "Binary Algorithm/Rule = ";
	for (exp = algVector.size()-1; exp >= 0; --exp)
		std::cout << algVector[exp];

	std::cout << std::endl;
	this->setAlgorithm(algVector);
}

void CellularAutomaton::setRandomAlgorithm()
{
	std::default_random_engine generator;
	std::random_device randomSeedGenerator;
	std::uniform_int_distribution<int> dist(0, 10000);
	uint32_t seed_val = dist(randomSeedGenerator);
	generator.seed(seed_val);
	std::uniform_int_distribution<int> randomCellState(0, this->numberOfStates__-1);
	
	unsigned int i;
	for (i = 0; i < this->algorithm__.size(); ++i) {
		this->algorithm__[i] = randomCellState(generator);
	}
}

void CellularAutomaton::setRandomStates()
{

	std::default_random_engine generator;
	std::random_device randomSeedGenerator;
	std::uniform_int_distribution<int> dist(0, 10000);
	uint32_t seed_val = dist(randomSeedGenerator);
	generator.seed(seed_val);
	std::uniform_int_distribution<int> randomCellState(0, this->numberOfStates__ - 1);

	std::vector<int> stateCounts(this->numberOfStates__);

	for each(int count in stateCounts) {
		count = 0;
	}

	unsigned int i;
	for (i = 0; i < this->cellState__[0].size(); ++i) {
		int state = randomCellState(generator);
		this->cellState__.back()[i] = state;
		++stateCounts[state];
	}

	// if there are three this won't work
	if(stateCounts[0] > stateCounts[1]) {
		this->mostCommonState__ = 0;
	}
	else if (stateCounts[0] < stateCounts[1]) {
		this->mostCommonState__ = 1;
	}
	else {
		setRandomStates();	// do it over if the number of elements is equal
	}

	for (i = 0; i < this->numberOfStates__; ++i) {
		std::cout << "# of Initial State " + std::to_string(i) + "'s = " << stateCounts[i] << std::endl;
	}
	std::cout << "Most Common State = " << this->mostCommonState__ << std::endl;
	this->drawCellularAutomata();	// draw current states
}

void CellularAutomaton::changeScore(int changeFactor)
{
	this->score__ += changeFactor;
}

void CellularAutomaton::setScore(int score)
{
	this->score__ = score;
}

void CellularAutomaton::reset()
{
	this->cellState__.erase(cellState__.begin()+1, cellState__.end());
	this->setRandomStates();
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
// If so, true.
bool CellularAutomaton::isFinalState()
{
	unsigned int i;
	for (i = 0; i < this->cellState__.size()-1; ++i) {
		if (cellState__.back() == cellState__[i]) {
			//std::cout << "Match found at index " << i << std::endl;
			return true;
		}
	}

	return false;
}

// Check if last two rows match. Or size limit reached lenght = 2*width
// If so, true.
bool CellularAutomaton::isFinalStateQuick()
{
	int index = this->cellState__.size() - 1;

	if(index >= 0 && index-1 >= 0)	{
		if (cellState__[index] == cellState__[index-1]	|| this->cellState__.size() > 2*this->cellState__[0].size()) {
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

// The command is the state to which the cell will be set.
void CellularAutomaton::performCommand(int cellIndex, int command)
{
	//if (cellIndex > cellGroupSize__ || cellIndex < 0) {
	//	std::cout << cellIndex << ' ' << command << std::endl;
	//}
	this->setState(cellIndex, command);
}

// Build the next row of cell states.
void CellularAutomaton::iterateCellGroup()
{
	//if (this->isFinalState()) {	// check if the current automaton is finished.
	//	return;	// Skip finished automaton.
	//}
	this->addNewCellGroup();	// Add a new row of cell states before trying to edit it.

	// Loop through each cell state in the current row to create the next row.
	unsigned int j;
	for (j = 0; j < this->cellState__[0].size(); ++j) {
		unsigned int situation = this->getSituationCode(j);	// get the situation
		int command = this->getCommand(situation);	// use the situation to get the command
		performCommand(j, command);	// perform the command
	}
}

bool CellularAutomaton::isCorrect()
{
	std::vector<unsigned int>::iterator it;
	for (it = cellState__.back().begin(); it < cellState__.back().end(); ++it) {
		if (*it != this->mostCommonState__) {
			return false;
		}
	}
	return true;
}
//=============================================================================
//	OpenCV Dependent Functions
//=============================================================================

#ifdef __OPENCV_ALL_HPP__

void CellularAutomaton::drawCellularAutomata()
{
	int i, x;
	int y = (this->cellState__.size()-1)*(this->cellLength__);

	for (i = 0, x = 0; i < this->cellState__.back().size(); ++i, x += this->cellLength__) {
		if (this->cellState__.back()[i]) {
			// make white rectangle
			cv::rectangle(this->image__, cv::Rect(cv::Point(x, y), cv::Point(x + this->cellLength__, y + this->cellLength__)),255,CV_FILLED);
			//std::cout << this->cellState__.size() << ' ' << x << ' ' << y << ' ' << x + cellLength__ << ' ' << y + cellLength__ << std::endl;
		}
		else {
			// Nothing. Black by defalt
			cv::rectangle(this->image__, cv::Rect(cv::Point(x, y), cv::Point(x + this->cellLength__, y + this->cellLength__)), 0, CV_FILLED);
		}
		// outline grids regardless of cell state
		cv::rectangle(this->image__, cv::Rect(cv::Point(x, y), cv::Point(x + this->cellLength__, y + this->cellLength__)), 125);
	}
	cv::imshow("Cellular Automaton", this->image__);
	cv::waitKey(1);
}


void CellularAutomaton::drawSituation(unsigned int situation)
{
	int temp = this->cellLength__;
	this->cellLength__ = 25;

	cv::Mat situationMat = cv::Mat::zeros(this->cellLength__, this->cellLength__*3, CV_8UC1);

	if (situation & 4) {
		int x = 0;
		cv::rectangle(situationMat, cv::Rect(cv::Point(x, 0), cv::Point(x + this->cellLength__, this->cellLength__)), 255, CV_FILLED);
	}
	if (situation & 2) {
		int x = this->cellLength__;
		cv::rectangle(situationMat, cv::Rect(cv::Point(x, 0), cv::Point(x + this->cellLength__, this->cellLength__)), 255, CV_FILLED);
	}
	if (situation & 1) {
		int x = this->cellLength__ * 2;
		cv::rectangle(situationMat, cv::Rect(cv::Point(x, 0), cv::Point(x + this->cellLength__, this->cellLength__)), 255, CV_FILLED);
	}

	// outline grids regardless of cell state
	int x;
	for (x = 0; x < 3 * this->cellLength__; x += this->cellLength__) {
		cv::rectangle(situationMat, cv::Rect(cv::Point(x, 0), cv::Point(x + this->cellLength__, this->cellLength__)), 125);
		cv::rectangle(situationMat, cv::Rect(cv::Point(x, 0), cv::Point(x + this->cellLength__, this->cellLength__)), 125);
		cv::rectangle(situationMat, cv::Rect(cv::Point(x, 0), cv::Point(x + this->cellLength__, this->cellLength__)), 125);
	}

	cv::imwrite("../data/situations/situation" + std::to_string(situation) + ".jpg", situationMat);
	cv::imshow("Situation "+std::to_string(situation),situationMat);
	cv::waitKey(1);
	this->cellLength__ = temp;
}

void CellularAutomaton::drawSituation(unsigned int situation, cv::Point upperLeftCorner, cv::Mat & situationMat)
{
	int x = 0;

	if (situation & 4) {
		cv::rectangle(situationMat, cv::Rect(cv::Point(x, 0) + upperLeftCorner, cv::Point(x + this->cellLength__, this->cellLength__) + upperLeftCorner), 255, CV_FILLED);
	}
	else {
		cv::rectangle(situationMat, cv::Rect(cv::Point(x, 0) + upperLeftCorner, cv::Point(x + this->cellLength__, this->cellLength__) + upperLeftCorner), 0, CV_FILLED);
	}

	x = this->cellLength__;

	if (situation & 2) {
		cv::rectangle(situationMat, cv::Rect(cv::Point(x, 0) + upperLeftCorner, cv::Point(x + this->cellLength__, this->cellLength__) + upperLeftCorner), 255, CV_FILLED);
	}
	else {
		cv::rectangle(situationMat, cv::Rect(cv::Point(x, 0) + upperLeftCorner, cv::Point(x + this->cellLength__, this->cellLength__) + upperLeftCorner), 0, CV_FILLED);
	}

	x = this->cellLength__ * 2;

	if (situation & 1) {
		cv::rectangle(situationMat, cv::Rect(cv::Point(x, 0) + upperLeftCorner, cv::Point(x + this->cellLength__, this->cellLength__) + upperLeftCorner), 255, CV_FILLED);
	}
	else {
		cv::rectangle(situationMat, cv::Rect(cv::Point(x, 0) + upperLeftCorner, cv::Point(x + this->cellLength__, this->cellLength__) + upperLeftCorner), 0, CV_FILLED);
	}

	// outline grids regardless of cell state
	for (x = 0; x < 3 * this->cellLength__; x += this->cellLength__) {
		cv::rectangle(situationMat, cv::Rect(cv::Point(x, 0) + upperLeftCorner, cv::Point(x + this->cellLength__, this->cellLength__) + upperLeftCorner), 125);
		cv::rectangle(situationMat, cv::Rect(cv::Point(x, 0) + upperLeftCorner, cv::Point(x + this->cellLength__, this->cellLength__) + upperLeftCorner), 125);
		cv::rectangle(situationMat, cv::Rect(cv::Point(x, 0) + upperLeftCorner, cv::Point(x + this->cellLength__, this->cellLength__) + upperLeftCorner), 125);
	}
}

void CellularAutomaton::drawRule(unsigned int rule)
{
	int temp = this->cellLength__;
	this->cellLength__ = 25;

	cv::Mat ruleMat(this->cellLength__ * 6, this->cellLength__ * 33, CV_8UC1, 255);

	int situation = 0;
	int rulebit = 128;
	int y = 1;
	int x;
	for (x = 1; x <= 29; x += 4) {
		if (situation < 8) {
			this->drawSituation(situation, cv::Point(x*this->cellLength__, this->cellLength__*y), ruleMat);

			int color = 125;
			cv::Point pi((x + 1.5)*this->cellLength__, (y + 1.5)*this->cellLength__);
			cv::Point pf((x + 1.5)*this->cellLength__, (y + 2.5)*this->cellLength__);
			cv::line(ruleMat, pi, pf, color, 2);

			double angle = atan2(pf.y - pi.y, pf.x - pi.x);

			cv::Point arrow;
			arrow.x = pf.x + 9 * cos(angle + 3.0*3.14159 / 4.0);
			arrow.y = pf.y + 9 * sin(angle + 3.0*3.14159 / 4.0);
			cv::line(ruleMat, arrow, pf, color, 2);

			arrow.x = pf.x + 9 * cos(angle - 3.0*3.14159 / 4.0);
			arrow.y = pf.y + 9 * sin(angle - 3.0*3.14159 / 4.0);
			cv::line(ruleMat, arrow, pf, color, 2);

			if (rule & rulebit) {
				cv::rectangle(ruleMat, cv::Rect((x + 1)*this->cellLength__, (y + 3)*this->cellLength__, this->cellLength__, this->cellLength__), 255, CV_FILLED);
			}
			else {
				cv::rectangle(ruleMat, cv::Rect((x + 1)*this->cellLength__, (y + 3)*this->cellLength__, this->cellLength__, this->cellLength__), 0, CV_FILLED);
			}
			cv::rectangle(ruleMat, cv::Rect((x + 1)*this->cellLength__, (y + 3)*this->cellLength__, this->cellLength__, this->cellLength__), 125);
			rulebit = rulebit >> 1;
			++situation;
		}
	}
	cv::imwrite("../../data/rules/rule" + std::to_string(rule) + ".jpg", ruleMat);
	cv::imshow("Rule " + std::to_string(rule), ruleMat);

	this->cellLength__ = temp;
}

void CellularAutomaton::saveCellularAutomata(std::string filename)
{
	cv::imwrite(filename, this->image__);
}
#else
void CellularAutomaton::drawCellularAutomata()
{
	this->printCellState();
}
#endif