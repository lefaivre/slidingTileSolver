#include "Sliding_Solver.h"

//Set the threshold for solving problems.
unsigned int g_maximumSizeForHeapAndVistedList = 10000;

//Specified constructor.
Sliding_Solver::Sliding_Solver(const std::string & inputString)
{
	//setup the starting config
	initialConfig = new Board_Tile(inputString);

	//Supply an end config if the user doesn't
	//This will ultimately be displayed in the output
	if (goalConfig == nullptr)
	{
		goalConfig = new Board_Tile("123456780");
		goalConfigWasSetByUser = false;
	}
}


//Supply a function to set the goal configuration of the Sliding_Solver object
void Sliding_Solver::setGoalConfig(const std::string & goalString)
{
	goalConfig = new Board_Tile(goalString);
	goalConfigWasSetByUser = true;
}

//This is essentially where we perform our A* search,
//the return type is a bool, so that we can set a threshold of how hard
//of a problem we want let the A* search try and solve.
bool Sliding_Solver::Solve_Puzzle(Board_Tile* goalConfigIn)
{
	if (initialConfig == nullptr ||
		goalConfig == nullptr ||
		goalConfigIn == nullptr)
	{
		std::cout << "Null Config was found. Check start & end configurations.\n";
		std::cout << "Exiting Solve_Puzzle.\n";
		return false;
	} 

	//Declare list for all of the used configurations and for all
	//of the possible new configurations.
	std::vector<Board_Tile> closedList;
	std::list<Board_Tile> nextConfigsList;

	//Declare flags
	bool isInClosedList = false;
	bool isInOpenHeap = false;

	//Declare what is needed for the minheap.
	std::vector<Board_Tile> openHeap;
	openHeap.push_back(*initialConfig);

	//Set the initial states goal config as the pointer goal config.
	//This unltimately allows the user to specifies any goal they want.
	openHeap.front().setGoalConfig(goalConfig);
	
	//We set the D(C) value of the initial state, so that
	//the next configs will have appropriate values for their total value.
	openHeap.front().setManhattanDistancePlusNumberOfMoves
		(openHeap.front().Manhattan_Distance(*goalConfig)
		+ openHeap.front().numMoves());

	while (!openHeap.empty())
	{
		//Check to see if the solution is found within the threshold.
		if ((openHeap.size() > g_maximumSizeForHeapAndVistedList)
			|| (closedList.size() > g_maximumSizeForHeapAndVistedList))
		{
			std::cout << "Solve_Puzzle was exited. ";
			std::cout << "The given problem has either no solution, ";
			std::cout << "or is exceptionally hard to solve.\n\n";
			return false;
		}

		//Since we are using a min heap, the configuration
		//with the smallest D(C) value will be chosen.
		std::make_heap(openHeap.begin(), openHeap.end(), compareBoardTiles());
		Board_Tile currentConfig = openHeap.front();

		if (currentConfig == *goalConfig)
		{
			printSolution(currentConfig);
			openHeap.clear();
			closedList.clear();
			nextConfigsList.clear();
			return true;
		}

		//If the current config is not the goal then get Successor states
		nextConfigsList = currentConfig.nextConfigs();

		openHeap.erase(openHeap.begin());
		std::make_heap(openHeap.begin(), openHeap.end(), compareBoardTiles());

		//Remove the root of the openHeap, and put it in the closed list
		//(Since we have now visited that configuration).
		closedList.push_back(currentConfig);

		//For each successor check if the configuration is different from all those
		//in the closed list and the openHeap.
		for (std::list<Board_Tile>::iterator nextConfigIt = nextConfigsList.begin();
			nextConfigIt != nextConfigsList.end(); ++nextConfigIt)
		{
			//Closed List check
			for (unsigned int i = 0; i < closedList.size(); i++)
			{
				if ((*nextConfigIt) == closedList.at(i))
				{
					isInClosedList = true;
					break;
				}
			}
			if (!isInClosedList)
			{
				//Open List check
				for (unsigned int i = 0; i < openHeap.size(); i++)
				{
					//First check if we are repeating a state (just in case)
					//This should always pass, but in the event that it doesn't
					//notify the user then exit the program.
					if (!(*nextConfigIt == currentConfig))
					{
						if (openHeap.at(i) == (*nextConfigIt))
						{
							//Compare the heuristics, if the config is in the open 
							//heap but has a worse heuristic, then replace it

							if (nextConfigIt->getManhattanDistancePlusNumberOfMoves()
								< openHeap.at(i).getManhattanDistancePlusNumberOfMoves())
							{
								//replace the config in the open heap with the next config.
								openHeap.at(i) = *nextConfigIt;
								std::make_heap(openHeap.begin(), openHeap.end(), compareBoardTiles());
							}

							isInOpenHeap = true;
						}
					}
					//Note that this else should always pass, but if somehow
					//the next config retrieved was theexact same as it's parent 
					//then we must return false.
					else
					{
						std::cout << "Next config retrieved same as parent config.\n";
						std::cout << "exiting Solve_Puzzle.\n";
						return false;
					}
				}
				//Finally if not in open heap, and not in closed list push back.
				if (!isInOpenHeap)
				{
					openHeap.push_back(*nextConfigIt);
					std::make_heap(openHeap.begin(), openHeap.end(), compareBoardTiles());
				}
			}
			//Reset flags (they need to be reset no matter what)
			isInOpenHeap = false;
			isInClosedList = false;
		}
		//clear our next configs list, so that we don't
		//reuse the same configs every time.
		nextConfigsList.clear();
	}
	openHeap.clear();
	closedList.clear();
	nextConfigsList.clear();
	return false;
}

//We define a nice printer as specified.
void Sliding_Solver::printSolution(const Board_Tile& inputConfig)
{
	std::cout
		<< std::internal
		<< initialConfig->getConfig()
		<< std::setw(16)
		<< inputConfig.getConfig();
	if (!this->goalConfigWasSetByUser)
	{
		std::cout
			<< " (defaultWasAssumed)"
			<< std::setw(7);
	} else
	{
		std::cout
			<< std::setw(11);
	}
	std::cout
		<< inputConfig.numMoves()
		<< "                  "
		<< inputConfig.getMovesFromStart()
		<< std::left;
}

//Just in case the user needs to check what was used for a goal configuration.
Board_Tile * Sliding_Solver::getGoalConfig()
{
	return goalConfig;
}