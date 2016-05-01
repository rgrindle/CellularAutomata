#include "CellularAutomatonManager.hpp"

//=============================================================================
//	Constructor/Desctructor Functions
//=============================================================================

CellularAutomatonManager::CellularAutomatonManager(unsigned int numberOfCellGroups, unsigned int numberOfCells, unsigned int numberOfStates)
{

	scoreOut.open("../data/score/score_data.csv");

	scoreOut << "Generation," << "Minimum," << "Average," << "Median," << "Maximum,";

	this->generation__ = 0;

	// set the population size
	this->cellGroup__.resize(numberOfCellGroups);

	// set the width of the automaton
	std::vector<CellularAutomaton>::iterator it;
	for (it = this->cellGroup__.begin(); it < this->cellGroup__.end(); ++it) {
		it->setCellGroupSize(numberOfCells);
		it->setNumberOfStates(numberOfStates);
		it->setRandomAlgorithm();
		it->addNewCellGroup();
		it->setRandomStates();
	}
}

CellularAutomatonManager::~CellularAutomatonManager()
{
	scoreOut.close();
	algorithmOut.close();
}

//=============================================================================
//	Get Functions
//=============================================================================

int CellularAutomatonManager::getMax()
{
	return this->max__;
}

int CellularAutomatonManager::getMin()
{
	return this->min__;
}

double CellularAutomatonManager::getAverage()
{
	return this->avg__;
}

double CellularAutomatonManager::getMedianScore()
{
	double median;
	int size = this->cellGroup__.size();
	// assuming that data is already sorted

	if (size % 2 == 0)
	{
		median = (this->cellGroup__[size / 2 - 1].getScore() + this->cellGroup__[size / 2].getScore()) / 2;
	}
	else
	{
		median = this->cellGroup__[(int)size / 2].getScore();
	}

	return median;
}

//=============================================================================
//	Set Functions
//=============================================================================

void CellularAutomatonManager::changeScore(int CellularAutomatonIndex, int changeFactor)
{
	this->cellGroup__[CellularAutomatonIndex].changeScore(changeFactor);
}

void CellularAutomatonManager::changeScore(std::vector<CellularAutomaton>::iterator CellularAutomatonIterator, int changeFactor)
{
	CellularAutomatonIterator->changeScore(changeFactor);
}


//=============================================================================
// Other Functions
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

void CellularAutomatonManager::simulateGeneration(int seasons)
{
	auto start = std::chrono::system_clock::now();

	for (std::vector<CellularAutomaton>::iterator it = cellGroup__.begin(); it != cellGroup__.end(); ++it) {
		it->setScore(0);
		int i;
		for (i = 0; i < seasons; ++i) {
			while(!(it->isFinalStateQuick())) {
				it->iterateCellGroup();
				//if (it->isCorrect()) {
				//	//it->changeScore(1);
				//}
			}
			if (it->isCorrect()) {
				//std::cout << "correct" << std::endl;
				it->drawCellularAutomata();
//				system("pause");
				it->changeScore(1);
			}
			it->reset();
		}
		it->setScore(100.0*(double)it->getScore() / (double)seasons);
	}

	auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - start);
	std::cout << duration.count() << " ms" << '\t';
}

void CellularAutomatonManager::saveAlgorithmList()
{
	std::vector<CellularAutomaton>::iterator CellularAutomatonIterator;

	std::ostringstream generationNumberString;
	generationNumberString << this->generation__;

	algorithmOut.open("../data/algorithm/generation" + generationNumberString.str() + ".csv", std::ofstream::out);


	for (CellularAutomatonIterator = cellGroup__.begin(); CellularAutomatonIterator != cellGroup__.end(); ++CellularAutomatonIterator) {
		std::vector<int> al = CellularAutomatonIterator->getAlgorithm();

		std::vector<int>::iterator it;


		for (it = al.begin(); it != al.end(); ++it) {
			algorithmOut << *it << ',';
		}
		algorithmOut << std::endl;
	}
	algorithmOut.close();

}

void CellularAutomatonManager::rank()
{
	std::vector<CellularAutomaton>::iterator it;

	// init min and max opposite 
	//int minimum = 100;	// set this to max
	//int maximum = 0;	// set this to min

	int sum = 0;
	int counter = 0;
	int score;

	// organize CellularAutomata from max to min score
	std::sort(cellGroup__.begin(), cellGroup__.end(), compScore);

	for (it = cellGroup__.begin(); it != cellGroup__.end(); ++it, ++counter) {
		score = it->getScore();

		//if (score > maximum) {
		//	maximum = score;
		//}

		//if (score < minimum) {
		//	minimum = score;
		//}
		sum += score;
	}
	this->avg__ = (double)sum / counter;
	this->min__ = this->cellGroup__.back().getScore();
	this->max__ = this->cellGroup__[0].getScore();
	this->median__ = this->getMedianScore();
}

void CellularAutomatonManager::printScore()
{
	std::cout << " for Generation " << this->generation__ << '\t' << this->min__ << '\t' << this->avg__ << '\t' << this->median__ << '\t' << this->max__ << std::endl;

	scoreOut << std::endl;
	scoreOut << this->generation__ << ',';
	scoreOut << this->min__ << ',';
	scoreOut << this->avg__ << ',';
	scoreOut << this->median__ << ',';
	scoreOut << this->max__ << ',';

	++generation__;
}

bool compScore(CellularAutomaton r1, CellularAutomaton r2)
{
	return (r1.getScore() > r2.getScore());
}

void CellularAutomatonManager::mate()
{
	std::vector<Pair> pairs;
	int start;
	int sum;
	int numberOfMates = 10;	// this is the number of mates for best individual only

	// initialize random stuff here to avoid doing it multiple times.
	std::default_random_engine generator;
	std::random_device randomSeedGenerator;
	std::uniform_int_distribution<int> dist(0, 10000);
	uint32_t seed_val = dist(randomSeedGenerator);
	generator.seed(seed_val);
	std::uniform_int_distribution<int> distributionChoose(0, 100);
	std::uniform_int_distribution<int> distributionMutate(0, 1);

	// figure out who is going to have babies together and how many
	for (start = 0, sum = 0; sum < this->cellGroup__.size(); ++start) {

		int j, babies;
		for (j = 0, babies = 16; j < numberOfMates; ++j) {
			sum += babies;

			if (sum > this->cellGroup__.size()) {
				babies = (int)this->cellGroup__.size() - (sum - babies);
			}

			pairs.push_back(Pair());
			pairs.back().firstIndex = start;
			pairs.back().secondIndex = start + j + 1;
			pairs.back().numberOfBabies = babies;

			if (babies != 2) {
				babies /= 2;
			}

			if (sum >= this->cellGroup__.size()) {
				break;
			}
		}

		if (numberOfMates != 1) {
			--numberOfMates;
		}
	}

	// make the babies
	std::vector<CellularAutomaton> tmp(this->cellGroup__.size(), CellularAutomaton());
	std::vector<CellularAutomaton>::iterator tmpIt;
	std::vector<int> newAlg(this->cellGroup__[0].getAlgorithm().size());
	std::vector<int> alg1;
	std::vector<int> alg2;
	int choose;
	int babies;
	std::vector<Pair>::iterator pairIt;

	for (tmpIt = tmp.begin(), pairIt = pairs.begin(); pairIt < pairs.end(); ++pairIt) {
		alg1 = this->cellGroup__[pairIt->firstIndex].getAlgorithm();
		alg2 = this->cellGroup__[pairIt->secondIndex].getAlgorithm();

		for (babies = pairIt->numberOfBabies; babies > 0, tmpIt < tmp.end(); --babies, ++tmpIt) {

			int i;
			for (i = 0; i < newAlg.size(); ++i) {
				choose = distributionChoose(generator);	// random number between 1 and 100
				// mutate at rate of 2%
				if (choose > 51) {
					newAlg[i] = alg1[i];	// mom's gene
				}
				else if (choose > 2) {
					newAlg[i] = alg2[i];	// dad's gene
				}
				else {
					newAlg[i] = distributionMutate(generator);	// mutated gene is random
				}
			}
			tmpIt->setAlgorithm(newAlg);
		}
	}
	std::vector<CellularAutomaton>::iterator cellIt;

	// set the babies that were made into the new generations and reset score to 0
	for (cellIt = this->cellGroup__.begin(), tmpIt = tmp.begin(); cellIt != this->cellGroup__.end(); ++cellIt, ++tmpIt) {
		cellIt->setAlgorithm(tmpIt->getAlgorithm());
		cellIt->setScore(0);
	}
}