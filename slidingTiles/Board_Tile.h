#include <iostream>
#include <vector>   
#include <string>   
#include <cmath>    
#include <stack>    
#include <list>
#include <iterator>
#include <sstream>
#include <queue>
#include <iostream>
#include <iomanip>
#include <assert.h>
#include <memory>

#ifndef BOARD_TILE_H
#define BOARD_TILE_H

class Board_Tile
{
private:
	
	//Necessary by project requirements
	int **twoDimensionalArray;
	std::string config;
	std::string moves_from_start;

	//Other useful members
	Board_Tile * goal;
	int manhattanDistancePlusNumberOfMoves;

public:
	//Default Constructor
	Board_Tile();
	//Constructor
	Board_Tile(const std::string&);
	//Third Constructor which sets up everything
	Board_Tile(std::string, const Board_Tile &, const std::string);

	//Copy Constructor
	Board_Tile(const Board_Tile&);

	//Destructor
	~Board_Tile();

	//Operators
	bool operator==(const Board_Tile &);
	Board_Tile& operator=(const Board_Tile &);
	bool operator<(const Board_Tile&) const;

	//Necessary by project requirements
	std::list<Board_Tile> nextConfigs() const;
	int numMoves() const;
	int Manhattan_Distance(const Board_Tile&) const;

	//Other useful functions:
	//Getters
	int getValueByRowAndColumn(int, int) const;
	std::string getConfig() const;
	std::string getMovesFromStart() const;
	Board_Tile* getGoalConfig() const;
	int getManhattanDistancePlusNumberOfMoves() const;
	bool rowAndColOfValue(int, std::vector<int>&) const;

	//setters
	void setGoalConfig(Board_Tile*);
	void setManhattanDistancePlusNumberOfMoves(int);
	
	//Modifiers
	void appendToMoves(std::string);
	void assign(int, int, int);

	//Printer
	void printConfig() const;	
	bool checkConfigString(std::string);
};
#endif