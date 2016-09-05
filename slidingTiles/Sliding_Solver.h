#include "Board_Tile.h"

#ifndef SLIDING_SOLVER_H
#define SLIDING_SOLVER_H

//We create a structure to compare the total D(C) value of each config
//so that we can heapify accordingly.
struct compareBoardTiles
{
	bool operator() (const Board_Tile & D1, const  Board_Tile & D2)
	{
		return D1 < D2;
	}
};

class Sliding_Solver
{
private:
	Board_Tile * initialConfig;
	Board_Tile * goalConfig;

public:
	//Constructor (as specified)
	Sliding_Solver(const std::string&);
	
	//A* search
	bool Solve_Puzzle(Board_Tile*);

	//Getter to see the Goal 
	Board_Tile * getGoalConfig();

	//Setter for the Goal 
	void setGoalConfig(const std::string&);

	//A check to see if the user has supplied their own goal config
	//or if the default will be used.
	bool goalConfigWasSetByUser;
	
	//A method to print everything nicely at the end of A* search.	
	void printSolution(const Board_Tile&);
};

#endif