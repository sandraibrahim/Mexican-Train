#include "Tile.h"

/* *********************************************************************
Class Name: Tile
Purpose: 
	- Creates tiles
Parameters: 
	- None
Return Value:
	- New Tile object
Algorithm:
	1) Define private members as -1 -1, shouldnt be used unless for flag
Assistance Received: 
	- None
********************************************************************* */
Tile::Tile()
{
	left = -1;
	right = -1;
}

/* *********************************************************************
Class Name: Tile
Purpose: 
	- Creates tiles with specific values in them
Parameters: 
	- 2 integers for left and right number
Return Value:
	- New Tile object
Algorithm:
	1) Define private members as sent in ones
Assistance Received: 
	- None
********************************************************************* */
Tile::Tile(int l, int r)
{
	left = l;
	right = r;
}

// -------------------Tile Logic-------------------

/* *********************************************************************
Function Name: switch tile
Purpose: 
	- switches the left and right in the tile
Parameters: 
	- None
Return Value:
	- None
Algorithm:
	1) Sets left to right, and right to left
Assistance Received: 
	- None
********************************************************************* */
void Tile::switch_tile()
{
	int temp = left;
	left = right;
	right = temp;
}

/* *********************************************************************
Function Name: operator==
Purpose: 
	- overloaded == operator checking if tiles are equal
Parameters: 
	- Tile object
Return Value:
	- Boolean
Algorithm:
	1) Compares left and right of both and returns true if same, else false.
Assistance Received: 
	- None
********************************************************************* */
bool Tile::operator==(Tile t2)
{
	return (this->getleft() == t2.getleft() && this->getright() == t2.getright());
}

/* *********************************************************************
Function Name: operator>
Purpose: 
	- overloaded > operator checking which is bigger
Parameters: 
	- Tile object
Return Value:
	- Boolean
Algorithm:
	1) Compares left and right of both and returns true if greater, else false.
Assistance Received: 
	- None
********************************************************************* */
bool Tile::operator>(Tile t2)
{
	int left_tile = this->getleft() + this->getright();
	int right_tile = t2.getleft() + t2.getright();

	return (left_tile > right_tile);
}

/* *********************************************************************
Function Name: operator<<
Purpose: 
	- overloaded << operator to print tiles in specific format
Parameters: 
	- Tile object, ostream object
Return Value:
	- ostream reference
Algorithm:
	1) Fills os with how it should be printed and returns
Assistance Received: 
	- None
********************************************************************* */
ostream &operator<<(ostream &os, Tile &t)
{
	os << t.getleft() << " - " << t.getright();
	return os;
}

// -------------------Getters/Setters-------------------

/* *********************************************************************
Function Name: getright
Purpose: 
	- returns right side of tile
Parameters: 
	- none
Return Value:
	- integer right side
Algorithm:
	1) Returns right side of tile
Assistance Received: 
	- None
********************************************************************* */
int Tile::getright() const
{
	return right;
}

/* *********************************************************************
Function Name: getleft
Purpose: 
	- returns left side of tile
Parameters: 
	- none
Return Value:
	- integer left side
Algorithm:
	1) Returns left side of tile
Assistance Received: 
	- None
********************************************************************* */
int Tile::getleft() const
{
	return left;
}