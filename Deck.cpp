#include "Deck.h"

/* *********************************************************************
Class Name: Deck
Purpose: 
	- Contain all functions relating to the boneyard and related logic.
Parameters: 
	- None
Return Value:
	- Returns a new Deck object containing a vector of tiles which represents the boneyard.
Algorithm:
	1) Define private members.
Assistance Received: 
	- None
********************************************************************* */
Deck::Deck()
{
	for (int i = 0; i <= 9; i++)
	{
		for (int j = i; j <= 9; j++)
			boneyard.push_back(Tile(i, j));
	}
}

// -------------------Deck Logic-------------------
/* *********************************************************************
Function Name: pop_engine
Purpose: 
	- Removes and returns the engine tile from within the boneyard.
Parameters: 
	- The integer representing both left and right value of engine.
Return Value:
	- Returns Engine Tile.
Algorithm:
	1) Loop through boneyard.
	2) Find Engine(If tile.left and tile.right == engine integer).
	3) Erase engine tile from boneyard.
	4) Return engine tile.
Assistance Received: 
	- None
********************************************************************* */
Tile Deck::pop_engine(int curr_eng)
{
	Tile engine = Tile();
	for (int i = 0; i < boneyard.size(); i++)
	{
		if (boneyard.at(i).getleft() == curr_eng && boneyard.at(i).getright() == curr_eng)
		{
			engine = boneyard.at(i);
			boneyard.erase(boneyard.begin() + i);
			break;
		}
	}
	return engine;
}

/* *********************************************************************
Function Name: shuffle_boneyard
Purpose: 
	- Shuffles vector of tiles which represent the boneyard.
Parameters: 
	- None
Return Value:
	- None
Algorithm:
	1) Create random number generator object.
	2) Call Shuffle library function to shuffle vector.
Assistance Received: 
	- None
********************************************************************* */
void Deck::shuffle_boneyard()
{
	random_device rd;
	default_random_engine rng(rd());
	shuffle(boneyard.begin(), boneyard.end(), rng);
}

/* *********************************************************************
Function Name: draw_tile_from_boneyard
Purpose: 
	- Draw tile from top of boneyard vector.
Parameters: 
	- None
Return Value:
	- Tile from top of boneyard vector.
Algorithm:
	1) Create copy of tile at the 0 position of boneyard vector.
	2) Erase tile at the 0 position of boneyard.
	3) Return copied tile.
Assistance Received: 
	- None
********************************************************************* */
Tile Deck::draw_tile_from_boneyard()
{
	Tile temp = boneyard.at(0);
	boneyard.erase(boneyard.begin());
	return temp;
}

// -------------------Output Functions-------------------
/* *********************************************************************
Function Name: operator<< - Deck Class
Purpose: 
	- Overloaded << operator to be used for output and logging purposes.
Parameters: 
	- Current outstream
	- Tile object
Return Value:
	- Outstream containing tile information.
Algorithm:
	1) Return Left and Right member functions of Tile class.
Assistance Received: 
	- None
********************************************************************* */
ostream &operator<<(ostream &os, const Tile &t)
{
	return os << t.getleft() << " - " << t.getright() << endl;
}

/* *********************************************************************
Function Name: print_boneyard
Purpose: 
	- Print boneyard vector.
Parameters: 
	- None
Return Value:
	- None
Algorithm:
	1) Loop through boneyard vector.
	2) For each element, print to console.
Assistance Received: 
	- None
********************************************************************* */
void Deck::print_boneyard()
{
	for (int i = 0; i < boneyard.size(); i++)
		cout << "Boneyard: " << boneyard.at(i);
}

// -------------------Getters/Setters-------------------
/* *********************************************************************
Function Name: get_boneyard
Purpose: 
	- Get boneyard vector.
Parameters: 
	- None
Return Value:
	- Vector of Tiles(The Boneyard)
Algorithm:
	1) Return member variable, boneyard
Assistance Received: 
	- None
********************************************************************* */
vector<Tile> Deck::get_boneyard()
{
	return boneyard;
}

void Deck::set_boneyard(vector<Tile> savedbone)
{
	boneyard = savedbone;
}