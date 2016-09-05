#include "Sliding_Solver.h"

int main()
{

	std::string configString;
	std::vector<std::string> configs;

	std::vector<Sliding_Solver> toSolveVector;
	std::vector<Board_Tile> startingVector;
	
	int numberOfPuzzlesToSolve = 0;
	std::cout << "Please enter the number of puzzles that you want to solve: ";
	std::cin >> numberOfPuzzlesToSolve;
	std::cout << std::endl;

	for (int i = 0; i < numberOfPuzzlesToSolve; i++)
	{
		std::cout << "Please enter puzzle number " << i << " start configuration: ";
		std::cin >> configString;
		Board_Tile startingBoard(configString);
		
		Sliding_Solver solveConfig(configString);
		std::cout << std::endl;

		std::cout << "Please enter puzzle number " << i << " end configuration: ";
		std::cin >> configString;
	
		solveConfig.setGoalConfig(configString);
		startingVector.push_back(startingBoard);
		toSolveVector.push_back(solveConfig);
		
		std::cout << std::endl;
	}

	std::cout << "\n\n";
	std::cout << "Solving Puzzles: \n\n";
	std::cout << "Start Board" 
		<< std::setw(15) 
		<< "Goal Board" 
		<< std::setw(20) 
		<< "Number of Moves" 
		<< std::setw(15)
		<< "Solution";
	std::cout << std::endl;
	std::cout << std::endl;

	for (unsigned int i = 0; i < toSolveVector.size(); i++)
	{
		if (!(toSolveVector.at(i).Solve_Puzzle(&startingVector.at(i))))
		{
			std::cout << "Sorry, unable to solve puzzle";
		}
		std::cout << "\n\n";
	}


	return 0;
}
