#include "Train.h"

/* *********************************************************************
Class Name: Train
Purpose: 
	- Class Train holds the vector of tiles to represent the train as well as
	  its next desired tile within, wanted_num, and a boolean flag, showing if
	  the train is marked.
Parameters: 
	- None
Return Value:
	- Returns a Train object after constructing private members.
Algorithm:
	- None
Assistance Received: 
	- None
********************************************************************* */
Train::Train()
{
	vector<Tile> curr_train{};
	is_marked = false;
	wanted_num = -1;
	is_double = false;
}

// -------------------Train Logic-------------------
/* *********************************************************************
Function Name: add_to_train
Purpose: 
	- Adds a Tile to train. tile is flipped depending on orientation of 
	  current train.
Parameters: 
	- None
Return Value:
	- None
Algorithm:
	1) Check if the current train is a human player or not
	2) If it is a human player, place tile on the right of train by comparing left
	3) If it is a computer player, place tile on left of train by comparing right.
Assistance Received: 
	- None
********************************************************************* */
void Train::add_to_train(Tile tile, bool type)
{
	if (type)
	{
		if (tile.getleft() == this->wanted_num)
		{
			tile.switch_tile();
		}
		curr_train.insert(curr_train.begin(), tile);
		this->wanted_num = curr_train.at(0).getleft();
	}
	else
	{
		if (tile.getright() == this->wanted_num)
		{
			tile.switch_tile();
		}
		curr_train.push_back(tile);
		this->wanted_num = curr_train.at(curr_train.size() - 1).getright();
	}
}

/* *********************************************************************
Function Name: operator==
Purpose: 
	- Overload == operator to make comparing Train objects easier.
Parameters: 
	- Train object to be compared with the current train.
Return Value:
	- Boolean value true if the compared train is equivalent. Else false.
Algorithm:
	1) Compare all train private attributes and return boolean result
Assistance Received: 
	- None
********************************************************************* */
bool Train::operator==(Train &t2)
{
	return (equal_to(this->get_curr_train(), t2.get_curr_train()) && this->wanted_num == t2.get_num() && this->is_marked == t2.get_marker());
}

/* *********************************************************************
Function Name: equal_to
Purpose: 
	- Compares two vectors of Tiles to determine if they are equivalent.
	  This function is used to overload the == operator.
Parameters: 
	- Two tiles of vectors.
Return Value:
	- Boolean value true if the compared vector is equivalent. Else false.
Algorithm:
	1) If vectors are of different sizes, they are not equal.
	2) Loop through each tile in both vectors.
	3) Compare left and right values of tiles within both vectors
	4) If there is one tile that does not equal another within the loop, vectors are not equal.
	5) If we run through the loop without any breaking, both vectors are equaivalent
Assistance Received: 
	- None
********************************************************************* */
bool Train::equal_to(vector<Tile> t1, vector<Tile> t2)
{
	if (t1.size() != t2.size())
	{
		return false;
	}
	for (int i = 0; i < t1.size(); i++)
	{
		if (t1.at(i).getleft() != t2.at(i).getleft() || t1.at(i).getright() != t2.at(i).getright())
		{
			return false;
		}
	}
	return true;
}

// -------------------Output Functions-------------------
/* *********************************************************************
Function Name: print_train
Purpose: 
	- Prints tiles within this current train.
Parameters: 
	- None
Return Value:
	- None
Algorithm:
	1) Loop through train.
	2) For each tile in train, print left and right values.
Assistance Received: 
	- None
********************************************************************* */
void Train::print_train()
{
	for (int i = 0; i < curr_train.size(); i++)
	{
		cout << curr_train.at(i).getleft() << " - " << curr_train.at(i).getright() << " ";
	}
}

// -------------------Getters/Setters-------------------
/* *********************************************************************
Function Name: get_side
Purpose: 
	- Returns an integer which lets the caller side know which side of
	  the train to play on
Parameters: 
	- None
Return Value:
	- Integer that represent the correct side to play the train on.
Algorithm:
	- Access Train object's private data member, 'tile_side'
Assistance Received: 
	- None
********************************************************************* */
int Train::get_side()
{
	return tile_side;
}

/* *********************************************************************
Function Name: set_side
Purpose: 
	- Sets the correct side to play tiles on for the current train.
Parameters: 
	- Boolean of the correct side to play tiles on.
Return Value:
	- None
Algorithm:
	- Access Train object's private data member, 'tile_side' and set equal to parameter.
Assistance Received: 
	- None
********************************************************************* */
void Train::set_side(bool b)
{
	this->tile_side = b;
}

/* *********************************************************************
Function Name: getnum
Purpose: 
	- Returns a Train's wanted number. This number is used to determine
	  which tiles can and cannot be played on this train.
Parameters: 
	- None
Return Value:
	- Train's private wanted number variable.
Algorithm:
	- Access Train object's private data member, 'wanted_num'
Assistance Received: 
	- None
********************************************************************* */
int Train::get_num()
{
	return wanted_num;
}

/* *********************************************************************
Function Name: set_num
Purpose: 
	- Sets the wanted number of the train to specific integer.
Parameters: 
	- Integer value of next expected tile to be played.
Return Value:
	- None
Algorithm:
	- Access Train object's private data member, 'wanted_num' and set equal to parameter.
Assistance Received: 
	- None
********************************************************************* */
void Train::set_num(int num)
{
	wanted_num = num;
}

/* *********************************************************************
Function Name: get_curr_train
Purpose: 
	- Return the vector of Tiles that represent the train.
Parameters: 
	- None
Return Value:
	- Train's vector of tiles which acts as the data structure that holds the train.
Algorithm:
	- Access Train object's private data member, 'curr_train'
Assistance Received: 
	- None
********************************************************************* */
vector<Tile> Train::get_curr_train()
{
	return curr_train;
}

/* *********************************************************************
Function Name: get_marker
Purpose: 
	- Returns a boolean flag representing if the Train object is marked or not.
Parameters: 
	- None
Return Value:
	- Train's private boolean value showing if it is marked or not.
Algorithm:
	- Access Train object's private data member, 'is_marked'
Assistance Received: 
	- None
********************************************************************* */
bool Train::get_marker()
{
	return is_marked;
}

/* *********************************************************************
Function Name: change_marker
Purpose: 
	- Change the current Train object's marker to specific bool value.
Parameters: 
	- Boolean value representing whether the train will be marked or not
Return Value:
	- None
Algorithm:
	- Access Train object's private data member, 'new_mark' and set equal to parameter.
Assistance Received: 
	- None
********************************************************************* */
void Train::change_marker(bool new_mark)
{
	is_marked = new_mark;
}

/* *********************************************************************
Function Name: get_is_double
Purpose: 
	- Returns the isdouble flag
Parameters: 
	- None
Return Value:
	- boolean is double flag
Algorithm:
	- returns is double flag
Assistance Received: 
	- None
********************************************************************* */
bool Train::get_is_double()
{
	return is_double;
}

/* *********************************************************************
Function Name: set_is_double
Purpose: 
	- Sets is double flag to new flag
Parameters: 
	- boolean flag
Return Value:
	- None
Algorithm:
	- Set is double to new flag
Assistance Received: 
	- None
********************************************************************* */
void Train::set_is_double(bool mark)
{
	is_double = mark;
}

/* *********************************************************************
Function Name: set_train
Purpose: 
	- Set curr train to new train
Parameters: 
	- vector of tiles (new train)
Return Value:
	- None
Algorithm:
	- Sets curr train to new train
Assistance Received: 
	- None
********************************************************************* */
void Train::set_train(vector<Tile> train)
{
	curr_train = train;
}