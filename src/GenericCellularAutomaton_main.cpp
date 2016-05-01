#include "CellularAutomaton.hpp"
#include "CellularAutomatonManager.hpp"

#include <algorithm>

//====================================================================================================================
//	Cmd line parsing found at http://stackoverflow.com/questions/865668/how-to-parse-command-line-arguments-in-c
//====================================================================================================================
char* getCmdOption(char ** begin, char ** end, const std::string & option)
{
	char ** itr = std::find(begin, end, option);
	if (itr != end && ++itr != end)
	{
		return *itr;
	}
	return 0;
}

bool cmdOptionExists(char** begin, char** end, const std::string& option)
{
	return std::find(begin, end, option) != end;
}

int cmdParsing(int argc, char * argv[], int &numberOfCells, int &rule, int &numberOfStates)
{
	if (argc == 1) {
		std::cout << "Not cmd line arguments. Use -help for more details." << std::endl;
		return -1;
	}

	if (cmdOptionExists(argv, argv + argc, "-help")) {
		std::cout << "Required:" << std::endl;
		std::cout << '\t' << "-cells" << '\t' << "Set number of cells (columns)." << std::endl;
		std::cout << '\t' << "-rule" << '\t' << "Set rule [0, 256]" << std::endl;
		std::cout << std::endl;

		std::cout << "Optional:" << std::endl;
		std::cout << '\t' << "-states" << '\t' << "Set number of states. Default is 2." << std::endl;
		std::cout << '\t' << "-ga" << '\t' << "If used a genetic algorithm will be executed otherwise only a single cellular automaton. The genetic algorithm doesn't currently work." << std::endl;
		std::cout << std::endl;

		std::cout << "Special Optional (Requires OpenCV):" << std::endl;
		std::cout << '\t' << "-situation" << '\t' << "Draws situation. If situation is out of range, draws all situations. Situations are saved to ../data/situations/" << std::endl;
		std::cout << '\t' << "-drawrule" << '\t' << "Draws rule. If rule is out of range, draws all rules. Rules are saved to ../data/rules/" << std::endl;
		std::cout << std::endl;

		return -1;
	}

#ifdef __OPENCV_ALL_HPP__
	if (cmdOptionExists(argv, argv + argc, "-situation")) {
		int situation = std::stoi(getCmdOption(argv, argv + argc, "-situation"));

		CellularAutomaton CA;

		std::cout << "Press 'q' to quit." << std::endl;
		char exit_key_press = 0;
		while (exit_key_press != 'q') {	// or key != ESC
			if (situation < 0 || situation > 7) {	// this only works for 2 states
				std::cout << "Situation requested is out of range. Drawing all situation." << std::endl;
				for (int i = 0; i < 8; ++i) {
					CA.drawSituation(i);
				}
			}
			else {
				CA.drawSituation(situation);
			}
			exit_key_press = cv::waitKey(10);
		}

		return -1;
	}

	if (cmdOptionExists(argv, argv + argc, "-drawrule")) {
		int rule = std::stoi(getCmdOption(argv, argv + argc, "-drawrule"));

		CellularAutomaton CA;

		std::cout << "Press 'q' to quit." << std::endl;
		char exit_key_press = 0;
		while (exit_key_press != 'q') {	// or key != ESC
			if (rule < 0 || rule > 256) {	// this only works for 2 states
				std::cout << "Rule requested is out of range. Drawing all rules." << std::endl;
				for (int i = 0; i < 256; ++i) {
					CA.drawRule(i);
				}
			}
			else {
				CA.drawRule(rule);
			}
			exit_key_press = cv::waitKey(10);
		}

		return -1;
	}
#endif

	if (cmdOptionExists(argv, argv + argc, "-cells")) {
		numberOfCells = std::stoi(getCmdOption(argv, argv + argc, "-cells"));
		std::cout << "Number of Cells = " << numberOfCells << std::endl;
	}
	else {
		std::cout << "cmd line option " << "-cells" << " not found." << std::endl;
	}

	if (cmdOptionExists(argv, argv + argc, "-rule")) {
		rule = std::stoi(getCmdOption(argv, argv + argc, "-rule"));
		std::cout << "Rule = " << rule << std::endl;
	}
	else {
		std::cout << "cmd line option " << "-rule" << " not found." << std::endl;
	}

	if (cmdOptionExists(argv, argv + argc, "-states")) {
		numberOfStates = std::stoi(getCmdOption(argv, argv + argc, "-states"));
		std::cout << "Number of States = " << numberOfStates << std::endl;
	}
	else {
		std::cout << "Number of States = 2 by default" << std::endl;
	}

	if (cmdOptionExists(argv, argv + argc, "-ga")) {
		std::cout << "Genetic Algorithm = True" << std::endl;
		return 1;
	}
	else {
		std::cout << "Genetic Algorithm = False" << std::endl;
		return 0;
	}
}

int main(int argc, char * argv[])
{
	int numberOfCells;
	int rule;
	int numberOfStates = 2;

	int next = cmdParsing(argc, argv, numberOfCells, rule, numberOfStates);

	if (next == -1)	{	// help
		return -1;
	}
	else if (next == 0) {	// single cellular automaton
		CellularAutomaton CA(numberOfCells, numberOfStates, rule);	// generate a cellular automaton with a width of 100 cells

		// while there is no repeated row state keep iterating
		while (!CA.isFinalState()) {
			CA.iterateCellGroup();	// iterate to get next row
			CA.drawCellularAutomata();	// draw current states
		}

		std::cout << "Press 'q' to quit." << std::endl;
		char exit_key_press = 0;

#ifdef __ALL_OPENCV_HPP__
		while (exit_key_press != 'q') {	// or key != ESC
			CA.drawCellularAutomata();
			exit_key_press = cv::waitKey(10);
		}
#else
		while (exit_key_press != 'q' && exit_key_press != 'Q') {
			std::cin >> exit_key_press;
		}
#endif
	}
	else if (next == 1)	{	// genetic algorithm
		CellularAutomatonManager manager(numberOfCells, rule, numberOfStates);	// generate 100 cellular automata with a width of 149 cells

		int i;
		// iterator for 1000 generations
		for (i = 0; i < 1000; ++i) {
			manager.simulateGeneration(100);	// 200 moves for each of the 100 seasons. seasons are the number of times 200 moves will be attempted. The score will be average over this many attemps.
			manager.rank();						// rank the individuals by score 
			manager.printScore();				// print the score in the console to give a basic idea of progress
			manager.saveAlgorithmList();		// save all algorithms in a file labeled by generation
			manager.mate();						// create the next generation by combining the algorithms of the best individuals in the current generation
		}
	}
}