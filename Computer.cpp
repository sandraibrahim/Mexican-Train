#include "Computer.h"

/* *********************************************************************
Class Name: Computer
Purpose:
	- Class Computer holds the computer's current move which is a pair including the Tile and a Train pointer.
Parameters:
	- None
Return Value:
	- Returns a Computer object after constructing private members.
Algorithm:
	- None
Assistance Received:
	- None
********************************************************************* */
Computer::Computer()
{
	pair<Tile, Train *> curr_move;
}

// Plays this players turn.
/*
	Check if the tile passed is -1 or if the Train passed is null, draw from boneyard logic
	Add the tile to the train given in the map
	Update variables: doubles, passed turns, etc
*/

/* *********************************************************************
Function Name: play
Purpose: 
	- Lets computer play its turn by checking its tiles and doing the proper logic to figure out what tile to play/what needs 
	to be done.
Parameters: 
	- None
Return Value:
	- None
Algorithm:
	1) Calls the return_valid_tiles function to get all of the valid tiles that can be played.
	2) Calls the move_calc function which will tell do all of the logic in picking the best tiles for the computer
	3) Outputs the move the computer will be doing and calls the move function to execute that move
Assistance Received: 
	- None
********************************************************************* */
void Computer::play()
{
	return_valid_tiles(curr_player);
	cout << "The computer chose to ";
	curr_move = curr_player->move_calc();
	cout << endl;
	move();
	cout << "Press any key to continue..." << endl;
	system("read");
	system("clear");
}

/* *********************************************************************
Function Name: move
Purpose: 
	- Executes the move calculated in the move calc class.
Parameters: 
	- None
Return Value:
	- None
Algorithm:
	1) Resets the turns marker if the previous player is not the current player.
	2) Checks if the current move is the default tile (-1,-1) or if the Train passed is null to see if it will be drawing from the boneyard.
	3) If it is, there is a series of checks that checks if the computer exhausted their hand after a double, has to draw from the boneyard, and if it does,
	it runs through a series of steps and does the proper thigns (adds/removes marker, etc)
	4) If drawing from boneyard, checks if the tile is valid and plays it on proper train. If it isnt, just adds to hand. If boneyard is empty, moves on.
	5) Checks if we are playing a double, sets flag
	6) If tile played was on personal marked train, remove marker.
	7) If player was initially skipped, reset skip flag.
	8) Add Tile to train and remove tile from hand.
	9) If tile played is a double, increment number of doubles and make another move.
	10) Else, cannot play more than 2 doubles in a row or it's not a double.
	11) Increments the turn counter
Assistance Received: 
	- None
********************************************************************* */
void Computer::move()
{
	int tile_num;
	int train_type;
	int location;
	Train *type = new Train();
	Tile tile = Tile();
	map<int, Tile> temp;
	int input;

	// reset turn markers if not previous player
	if (get_prev_player() != curr_player)
	{
		curr_player->reset_turns_marker();
	}

	//Check if the tile passed is -1 or if the Train passed is null, draw from boneyard logic
	if (curr_move.first == Tile())
	{
		if (curr_player->get_hand().size() == 0 && get_if_double())
		{
			cout << "Computer has exhausted hand after double." << endl;
			round_ended = true;
			add_score(human);
			add_score(computer);
			return;
		}
		cout << "Computer must draw from boneyard, no available tiles to play." << endl;
		if (boneyard->get_boneyard().size() > 0)
		{
			tile = boneyard->draw_tile_from_boneyard();
			curr_player->draw_tile(tile);
			cout << "Tile " << tile << " was drawn." << endl;
			if (isvalid(tile, curr_player, human->get_train()) || isvalid(tile, curr_player, computer->get_train()) || isvalid(tile, curr_player, mexican_train))
			{
				play_tile(tile);
				cout << "Tile drawn can be played on an available train. Tile has been added to train." << endl;
				return;
			}
			else
			{
				computer->get_train()->change_marker(true);
				computer->set_turns_marker();
				cout << "Tile drawed is not valid to play on any trains. Marker has been added to your train." << endl;
				return;
			}
		}
		else
		{
			cout << "Boneyard is empty, Marker has been added to your train. Turn passed." << endl;
			computer->get_train()->change_marker(true);
			computer->set_skipped(true);
			return;
		}
	}

	// Checks if we are playing a double, sets flag
	if (curr_move.first.getleft() == curr_move.first.getright())
	{
		set_if_double(true);
		set_count_double(count_double + 1);
	}
	else
	{
		set_if_double(false);
		set_count_double(0);
	}

	// If tile played was on personal marked train, remove marker.
	if (curr_move.second == curr_player->get_train() && curr_player->get_train()->get_marker())
	{
		curr_player->get_train()->change_marker(false);
		curr_player->reset_turns_marker();
		cout << "Marker has been removed" << endl;
		cout << endl;
	}

	// If player was initially skipped, reset skip flag.
	if (curr_player->get_skipped())
	{
		curr_player->set_skipped(false);
	}

	// Add Tile to train and remove tile from hand.
	location = curr_player->find_tile_in_hand(curr_move.first);
	curr_move.second->add_to_train(curr_move.first, curr_move.second->get_side());
	curr_player->remove_from_hand(location);

	// If tile played is a double, increment number of doubles and make another move.
	if (get_if_double() && count_double < 3)
	{
		set_prev_player(curr_player);
		curr_move.second->set_is_double(true);
		//set_count_double(count_double + 1);
		curr_player->reset_maps();
		cout << "Player played a Double! You get another move!" << endl;
		cout << endl;
		play();
	}
	// Cannot play more than 2 doubles in a row or it's not a double.
	else
	{
		curr_move.second->set_is_double(false);
		//set_count_double(0);
	}
	curr_player->set_turns_marker();
}

void Computer::play_tile(Tile tile)
{
	bool flag1 = false;
	bool flag2 = false;
	bool flag3 = false;

	curr_player->set_skipped(false);

	//Check which trains it is valid to play on
	if (isvalid(tile, curr_player, human->get_train()))
	{
		flag1 = true;
	}
	else if (isvalid(tile, curr_player, computer->get_train()))
	{
		flag2 = true;
	}
	else if (isvalid(tile, curr_player, mexican_train))
	{
		flag3 = true;
	}

	// Play train in this priority order: 1. Personal (if marker) 2. Opponent, 2. Mexican 3. Personal(no marker)
	if (flag2 && computer->get_train()->get_marker())
	{
		curr_player->get_train()->change_marker(false);
		curr_player->reset_turns_marker();
		cout << "Marker has been removed" << endl;
		cout << endl;
		computer->get_train()->add_to_train(tile, computer->get_identifier());
	}
	else if (flag1)
	{
		human->get_train()->add_to_train(tile, human->get_identifier());
	}
	else if (flag3)
	{
		mexican_train->add_to_train(tile, false);
	}
	else
	{
		computer->get_train()->add_to_train(tile, computer->get_identifier());
	}
}