#include "Board_Tile.h"

int g_puzzleSize = 3;
int g_totalMovesOfEmptyTile = 4;
int g_maxConfigLength = 9;

enum move
{
	up = 0,
	left = 1,
	right = 2,
	down = 3
};

bool Board_Tile::checkConfigString(std::string configString)
{

	std::sort(configString.begin(), configString.end());
	if (!(std::adjacent_find(configString.begin(), configString.end()) == configString.end()))
	{
		return false;
	}
	else if (configString.length() != g_maxConfigLength)
	{
		return false;
	}
	else if (!(configString.find_first_not_of("012345678") == std::string::npos))
	{
		return false;
	}	
	else return true;
	
}

Board_Tile::Board_Tile(const std::string& configString)
{
	if (!(checkConfigString(configString)))
	{
		std::cout << "improper initial config string entered";
		exit(1);
	}


	config = configString;
	moves_from_start = "";
	manhattanDistancePlusNumberOfMoves = 0;

	std::stack<int> tempStack;
	int configStringAsInt = std::stoi(configString);

	for (unsigned int i = 0; i < configString.size(); i++)
	{
		int tempValue = configStringAsInt % 10;
		tempStack.push(tempValue);
		configStringAsInt /= 10;
	}

	//Create the two dimensional array with the config strings values.
	twoDimensionalArray = new int*[g_puzzleSize];
	for (int i = 0; i < g_puzzleSize; i++)
	{
		twoDimensionalArray[i] = new int[g_puzzleSize];
		for (int j = 0; j < g_puzzleSize; j++)
		{
			twoDimensionalArray[i][j] = tempStack.top();
			tempStack.pop();
		}
	}
}

//Default Constructor
Board_Tile::Board_Tile()
{
	config = "";
	moves_from_start = "";
	manhattanDistancePlusNumberOfMoves = INT_MAX;
}

Board_Tile::Board_Tile(std::string movesFromStart, const Board_Tile & goalConfig, const std::string setupConfig)
{
	
	if (!(strspn(setupConfig.c_str(), "012345678")))
	{
		std::cout << "improper initial config string entered";
		exit(1);
	}
	if (!(strspn(goalConfig.getConfig().c_str(), "012345678")))
	{
		std::cout << "improper initial config string entered";
		exit(1);
	}

	config = setupConfig;
	moves_from_start = movesFromStart;

	std::stack<int> tempStack;
	int configStringAsInt = std::stoi(setupConfig);

	for (unsigned int i = 0; i < setupConfig.size(); i++)
	{
		int tempValue = configStringAsInt % 10;
		tempStack.push(tempValue);
		configStringAsInt /= 10;
	}

	//Create the two dimensional array with the config strings values.
	twoDimensionalArray = new int*[g_puzzleSize];
	for (int i = 0; i < g_puzzleSize; i++)
	{
		twoDimensionalArray[i] = new int[g_puzzleSize];
		for (int j = 0; j < g_puzzleSize; j++)
		{
			twoDimensionalArray[i][j] = tempStack.top();
			tempStack.pop();
		}
	}

	manhattanDistancePlusNumberOfMoves = movesFromStart.length() +
		Manhattan_Distance(goalConfig);
}

//Destructor
Board_Tile::~Board_Tile()
{
	//deleteing memory to prevent memory leak
	for (int i = 0; i < g_puzzleSize; ++i)
		delete[] twoDimensionalArray[i];

	delete[] twoDimensionalArray;
}

// Copy Constructor
Board_Tile::Board_Tile(const Board_Tile & toBeCopied)
{

	if (!(strspn(toBeCopied.getConfig().c_str(), "012345678")))
	{
		std::cout << "improper initial config string entered";
		exit(1);
	}

	twoDimensionalArray = new int*[g_puzzleSize];
	for (int i = 0; i < g_puzzleSize; i++)
	{
		twoDimensionalArray[i] = new int[g_puzzleSize];
		for (int j = 0; j < g_puzzleSize; j++)
		{
			twoDimensionalArray[i][j] = toBeCopied.getValueByRowAndColumn(i, j);
		}
	}

	moves_from_start = toBeCopied.getMovesFromStart();
	config = toBeCopied.getConfig();
	goal = toBeCopied.goal;
	manhattanDistancePlusNumberOfMoves =
		toBeCopied.manhattanDistancePlusNumberOfMoves;
}

//Assignment operator
Board_Tile& Board_Tile::operator=(const Board_Tile &rightPuzzle)
{
	if (!(strspn(rightPuzzle.getConfig().c_str(), "012345678")))
	{
		std::cout << "improper initial config string entered";
		exit(1);
	}

	if (this == &rightPuzzle)
	{
		return *this;
	}

	//twoDimensionalArray = new int*[g_puzzleSize];
	for (int i = 0; i < g_puzzleSize; i++)
	{
		//this->twoDimensionalArray[i] = new int[g_puzzleSize];
		for (int j = 0; j < g_puzzleSize; j++)
		{
			twoDimensionalArray[i][j] = rightPuzzle.getValueByRowAndColumn(i, j);
		}
	}

	moves_from_start = rightPuzzle.getMovesFromStart();
	config = rightPuzzle.getConfig();
	manhattanDistancePlusNumberOfMoves =
		rightPuzzle.manhattanDistancePlusNumberOfMoves;
	goal = rightPuzzle.goal;


	return *this;
}


//Comparison Operator
bool Board_Tile::operator==(const Board_Tile &rightPuzzle)
{
	bool toReturn = true;
	//return (config == rightPuzzle.getConfig());
	for (int i = 0; i < g_puzzleSize; i++)
	{
		for (int j = 0; j < g_puzzleSize; j++)
		{
			if (twoDimensionalArray[i][j] != rightPuzzle.twoDimensionalArray[i][j])
			{
				toReturn = false;
				break;
			}
		}
	}

	return toReturn;
}

bool Board_Tile::operator<(const Board_Tile& toCompare) const
{
	return (toCompare.manhattanDistancePlusNumberOfMoves <
		this->manhattanDistancePlusNumberOfMoves);
}

void Board_Tile::appendToMoves(std::string newMove)
{
	moves_from_start = moves_from_start + newMove;
}

int Board_Tile::numMoves() const
{
	return moves_from_start.length();
}

std::list<Board_Tile> Board_Tile::nextConfigs() const
{
	std::list<Board_Tile> listToReturn;
	listToReturn.clear();
	int valueToBeSwapped;
	std::vector<int> rowAndColOfEmptyTile;

	Board_Tile tempConfig = *this;
	std::string move;

	int rowToCheck = 0;
	int colToCheck = 0;


	//First find where the empty tile is
	if (this->rowAndColOfValue(0, rowAndColOfEmptyTile))
	{
		//Loop through all of the possible moves, and determine if they are valid.
		for (int i = 0; i < g_totalMovesOfEmptyTile; i++)
		{

			//Depending on the move, we must define which column and row to check.
			if (i == move::up)
			{
				rowToCheck = rowAndColOfEmptyTile.at(0) - 1;
				colToCheck = rowAndColOfEmptyTile.at(1);
				move = 'U';
			} 
			else if (i == move::left)
			{
				rowToCheck = rowAndColOfEmptyTile.at(0);
				colToCheck = rowAndColOfEmptyTile.at(1) - 1;
				move = 'L';
			}
			else if (i == move::right)
			{
				rowToCheck = rowAndColOfEmptyTile.at(0);
				colToCheck = rowAndColOfEmptyTile.at(1) + 1;
				move = 'R';
			}
			else if (i == move::down)
			{
				rowToCheck = rowAndColOfEmptyTile.at(0) + 1;
				colToCheck = rowAndColOfEmptyTile.at(1);
				move = 'D';
			}

			//Check if the move is valid
			if ((0 <= rowToCheck) && (rowToCheck < g_puzzleSize)
				&& (0 <= colToCheck) && (colToCheck < g_puzzleSize))
			{

				//If the move is valid then get the value to be swapped by the empty tile
				valueToBeSwapped = this->getValueByRowAndColumn
					(rowToCheck, colToCheck);

				//Do the swapping
				tempConfig.assign
					(rowToCheck,
					colToCheck, 0);

				tempConfig.assign
					(rowAndColOfEmptyTile.at(0),
					rowAndColOfEmptyTile.at(1),
					valueToBeSwapped);

				tempConfig.appendToMoves(move);

				//Insert the configuration into the list
				listToReturn.push_back(tempConfig);

				//Restore tempConfig so that the next check 
				tempConfig = *this;
			}
		}
	}
	else
	{
		std::cout << "Unable to find empty tile, please check config of board.\n";
	}

	//Setup the manhattan distance for each of the next configs.
	for (std::list<Board_Tile>::iterator nextConfigIt = listToReturn.begin();
		nextConfigIt != listToReturn.end(); ++nextConfigIt)
	{
		nextConfigIt->setGoalConfig(this->getGoalConfig());
		nextConfigIt->manhattanDistancePlusNumberOfMoves =
			nextConfigIt->Manhattan_Distance(*(nextConfigIt->getGoalConfig()))
			+ nextConfigIt->numMoves();
	}

	return listToReturn;
}

int Board_Tile::Manhattan_Distance(const Board_Tile& goalconfig) const
{
	int manhattanDistance = 0;
	int valueOnThisBoard = 0;
	std::vector<int> rowAndColOfGoalValue;

	for (int i = 0; i < g_puzzleSize; i++)
	{
		for (int j = 0; j < g_puzzleSize; j++)
		{
			valueOnThisBoard = this->getValueByRowAndColumn(i, j);
			if (goalconfig.rowAndColOfValue(valueOnThisBoard, rowAndColOfGoalValue))
			{
				//Calculate the manhattan distance by comparing the two boards.
				manhattanDistance = manhattanDistance + std::abs(i - rowAndColOfGoalValue.at(0))
					+ std::abs(j - rowAndColOfGoalValue.at(1));
			} 
			else
			{
				std::cout << "Manhattan Distance could not be calculated, check goal configuration\n";
			}
			//Clear the vector to find the next row and column tuple.
			rowAndColOfGoalValue.clear();
		}
	}
	return manhattanDistance;
}

bool Board_Tile::rowAndColOfValue(int valueToGet, std::vector<int> & vectorToReturn) const
{
	//Loop by rows and then columns
	for (int i = 0; i < g_puzzleSize; i++)
	{
		for (int j = 0; j < g_puzzleSize; j++)
		{
			if (this->getValueByRowAndColumn(i, j) == valueToGet)
			{
				vectorToReturn.push_back(i);
				vectorToReturn.push_back(j);
				return true;
			}
		}
	}

	return false;
}


//Function to return any value in the puzzle.
int Board_Tile::getValueByRowAndColumn(int row, int column) const
{
	return twoDimensionalArray[row][column];
}

//Function to assign any value in the puzzle.
void Board_Tile::assign(int row, int col, int value)
{
	twoDimensionalArray[row][col] = value;

	//update the config string to match the assignment
	config = "";
	std::stringstream ss;
	for (int i = 0; i < g_puzzleSize; i++)
	{
		for (int j = 0; j < g_puzzleSize; j++)
		{
			ss << twoDimensionalArray[i][j];
			config = ss.str();
		}
	}
}

std::string Board_Tile::getConfig() const
{
	return config;
}

std::string Board_Tile::getMovesFromStart() const
{
	return moves_from_start;
}

void Board_Tile::printConfig() const
{
	for (int i = 0; i < g_puzzleSize; i++)
	{
		for (int j = 0; j < g_puzzleSize; j++)
		{
			std::cout << this->twoDimensionalArray[i][j];
		}
		std::cout << std::endl;
	}
}


void Board_Tile::setGoalConfig(Board_Tile* goalConfig)
{
	goal = goalConfig;
}


Board_Tile* Board_Tile::getGoalConfig() const
{
	return goal;
}

void Board_Tile::setManhattanDistancePlusNumberOfMoves(int value) 
{
	manhattanDistancePlusNumberOfMoves = value;
}

int Board_Tile::getManhattanDistancePlusNumberOfMoves() const
{
	return manhattanDistancePlusNumberOfMoves;
}