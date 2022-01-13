#include "Player.h"
#include "Deck.h"

/* *********************************************************************
Class Name: Player
Purpose:
	- Class Player holds a player's score, if computer or not, how many turns have passed, round end flag,
	  train, flags for doubles, saved game flag, all maps for valid tiles, and data from the current round. 
Parameters:
	- None
Return Value:
	- Returns a player object after constructing private members.
Algorithm:
	- None
Assistance Received:
	- None
********************************************************************* */
Player::Player()
{
	// Personal Data.
	vector<Tile> hand{};
	score = 0;
	is_comp = false;
	turnsMarker = 0;
	skipped = false;
	round_ended = false;
	train = new Train();
	second_invalid_double = false;
	count_double = 0;
	savegame = false;

	// Valid tile Maps.
	map<int, Tile> human_valid_nondoubles;
	map<int, Tile> computer_valid_nondoubles;
	map<int, Tile> mexican_valid_nondouble;
	map<int, Tile> human_valid_doubles;
	map<int, Tile> computer_valid_doubles;
	map<int, Tile> mexican_valid_double;

	// Data from Current Round.
	Player *human;
	Player *computer;
	Train *mexican_train;
	Player *curr_player;
	Deck *boneyard;
}

// -------------------Round Logic-------------------

/* *********************************************************************
Function Name: move
Purpose:
	- If this is called, something is wrong so it is a flag.
Parameters:
	- None
Return Value:
	- None
Algorithm:
	1) If this is called, something is wrong so it is a flag.
Assistance Received:
	- None
********************************************************************* */
void Player::move()
{
	cout << "Invalid move call!" << endl;
};

/* *********************************************************************
Function Name: play valid tile
Purpose:
	- Checks if the tile you drew from boneyard is valid to play and plays it.
Parameters:
	- A tile object and a player object
Return Value:
	- None
Algorithm:
	1) Calls is valid function checking if drawn tile is valid to play and outputs where it would be valid if applicable.
	2) Prompts user to choose where they want to play it and plays on train.
Assistance Received:
	- None
********************************************************************* */
void Player::play_valid_tile(Tile tile, Player *player)
{
	bool flag1 = false;
	bool flag2 = false;
	bool flag3 = false;
	int input;
	bool reask = false;
	cout << endl;
	if (isvalid(tile, player, human->get_train()))
	{
		cout << tile << " is valid to play on human train." << endl;
		flag1 = true;
	}
	else if (isvalid(tile, player, computer->get_train()))
	{
		cout << tile << " is valid to play on computer train." << endl;
		flag2 = true;
	}
	else if (isvalid(tile, player, mexican_train))
	{
		cout << tile << " is valid to play on mexican train." << endl;
		flag3 = true;
	}
	cout << endl;
	do
	{
		do
		{
			cout << "Which train would you like to play your drawn tile on: " << endl;
			cout << "1. Human Train" << endl;
			cout << "2. Computer Train" << endl;
			cout << "3. Mexican Train " << endl;
			cin >> input;
		} while (input < 1 || input > 3);

		switch (input)
		{
		case 1:
			if (flag1)
			{
				if (human->get_train() == player->get_train() && player->get_train()->get_marker())
				{
					player->get_train()->change_marker(false);
					player->reset_turns_marker();
					cout << "Marker has been removed" << endl;
					cout << endl;
				}
				player->set_skipped(false);
				human->get_train()->add_to_train(tile, human->get_identifier());
				reask = false;
			}
			else
			{
				reask = true;
			}
			break;
		case 2:
			if (flag2)
			{
				if (computer->get_train() == player->get_train() && player->get_train()->get_marker())
				{
					player->get_train()->change_marker(false);
					player->reset_turns_marker();
					cout << "Marker has been removed" << endl;
					cout << endl;
				}
				player->set_skipped(false);
				computer->get_train()->add_to_train(tile, computer->get_identifier());
				reask = false;
			}
			else
			{
				reask = true;
			}
			break;

		case 3:
			if (flag3)
			{
				player->set_skipped(false);
				mexican_train->add_to_train(tile, false);
				reask = false;
			}
			else
			{
				reask = true;
			}
			break;
		}
	} while (reask);
}

/* *********************************************************************
Function Name: which map
Purpose:
	- Sees which map user wants to play on and retuns it
Parameters:
	- None
Return Value:
	- integer
Algorithm:
	1) Asks user if they wanna play a double or non double and returns their answer
Assistance Received:
	- None
********************************************************************* */
int Player::which_map()
{
	int input;
	do
	{
		cout << "Would you like to play a nondouble or double? 1. Non-Double or 2. Double" << endl;
		cin >> input;
	} while (input < 1 || input > 2);
	return input;
}

/* *********************************************************************
Function Name: if draw
Purpose:
	- Checks if the player must draw from boneyard
Parameters:
	- Player object named current player
Return Value:
	- Boolean
Algorithm:
	- 1) Checks if the maps are empty and returns true if they are or false if not.
Assistance Received:
	- None
********************************************************************* */
bool Player::if_draw(Player *curr_player)
{
	Player *opp = get_opp_player(curr_player);

	// Check if there are no valid tiles for any map. You must draw.
	if (curr_player->get_human_nondouble_map().size() == 0 && curr_player->get_comp_nondouble_map().size() == 0 && curr_player->get_mex_nondouble_map().size() == 0 && curr_player->get_human_double_map().size() == 0 && curr_player->get_comp_double_map().size() == 0 && curr_player->get_mex_double_map().size() == 0)
	{
		return true;
	}
	return false;
}

/* *********************************************************************
Function Name: update train
Purpose:
	- Updates nessesary trains
Parameters:
	- Train object and integer named train type
Return Value:
	- None
Algorithm:
	1) Checks to see which train they chose and updates it.
Assistance Received:
	- None
********************************************************************* */
void Player::update_train(Train *type, int train_type)
{
	switch (train_type)
	{
	case 1:
		this->human->set_train(type);
		break;
	case 2:
		this->computer->set_train(type);
		break;
	case 3:
		this->mexican_train = type;
		break;
	default:
		break;
	}
}

/* *********************************************************************
Function Name: find tile index
Purpose:
	- Finds the index of a certain tile in hand
Parameters:
	- integer names num, map of integers and tiles named temp and a player object named player
Return Value:
	- Integer
Algorithm:
	1) Loops through hand to find the tiles index and returns it, if not there returns 0 as flag.
Assistance Received:
	- None
********************************************************************* */
int Player::find_tile_index(int num, map<int, Tile> temp, Player *player)
{
	for (int i = 0; i < player->get_hand().size(); i++)
	{
		if (player->get_hand().at(i) == temp[num - 1])
		{
			return i;
		}
	}
	return 0;
}

/* *********************************************************************
Function Name: get opp player
Purpose:
	- Returns opposite player
Parameters:
	- Player object named play
Return Value:
	- Player pointer
Algorithm:
	1) Returns the opposite player
Assistance Received:
	- None
********************************************************************* */
Player *Player::get_opp_player(Player *play)
{
	if (play->get_train() == human->get_train())
	{
		return this->computer;
	}
	return this->human;
}

/* *********************************************************************
Function Name: return valid tiles
Purpose:
	- Finds the valid tiles for each train and stores them in seperate maps
Parameters:
	- Player object
Return Value:
	- None
Algorithm:
	1) Checks for doubles and if the player had played a double previously.
	2) Calls is valid function for each tile in hand with each train
	3) Fills maps with the valid tiles.
Assistance Received:
	- None
********************************************************************* */
void Player::return_valid_tiles(Player *player)
{
	Tile curr_tile = Tile();
	int count1 = 0;
	int count2 = 0;
	int count3 = 0;
	int count4 = 0;
	int count5 = 0;
	int count6 = 0;
	vector<Tile> human_train = human->get_train()->get_curr_train();
	vector<Tile> comp_train = computer->get_train()->get_curr_train();
	vector<Tile> mex_train = mexican_train->get_curr_train();

	// if the round has curr_moved onto next player after prev played double.
	if (get_prev_player() != player)
	{
		//marks trains as doubles if their last played tile was a double
		if (human_train.at(human_train.size() - 1).getleft() == human_train.at(human_train.size() - 1).getright() && !(human_train.at(human_train.size() - 1) == engine))
		{
			human->get_train()->set_is_double(true);
		}
		if (comp_train.at(0).getleft() == comp_train.at(0).getright() && !(comp_train.at(0) == engine))
		{
			computer->get_train()->set_is_double(true);
		}
		if (mex_train.size() > 0 && mex_train.at(mex_train.size() - 1).getleft() == mex_train.at(mex_train.size() - 1).getright())
		{
			mexican_train->set_is_double(true);
		}
	}

	// Iterate through hand and populate valid tiles to valid tile maps for player.
	for (int i = 0; i < player->get_hand().size(); i++)
	{
		curr_tile = player->get_hand().at(i);
		// Human Valid Tiles.
		if (isvalid(curr_tile, player, human->get_train()))
		{
			if (curr_tile.getleft() != curr_tile.getright())
			{
				player->set_human_nondouble_map(count1, curr_tile);
				count1++;
			}
			else
			{
				player->set_human_double_map(count2, curr_tile);
				count2++;
			}
		}
		// Computer Valid Tiles.
		if (isvalid(curr_tile, player, computer->get_train()))
		{
			if (curr_tile.getleft() != curr_tile.getright())
			{
				player->set_comp_nondouble_map(count3, curr_tile);
				count3++;
			}
			else
			{
				player->set_comp_double_map(count4, curr_tile);
				count4++;
			}
		}
		//Mexican Valid Tiles.
		if (isvalid(curr_tile, player, mexican_train))
		{
			if (curr_tile.getleft() != curr_tile.getright())
			{
				player->set_mex_nondouble_map(count5, curr_tile);
				count5++;
			}
			else
			{
				player->set_mex_double_map(count6, curr_tile);
				count6++;
			}
		}
	}
}

/* *********************************************************************
Function Name: is valid
Purpose:
	- Checks if a tile is valid to play on certain train
Parameters:
	- Tile obkect, Player pointer object, Train pointer object
Return Value:
	- boolean
Algorithm:
	1) Checks for valid tiles for each hand depending on doubles played, markers, etc.
Assistance Received:
	- None
********************************************************************* */
bool Player::isvalid(Tile tile, Player *player, Train *type)
{
	int left = tile.getleft();
	int right = tile.getright();
	int currplayer_num = player->get_train()->get_num();
	int opp_num = get_opp_player(player)->get_train()->get_num();
	int mex_num = mexican_train->get_num();

	// If a second double is being played.
	if (count_double == 1 && (left == right))
	{
		// Check if double tile is valid (Current Player Train).
		if (type == player->get_train())
		{
			if (type->get_num() == left)
			{
				// Iterate through every hand in the tile.

				for (int i = 0; i < player->get_hand().size(); i++)
				{
					// Check if player can play a tile after the double to be played.
					if ((player->get_hand().at(i).getleft() == left || player->get_hand().at(i).getright() == right) && !(player->get_hand().at(i) == tile))
					{
						return true;
					}

					// Check if we can play a tile on Opponent train.
					if (get_opp_player(player)->get_train()->get_marker() && (player->get_hand().at(i).getleft() == opp_num || player->get_hand().at(i).getright() == opp_num))
					{
						return true;
					}

					// Check if we can play a tile on Mexican train.
					if (player->get_hand().at(i).getleft() == mex_num || player->get_hand().at(i).getright() == mex_num)
					{
						return true;
					}
				}
				second_invalid_double = true;
			}
		}
		// Check if double tile is valid (Mexican Train).
		else if (type == mexican_train)
		{
			if (type->get_num() == left)
			{

				// Iterate through every hand in the tile.
				for (int i = 0; i < player->get_hand().size(); i++)
				{

					// Check if player can play a tile after the double to be played.
					if ((player->get_hand().at(i).getleft() == left || player->get_hand().at(i).getright() == right) && !(player->get_hand().at(i) == tile))
					{
						return true;
					}

					// Check if we can play a tile on Current Player train.
					if (player->get_hand().at(i).getleft() == currplayer_num || player->get_hand().at(i).getright() == currplayer_num)
					{
						return true;
					}

					// Check if we can play a tile on Opponent train.
					if (get_opp_player(player)->get_train()->get_marker() && (player->get_hand().at(i).getleft() == opp_num || player->get_hand().at(i).getright() == opp_num))
					{
						return true;
					}
				}

				second_invalid_double = true;
			}
		}
		// Check if double tile is valid (Opponent Train).
		else if (type == get_opp_player(player)->get_train() && get_opp_player(player)->get_train()->get_marker())
		{
			if (type->get_num() == left)
			{
				for (int i = 0; i < player->get_hand().size(); i++)
				{
					if ((player->get_hand().at(i).getleft() == left || player->get_hand().at(i).getright() == right) && (!(player->get_hand().at(i) == tile)))
					{
						return true;
					}

					// Check if we can play a tile on Current Player train.
					if (player->get_hand().at(i).getleft() == currplayer_num || player->get_hand().at(i).getright() == currplayer_num)
					{
						return true;
					}

					// Check if we can play a tile on Mexican train.
					if (player->get_hand().at(i).getleft() == mex_num || player->get_hand().at(i).getright() == mex_num)
					{
						return true;
					}
				}
				second_invalid_double = true;
			}
		}

		return false;
	}

	// Case of Doubles.
	if (!(player == get_prev_player()) && (human->get_train()->get_is_double() || computer->get_train()->get_is_double() || mexican_train->get_is_double()))
	{
		// If there exists an orphan double on human train.
		if (human->get_train()->get_is_double() && type == human->get_train())
		{
			if (left == type->get_num() || right == type->get_num())
			{
				return true;
			}
			return false;
		}
		// If there exists an orphan double on computer train.
		else if (computer->get_train()->get_is_double() && type == computer->get_train())
		{
			if (left == type->get_num() || right == type->get_num())
			{
				return true;
			}
			return false;
		}
		// If there exists an orphan double on mexican train.
		else if (mexican_train->get_is_double() && type == mexican_train)
		{
			if (left == type->get_num() || right == type->get_num())
			{
				return true;
			}
			return false;
		}
		return false;
	}

	// If there is a marker on current players train.
	if (player->get_train()->get_marker())
	{
		if (player->get_turns_marker() % 2 == 0)
		{
			// Checking if Opponent Train is valid.
			if (type == player->get_train() || type == mexican_train)
			{
				if (left == type->get_num() || right == type->get_num())
				{
					return true;
				}
				return false;
			}

			if (type == get_opp_player(player)->get_train() && get_opp_player(player)->get_train()->get_marker())
			{
				if (left == type->get_num() || right == type->get_num())
				{
					return true;
				}
				return false;
			}
			else
			{
				return false;
			}
		}
		// Player can only play on their own train.
		else
		{
			// Checking if Current Train.
			if (type == player->get_train())
			{
				if (left == type->get_num() || right == type->get_num())
				{
					return true;
				}
				return false;
			}
			// Cannot play on any other train.
			return false;
		}
	}

	// General Case.
	else
	{
		// Checking if Opponent Train is valid.
		if (type == player->get_train() || type == mexican_train)
		{
			if (left == type->get_num() || right == type->get_num())
			{
				return true;
			}
			return false;
		}

		if (type == get_opp_player(player)->get_train() && get_opp_player(player)->get_train()->get_marker())
		{
			if (left == type->get_num() || right == type->get_num())
			{
				return true;
			}
			return false;
		}
		else
		{
			return false;
		}
	}
}

/* *********************************************************************
Function Name: move calc
Purpose:
	- Calculates best move for the player
Parameters:
	- None
Return Value:
	- Pair of a Tile and Train pointer
Algorithm:
	1) Checks if there is available tiles and tells user to draw from boneyard.
	2) Determine if there are doubles.
	3) If there are doubles: Pick highest double and play.
	4) If there are multiple "Highest" doubles, follow this Order:
			- Mex Train.
			- Opponent Train.
			- Personal Train.
	5) Orphan doubles are handled passively.
	6) If there are no doubles: Find the tile with largest value and play.
	7) If there are multiple "Highest" tiles, follow this Order:
			- Opponent Train.
			- Mex Train.
			- Personal Train.
Assistance Received:
	- None
********************************************************************* */
pair<Tile, Train *> Player::move_calc()
{
	pair<Tile, Train *> curr_move;
	Tile largest_human = Tile();
	Tile largest_comp = Tile();
	Tile largest_mex = Tile();

	// If no available tiles, draw from boneyard. Returns default tile
	if (human_valid_doubles.size() == 0 && computer_valid_doubles.size() == 0 && mexican_valid_doubles.size() == 0 &&
		 human_valid_nondoubles.size() == 0 && computer_valid_nondoubles.size() == 0 && mexican_valid_nondoubles.size() == 0)
	{
		cout << "draw from the boneyard. No available tiles to play." << endl;
		curr_move.first = Tile();
		curr_move.second = NULL;
		return curr_move;
	}

	// Check if there are available doubles to play
	if (human_valid_doubles.size() > 0 || computer_valid_doubles.size() > 0 || mexican_valid_doubles.size() > 0)
	{
		if (human_valid_doubles.size() > 0)
		{
			largest_human = get_largest(human_valid_doubles);
		}
		if (computer_valid_doubles.size() > 0)
		{
			largest_comp = get_largest(computer_valid_doubles);
		}
		if (mexican_valid_doubles.size() > 0)
		{
			largest_mex = get_largest(mexican_valid_doubles);
		}
		//gets the largest tile and train and returns
		curr_move = compare_largest(largest_human, largest_comp, largest_mex);
		cout << " This tile has priority because it is a double." << endl;
		return curr_move;
	}
	// Check nondoubles
	else
	{
		if (human_valid_nondoubles.size() > 0)
		{
			largest_human = get_largest(human_valid_nondoubles);
		}
		if (computer_valid_nondoubles.size() > 0)
		{
			largest_comp = get_largest(computer_valid_nondoubles);
		}
		if (mexican_valid_nondoubles.size() > 0)
		{
			largest_mex = get_largest(mexican_valid_nondoubles);
		}
		//gets the largest tile and train and returns
		curr_move = compare_largest(largest_human, largest_comp, largest_mex);
		cout << endl;
		return curr_move;
	}
}

/* *********************************************************************
Function Name: quit
Purpose:
	- Ends the round and adds scores to each player.
Parameters:
	- None
Return Value:
	- None
Algorithm:
	1) Set quit flag as True.
	2) Add scores to both players.
Assistance Received:
	- None
********************************************************************* */
void Player::quit()
{
	round_ended = true;
	cout << "Player has quit Round." << endl;
}

/* *********************************************************************
Function Name: get largest
Purpose:
	- Gets the largest tile in map
Parameters:
	- Map of integers and tile objects
Return Value:
	- Tile object
Algorithm:
	1) Loops through map and compares to each tile and stores largest and returns.
Assistance Received:
	- None
********************************************************************* */
Tile Player::get_largest(map<int, Tile> curr_map)
{
	Tile largest = curr_map.at(0);
	for (int i = 0; i < curr_map.size(); i++)
	{
		if (curr_map.at(i) > largest)
		{
			largest = curr_map.at(i);
		}
	}
	return largest;
}

/* *********************************************************************
Function Name: compare largest
Purpose:
	- Compares the largest tiles if there are multiple of the same and uses same priority as before with trains.
Parameters:
	-  3 Tile objects
Return Value:
	- pair of Tile and Train pointed
Algorithm:
	1) Compares the largest tiles if there are multiple of the same and uses same priority as before with trains.
Assistance Received:
	- None
********************************************************************* */
pair<Tile, Train *> Player::compare_largest(Tile largest_human, Tile largest_comp, Tile largest_mex)
{
	Player *opp_player = get_opp_player(curr_player);
	pair<Tile, Train *> curr_move;
	Tile largest_tile = Tile();

	// if all have the same largest tile, play on player if marked, else play on opponent
	if (largest_comp == largest_human && largest_human == largest_mex)
	{
		if (curr_player->get_train()->get_marker())
		{
			curr_move.first = largest_comp;
			curr_move.second = curr_player->get_train();
			cout << "play " << largest_mex << " on current player's train because " << largest_mex << " was the largest tile in hand and the current player's train has a marker.";
			return curr_move;
		}
		else
		{
			curr_move.first = largest_comp;
			curr_move.second = opp_player->get_train();
			cout << "play " << largest_mex << " on the opponent's train because " << largest_mex << " was the largest tile in hand and the opponent's train has a marker.";
			return curr_move;
		}
	}

	// if largest human = computer and the current player is human/computer, play on player if marked, else play on opponent
	if (largest_human == largest_comp && !(largest_human == Tile()))
	{
		if (curr_player == human)
		{
			if (curr_player->get_train()->get_marker())
			{
				curr_move.first = largest_human;
				curr_move.second = human->get_train();
				cout << "play " << largest_human << " on current player's train because " << largest_human << " was the largest tile in hand and the current player's train has a marker.";
				return curr_move;
			}
			else
			{
				curr_move.first = largest_comp;
				curr_move.second = computer->get_train();
				cout << "play " << largest_comp << " on the opponent's train because " << largest_comp << " was the largest tile in hand and the opponent's train has a marker.";
				return curr_move;
			}
		}
		else
		{
			if (curr_player->get_train()->get_marker())
			{
				curr_move.first = largest_comp;
				curr_move.second = computer->get_train();
				cout << "play " << largest_comp << " on current player's train because " << largest_comp << " was the largest tile in hand and the current player's train has a marker.";
				return curr_move;
			}
			else
			{
				curr_move.first = largest_human;
				curr_move.second = human->get_train();
				cout << "play " << largest_human << " on the opponent's train because " << largest_human << " was the largest tile in hand and the opponent's train has a marker.";
				return curr_move;
			}
		}
	}

	// if largest human = largest mexican, if current player is human, check marked, play if true, else play on mex, if not curr player play on human bc opponents
	else if (largest_human == largest_mex && !(largest_human == Tile()))
	{
		if (curr_player == human)
		{
			if ((curr_player->get_train()->get_marker()))
			{
				curr_move.first = largest_human;
				curr_move.second = human->get_train();
				cout << "play " << largest_human << " on current player's train because " << largest_human << " was the largest tile in hand and the current player's train has a marker.";
				return curr_move;
			}
			else
			{
				curr_move.first = largest_mex;
				curr_move.second = mexican_train;
				cout << "play " << largest_mex << " on the mexican train because " << largest_mex << " was the largest valid tile in hand.";
				return curr_move;
			}
		}
		else
		{
			curr_move.first = largest_human;
			curr_move.second = human->get_train();
			cout << "play " << largest_human << " on the opponent's train because " << largest_human << " was the largest tile in hand and the opponent's train has a marker.";
			return curr_move;
		}
	}
	// if largest comp = largest mexican, if current player is comp, check marked and play on if true, else play on mex, if not curr player else play on human bc opponents
	else if (largest_comp == largest_mex && !(largest_comp == Tile()))
	{
		if (curr_player == computer)
		{
			if (curr_player->get_train()->get_marker())
			{
				curr_move.first = largest_comp;
				curr_move.second = computer->get_train();
				cout << "play " << largest_comp << " on current player's train because " << largest_comp << " was the largest tile in hand and the current player's train has a marker.";
				return curr_move;
			}
			else
			{
				curr_move.first = largest_mex;
				curr_move.second = mexican_train;
				cout << "play " << largest_mex << " on the mexican train because " << largest_mex << " was the largest valid tile in hand.";
				return curr_move;
			}
		}
		else
		{
			curr_move.first = largest_comp;
			curr_move.second = computer->get_train();
			cout << "play " << largest_comp << " on the opponent's train because " << largest_comp << " was the largest tile in hand and the opponent's train has a marker.";
			return curr_move;
		}
	}

	// Check whos the largest in all
	if (largest_human > largest_comp)
	{
		if (largest_human > largest_mex)
		{
			largest_tile = largest_human;
			curr_move.first = largest_tile;
			curr_move.second = human->get_train();
			cout << "play " << largest_tile << " on the human's train because " << largest_tile << " was the largest valid tile in hand ";
			if (curr_player == human)
			{
				cout << " and the current player's train was the only option.";
				return curr_move;
			}
			else
			{
				cout << " and the opponent's train has a marker.";
				return curr_move;
			}
		}
		else
		{
			largest_tile = largest_mex;
			curr_move.first = largest_tile;
			curr_move.second = mexican_train;
			cout << "play " << largest_tile << " on the mexican train because " << largest_tile << " was the largest valid tile in hand and the mexican train was the only option for this tile.";
			return curr_move;
		}
	}
	else if (largest_comp > largest_mex)
	{
		largest_tile = largest_comp;
		curr_move.first = largest_tile;
		curr_move.second = computer->get_train();
		cout << "play " << largest_tile << " on the computer's train because " << largest_tile << " was the largest valid tile in hand";
		if (curr_player == computer)
		{
			cout << " and the current player's train was the only option.";
			return curr_move;
		}
		else
		{
			cout << " and the opponent's train has a marker.";
			return curr_move;
		}
	}
	else
	{
		largest_tile = largest_mex;
		curr_move.first = largest_tile;
		curr_move.second = mexican_train;
		cout << "play " << largest_tile << " on the mexican train because " << largest_tile << " was the largest valid tile in hand and the mexican train was the only option for this tile.";
		return curr_move;
	}

	return curr_move;
}

/* *********************************************************************
Function Name: find tile in hand
Purpose:
	- Finds a specific tile in hand
Parameters:
	-  Tile objects
Return Value:
	- integer
Algorithm:
	1) Finds tile in hand and returns index.
Assistance Received:
	- None
********************************************************************* */
int Player::find_tile_in_hand(Tile tile)
{
	for (int i = 0; i < curr_player->get_hand().size(); i++)
	{
		if (curr_player->get_hand().at(i) == tile)
		{
			return i;
		}
	}
	return 0;
}

// -------------------Output Helper Functions-------------------

/* *********************************************************************
Function Name: print_valid_tiles
Purpose:
	- Prints all valid tiles a player can play.
Parameters:
	- Specific Player object to print and integer of which train's valid tiles to print.
Return Value:
	- None
Algorithm:
	1) Determine which train's valid tiles are being printed with num
	2) Iterate through appropriate valid tile map and print all Tiles.
Assistance Received:
	- None
********************************************************************* */
void Player::print_valid_tiles(Player *player, int num, int option)
{
	if (num == 1)
	{
		if (option == 1)
		{
			cout << "Human Non-Double Options" << endl;
			cout << "------------------------------------" << endl;
			for (int i = 0; i < curr_player->get_human_nondouble_map().size(); i++)
			{
				cout << i + 1 << ". " << player->get_human_nondouble_map()[i].getleft() << " - " << player->get_human_nondouble_map()[i].getright() << " ";
			}
		}
		else
		{
			cout << "Human Double Options" << endl;
			cout << "------------------------------------" << endl;
			for (int i = 0; i < curr_player->get_human_double_map().size(); i++)
			{
				cout << i + 1 << ". " << player->get_human_double_map()[i].getleft() << " - " << player->get_human_double_map()[i].getright() << " ";
			}
		}
		cout << endl;
	}
	else if (num == 2)
	{
		if (option == 1)
		{
			cout << "Computer Non-Double Options" << endl;
			cout << "------------------------------------" << endl;
			for (int i = 0; i < curr_player->get_comp_nondouble_map().size(); i++)
			{
				cout << i + 1 << ". " << player->get_comp_nondouble_map()[i].getleft() << " - " << player->get_comp_nondouble_map()[i].getright() << " ";
			}
		}
		else
		{
			cout << "Computer Double Options" << endl;
			cout << "------------------------------------" << endl;
			for (int i = 0; i < curr_player->get_comp_double_map().size(); i++)
			{
				cout << i + 1 << ". " << player->get_comp_double_map()[i].getleft() << " - " << player->get_comp_double_map()[i].getright() << " ";
			}
		}
		cout << endl;
	}
	else
	{
		if (option == 1)
		{
			cout << "Mexican Non-Double Options" << endl;
			cout << "------------------------------------" << endl;
			for (int i = 0; i < curr_player->get_mex_nondouble_map().size(); i++)
			{
				cout << i + 1 << ". " << player->get_mex_nondouble_map()[i].getleft() << " - " << player->get_mex_nondouble_map()[i].getright() << " ";
			}
		}
		else
		{
			cout << "Mexican Double Options" << endl;
			cout << "------------------------------------" << endl;
			for (int i = 0; i < curr_player->get_mex_double_map().size(); i++)
			{
				cout << i + 1 << ". " << player->get_mex_double_map()[i].getleft() << " - " << player->get_mex_double_map()[i].getright() << " ";
			}
		}
		cout << endl;
	}
}

/* *********************************************************************
Function Name: print_options
Purpose:
	- Prints all valid tile options a player can play on every train.
Parameters:
	- Specific Player object to print.
Return Value:
	- None
Algorithm:
	1) Loop through every valid tiles map
	2) For each element in every map, print their tiles.
Assistance Received:
	- None
********************************************************************* */
void Player::print_options(Player *player)
{
	cout << "Available Trains Options:" << endl;
	cout << endl;
	cout << "1. Human Train Available Tiles" << endl;
	cout << "------------------------------------" << endl;
	cout << "Non-Double Options" << endl;
	for (int i = 0; i < curr_player->get_human_nondouble_map().size(); i++)
	{
		cout << i + 1 << ". " << player->get_human_nondouble_map()[i].getleft() << " - " << player->get_human_nondouble_map()[i].getright() << " ";
	}
	cout << endl;
	cout << "Double Options" << endl;
	for (int i = 0; i < curr_player->get_human_double_map().size(); i++)
	{
		cout << i + 1 << ". " << player->get_human_double_map()[i].getleft() << " - " << player->get_human_double_map()[i].getright() << " ";
	}
	cout << endl;
	cout << "2. Computer Train Available Tiles" << endl;
	cout << "-------------------------------" << endl;
	cout << "Non-Double Options" << endl;
	for (int i = 0; i < curr_player->get_comp_nondouble_map().size(); i++)
	{
		cout << i + 1 << ". " << player->get_comp_nondouble_map()[i].getleft() << " - " << player->get_comp_nondouble_map()[i].getright() << " ";
	}
	cout << endl;
	cout << "Double Options" << endl;
	for (int i = 0; i < curr_player->get_comp_double_map().size(); i++)
	{
		cout << i + 1 << ". " << player->get_comp_double_map()[i].getleft() << " - " << player->get_comp_double_map()[i].getright() << " ";
	}
	cout << endl;
	cout << "3. Mexican Train Available Tiles " << endl;
	cout << "-------------------------------" << endl;
	cout << "Non-Double Options" << endl;
	for (int i = 0; i < curr_player->get_mex_nondouble_map().size(); i++)
	{
		cout << i + 1 << ". " << player->get_mex_nondouble_map()[i].getleft() << " - " << player->get_mex_nondouble_map()[i].getright() << " ";
	}
	cout << endl;
	cout << "Double Options" << endl;
	for (int i = 0; i < curr_player->get_mex_double_map().size(); i++)
	{
		cout << i + 1 << ". " << player->get_mex_double_map()[i].getleft() << " - " << player->get_mex_double_map()[i].getright() << " ";
	}
	cout << endl;
	cout << endl;
}

// -------------------Player Logic-------------------
/* *********************************************************************
Function Name: draw_tile
Purpose:
	- Adds tile to hand vector of tiles.
Parameters:
	- Tile object
Return Value:
	- None
Algorithm:
	- Add passed tiled to hand vector using push_back().
Assistance Received:
	- None
********************************************************************* */
void Player::draw_tile(Tile tile)
{
	hand.push_back(tile);
}

/* *********************************************************************
Function Name: operator==
Purpose:
	- Returns whether a player object is equivalent to another.
Parameters:
	- Tile object
Return Value:
	- Boolean value which
Algorithm:
	- Add passed tiled to hand vector using push_back().
Assistance Received:
	- None
********************************************************************* */
bool Player::operator==(Player &p2)
{
	return (this->get_train() == p2.get_train());
}

/* *********************************************************************
Function Name: play
Purpose:
	- Flag play, not supposed to enter
Parameters:
	- None
Return Value:
	- None
Algorithm:
	- Prints out invalid play call
Assistance Received:
	- None
********************************************************************* */
void Player::play()
{
	cout << "Invalid play call!" << endl;
}

// -------------------Output Functions-------------------
/* *********************************************************************
Function Name: print_hands
Purpose:
	- Prints player's hand vector.
Parameters:
	- None
Return Value:
	- None
Algorithm:
	- For every Tile, print left and right values.
Assistance Received:
	- None
********************************************************************* */
void Player::print_hands()
{
	for (int i = 0; i < hand.size(); i++)
	{
		cout << "(" << hand.at(i).getleft() << " - " << hand.at(i).getright() << ") ";
	}
	cout << endl;
}

/* *********************************************************************
Function Name: print_hands
Purpose:
	- Prints player's train object.
Parameters:
	- None
Return Value:
	- None
Algorithm:
	- Call player train's inner class function to print train object.
Assistance Received:
	- None
********************************************************************* */
void Player::print_trains()
{
	train->print_train();
	if (train->get_marker())
	{
		cout << "M" << endl;
	}
	cout << endl;
}

// -------------------Getters/Setters-------------------
/* *********************************************************************
Function Name: add_train
Purpose:
	- Add in a tile to a player's train object.
Parameters:
	- Tile object to be added to train
Return Value:
	- None
Algorithm:
	- Call player train's inner class function to add tile to train object.
Assistance Received:
	- None
********************************************************************* */
void Player::add_train(Tile tile)
{
	//check if computer or not later
	train->add_to_train(tile, is_comp);
}

/* *********************************************************************
Function Name: get_train
Purpose:
	- Get player's train object.
Parameters:
	- None
Return Value:
	- Player's train Object
Algorithm:
	- Access Player's private train variable.
Assistance Received:
	- None
********************************************************************* */
Train *Player::get_train()
{
	return train;
}

/* *********************************************************************
Function Name: set_train
Purpose:
	- Set player's train object to a new Train object.
Parameters:
	- None
Return Value:
	- Player's train Object
Algorithm:
	- Access Player's private train variable.
Assistance Received:
	- None
********************************************************************* */
void Player::set_train(Train *new_train)
{
	this->train = new_train;
}

/* *********************************************************************
Function Name: get_identifier
Purpose:
	- Get player's player identifier(If they are a computer or not).
Parameters:
	- None
Return Value:
	- Player's identifier boolean
Algorithm:
	- Access Player's private identifier variable.
Assistance Received:
	- None
********************************************************************* */
bool Player::get_identifier()
{
	return is_comp;
}

/* *********************************************************************
Function Name: set_identifier
Purpose:
	- Set player's player identifier(If they are a computer or not).
Parameters:
	- Boolean value indicating player's identifier.
Return Value:
	- Player's identifier boolean.
Algorithm:
	- Access Player's private identifier variable and set it equal to parameter.
Assistance Received:
	- None
********************************************************************* */
void Player::set_identifier(bool ident)
{
	is_comp = ident;
}

/* *********************************************************************
Function Name: get_hand
Purpose:
	- Get player's hand vector.
Parameters:
	- None
Return Value:
	- Vector of Tiles representing Player's hand
Algorithm:
	- Access Player's private hand variable.
Assistance Received:
	- None
********************************************************************* */
vector<Tile> Player::get_hand()
{
	return hand;
}

/* *********************************************************************
Function Name: remove_from_hand
Purpose:
	- remove a specific tile from a hand at a given index.
Parameters:
	- Integer representing index to erase tile from hand.
Return Value:
	- None
Algorithm:
	- Call the .erase function for the player's "hand" vector.
Assistance Received:
	- None
********************************************************************* */
void Player::remove_from_hand(int location)
{
	this->hand.erase(this->hand.begin() + (location));
}

/* *********************************************************************
Function Name: get_turns_marker
Purpose:
	- Get number of turns since player's train has been marked
Parameters:
	- None
Return Value:
	- Player's turn counter integer
Algorithm:
	- Access Player's private turnsMarker variable.
Assistance Received:
	- None
********************************************************************* */
int Player::get_turns_marker()
{
	return turnsMarker;
}

/* *********************************************************************
Function Name: set_turns marker
Purpose:
	- Adds one to the turns marker
Parameters:
	- None
Return Value:
	- None
Algorithm:
	- Adds one to the turns marker variable
Assistance Received:
	- None
********************************************************************* */
void Player::set_turns_marker()
{
	turnsMarker += 1;
}

/* *********************************************************************
Function Name: get_score
Purpose:
	- Get player's current score for the Round.
Parameters:
	- None
Return Value:
	- Player's score integer
Algorithm:
	- Access Player's private score variable.
Assistance Received:
	- None
********************************************************************* */
int Player::get_score()
{
	return score;
}

/* *********************************************************************
Function Name: set_score
Purpose:
	- Add a specific integer value to player's score.
Parameters:
	- Integer representing score to append to.
Return Value:
	- None
Algorithm:
	- Access player's score and add the parameter value to private variable.
Assistance Received:
	- None
********************************************************************* */
void Player::set_score(int num)
{
	score += num;
}

/* *********************************************************************
Function Name: assign_score
Purpose:
	- Assigns score
Parameters:
	- integer num
Return Value:
	- None
Algorithm:
	- Sets the score to the new score
Assistance Received:
	- None
********************************************************************* */
void Player::assign_score(int num)
{
	score = num;
}

/* *********************************************************************
Function Name: get_human_nondouble_map
Purpose:
	- Get map of all valid tiles a player can play on the human train for a specific turn.
Parameters:
	- None
Return Value:
	- Map of (int, Tile) pairs representing valid Tiles that can be played on Human Train.
Algorithm:
	- Access Player's private human_valid_nondoubles variable.
Assistance Received:
	- None
********************************************************************* */
map<int, Tile> Player::get_human_nondouble_map()
{
	return human_valid_nondoubles;
}

/* *********************************************************************
Function Name: get_comp_nondouble_map
Purpose:
	- Get map of all valid tiles a player can play on the human train for a specific turn.
Parameters:
	- None
Return Value:
	- Map of (int, Tile) pairs representing valid Tiles that can be played on Computer Train.
Algorithm:
	- Access Player's private computer_valid_nondoubles variable.
Assistance Received:
	- None
********************************************************************* */
map<int, Tile> Player::get_comp_nondouble_map()
{
	return computer_valid_nondoubles;
}

/* *********************************************************************
Function Name: get_mex_nondouble_map
Purpose:
	- Get map of all valid tiles a player can play on the mexican train for a specific turn.
Parameters:
	- None
Return Value:
	- Map of (int, Tile) pairs representing valid Tiles that can be played on Mexican Train.
Algorithm:
	- Access Player's private mexican_valid_nondoubles variable.
Assistance Received:
	- None
********************************************************************* */
map<int, Tile> Player::get_mex_nondouble_map()
{
	return mexican_valid_nondoubles;
}

/* *********************************************************************
Function Name: set_human_nondouble_map
Purpose:
	- Adds a (Int, Tile) to human_valid_nondoubles map.
Parameters:
	- Integer key and Tile object to store within human_valid_nondoubles map.
Return Value:
	- None
Algorithm:
	- Append new key and value pair to map by using human_valid_nondoubles[key] = value.
Assistance Received:
	- None
********************************************************************* */
void Player::set_human_nondouble_map(int num, Tile tile)
{
	human_valid_nondoubles[num] = tile;
}

/* *********************************************************************
Function Name: set_comp_nondouble_map
Purpose:
	- Adds a (Int, Tile) to computer_valid_nondoubles map.
Parameters:
	- Integer key and Tile object to store within computer_valid_nondoubles map.
Return Value:
	- None
Algorithm:
	- Append new key and value pair to map by using computer_valid_nondoubles[key] = value.
Assistance Received:
	- None
********************************************************************* */
void Player::set_comp_nondouble_map(int num, Tile tile)
{
	computer_valid_nondoubles[num] = tile;
}

/* *********************************************************************
Function Name: set_mex_nondouble_map
Purpose:
	- Adds a (Int, Tile) to mexican_valid_nondoubles map.
Parameters:
	- Integer key and Tile object to store within mexican_valid_nondoubles map.
Return Value:
	- None
Algorithm:
	- Append new key and value pair to map by using mexican_valid_nondoubles[key] = value.
Assistance Received:
	- None
********************************************************************* */
void Player::set_mex_nondouble_map(int num, Tile tile)
{
	mexican_valid_nondoubles[num] = tile;
}

/* *********************************************************************
Function Name: reset_maps
Purpose:
	- Wipe all of the players valid tile maps.
Parameters:
	- None
Return Value:
	- None
Algorithm:
	- Call .clear on all private player maps.
Assistance Received:
	- None
********************************************************************* */
void Player::reset_maps()
{

	human_valid_nondoubles.clear();
	computer_valid_nondoubles.clear();
	mexican_valid_nondoubles.clear();
	human_valid_doubles.clear();
	computer_valid_doubles.clear();
	mexican_valid_doubles.clear();
}

/* *********************************************************************
Function Name: reset_turns_marker
Purpose:
	- Reinitialize turnsMarker to 1 to reset the private member.
Parameters:
	- None
Return Value:
	- None
Algorithm:
	- Set the value of the palyer's turnsMarker variable to 1.
Assistance Received:
	- None
********************************************************************* */
void Player::reset_turns_marker()
{
	turnsMarker = 1;
}

/* *********************************************************************
Function Name: get_skipped
Purpose:
	- Get skipped flag
Parameters:
	- None
Return Value:
	- boolean
Algorithm:
	- Returns skipped flag
Assistance Received:
	- None
********************************************************************* */
bool Player::get_skipped()
{
	return skipped;
}

/* *********************************************************************
Function Name: set_skipped
Purpose:
	- Update the player's skipped flag to a passed boolean.
Parameters:
	- Bool representing what value to switch the skipped flag to.
Return Value:
	- None
Algorithm:
	- Access player's skipped flag and change the parameter value to private variable.
Assistance Received:
	- None
********************************************************************* */
void Player::set_skipped(bool skip)
{
	skipped = skip;
}

/* *********************************************************************
Function Name: get_human_double_map
Purpose:
	- Get map of all valid tiles a player can play on the human train for a specific turn.
Parameters:
	- None
Return Value:
	- Map of (int, Tile) pairs representing valid Tiles that can be played on Human Train.
Algorithm:
	- Access Player's private human_valid_doubles variable.
Assistance Received:
	- None
********************************************************************* */
map<int, Tile> Player::get_human_double_map()
{
	return human_valid_doubles;
}

/* *********************************************************************
Function Name: get_comp_double_map
Purpose:
	- Get map of all valid tiles a player can play on the computer train for a specific turn.
Parameters:
	- None
Return Value:
	- Map of (int, Tile) pairs representing valid Tiles that can be played on Human Train.
Algorithm:
	- Access Player's private comp_valid_doubles variable.
Assistance Received:
	- None
********************************************************************* */
map<int, Tile> Player::get_comp_double_map()
{
	return computer_valid_doubles;
}

/* *********************************************************************
Function Name: get_mex_double_map
Purpose:
	- Get map of all valid tiles a player can play on the mexican train for a specific turn.
Parameters:
	- None
Return Value:
	- Map of (int, Tile) pairs representing valid Tiles that can be played on Human Train.
Algorithm:
	- Access Player's private mex_valid_doubles variable.
Assistance Received:
	- None
********************************************************************* */
map<int, Tile> Player::get_mex_double_map()
{
	return mexican_valid_doubles;
}

/* *********************************************************************
Function Name: set_human_double_map
Purpose:
	- Adds a (Int, Tile) to human_valid_doubles map.
Parameters:
	- Integer key and Tile object to store within human_valid_doubles map.
Return Value:
	- None
Algorithm:
	- Append new key and value pair to map by using human_valid_doubles[key] = value.
Assistance Received:
	- None
********************************************************************* */
void Player::set_human_double_map(int num, Tile tile)
{
	human_valid_doubles[num] = tile;
}

/* *********************************************************************
Function Name: set_comp_nondouble_map
Purpose:
	- Adds a (Int, Tile) to comp_valid_doubles map.
Parameters:
	- Integer key and Tile object to store within comp_valid_doubles map.
Return Value:
	- None
Algorithm:
	- Append new key and value pair to map by using comp_valid_doubles[key] = value.
Assistance Received:
	- None
********************************************************************* */
void Player::set_comp_double_map(int num, Tile tile)
{
	computer_valid_doubles[num] = tile;
}

/* *********************************************************************
Function Name: set_mex_double_map
Purpose:
	- Adds a (Int, Tile) to mex_valid_doubles map.
Parameters:
	- Integer key and Tile object to store within mex_valid_doubles map.
Return Value:
	- None
Algorithm:
	- Append new key and value pair to map by using mex_valid_doubles[key] = value.
Assistance Received:
	- None
********************************************************************* */
void Player::set_mex_double_map(int num, Tile tile)
{
	mexican_valid_doubles[num] = tile;
}

/* *********************************************************************
Function Name: set_hand
Purpose:
	- Sets hand from saved game
Parameters:
	- vector of tiles saved hand
Return Value:
	- None
Algorithm:
	- sets hand to the saved hand
Assistance Received:
	- None
********************************************************************* */
void Player::set_hand(vector<Tile> savedhand)
{
	hand = savedhand;
}

/* *********************************************************************
Function Name: add_score
Purpose:
	- Add up tiles within a players had to get the player's score.
Parameters:
	- None
Return Value:
	- None
Algorithm:
	- Call reset_players and reset_boneyard to wipe existing objects.

Assistance Received:
	- None
********************************************************************* */
void Player::add_score(Player *play)
{
	int total = 0;
	for (int i = 0; i < play->get_hand().size(); i++)
	{
		total += (play->get_hand().at(i).getleft()) + (play->get_hand().at(i).getright());
	}
	play->set_score(total);
}

/* *********************************************************************
Function Name: get if double
Purpose:
	- Gets if double flag
Parameters:
	- None
Return Value:
	- Bool
Algorithm:
	- Returns if double

Assistance Received:
	- None
********************************************************************* */
bool Player::get_if_double()
{
	return if_double;
}

/* *********************************************************************
Function Name: set if double
Purpose:
	- Sets if double flag
Parameters:
	- boolean
Return Value:
	- None
Algorithm:
	- Sets if double

Assistance Received:
	- None
********************************************************************* */
void Player::set_if_double(bool choice)
{
	if_double = choice;
}

/* *********************************************************************
Function Name: get savegame
Purpose:
	- Gets if double flag
Parameters:
	- None
Return Value:
	- Bool
Algorithm:
	- Returns savegame flag

Assistance Received:
	- None
********************************************************************* */
bool Player::get_savegame()
{
	return savegame;
}

/* *********************************************************************
Function Name: set savegame
Purpose:
	- Sets if savegame
Parameters:
	- boolean flag
Return Value:
	- Bool
Algorithm:
	- Sets savegame to flag

Assistance Received:
	- None
********************************************************************* */
void Player::set_savegame(bool flag)
{
	savegame = flag;
}

/* *********************************************************************
Function Name: get count double
Purpose:
	- Gets count double flag
Parameters:
	- None
Return Value:
	- int
Algorithm:
	- Returns count double

Assistance Received:
	- None
********************************************************************* */
int Player::get_count_double()
{
	return count_double;
}

/* *********************************************************************
Function Name: set count double
Purpose:
	- Sets count double
Parameters:
	- integer count
Return Value:
	- void
Algorithm:
	- Sets count double to the new count
Assistance Received:
	- None
********************************************************************* */
void Player::set_count_double(int count)
{
	count_double = count;
}

/* *********************************************************************
Function Name: get prev player
Purpose:
	- Gets previous player
Parameters:
	- None
Return Value:
	- Player pointer
Algorithm:
	- Returns previous player pointer

Assistance Received:
	- None
********************************************************************* */
Player *Player::get_prev_player()
{
	return previous_player;
}

/* *********************************************************************
Function Name: set prev player
Purpose:
	- Sets previous player
Parameters:
	- Player pointer
Return Value:
	- None
Algorithm:
	- Sets previous player to new player
Assistance Received:
	- None
********************************************************************* */
void Player::set_prev_player(Player *player)
{
	previous_player = player;
}

/* *********************************************************************
Function Name: set human
Purpose:
	- Sets human player
Parameters:
	- Player object pointer
Return Value:
	- None
Algorithm:
	- Sets human to the new object
Assistance Received:
	- None
********************************************************************* */
void Player::set_human(Player *oghuman)
{
	human = oghuman;
}

/* *********************************************************************
Function Name: set computer
Purpose:
	- Sets computer player
Parameters:
	- Player object pointer
Return Value:
	- None
Algorithm:
	- Sets computer to the new object
Assistance Received:
	- None
********************************************************************* */
void Player::set_computer(Player *ogcomp)
{
	computer = ogcomp;
}

/* *********************************************************************
Function Name: set boneyard
Purpose:
	- Sets boneyard
Parameters:
	- Deck pointer 
Return Value:
	- None
Algorithm:
	- Sets boneyard to new boneyard
Assistance Received:
	- None
********************************************************************* */
void Player::set_boneyard(Deck *ogboneyard)
{
	boneyard = ogboneyard;
}

/* *********************************************************************
Function Name: set mexican train
Purpose:
	- Sets mexican train
Parameters:
	- Train pointer
Return Value:
	- None
Algorithm:
	- Sets mexican train to new train
Assistance Received:
	- None
********************************************************************* */
void Player::set_mexican_train(Train *ogmextrain)
{
	mexican_train = ogmextrain;
}

/* *********************************************************************
Function Name: set curr player
Purpose:
	- Sets current player
Parameters:
	- Player object pointer
Return Value:
	- None
Algorithm:
	- Sets curr player to new current player
Assistance Received:
	- None
********************************************************************* */
void Player::set_curr_player(Player *ogcurrplayer)
{
	curr_player = ogcurrplayer;
}

/* *********************************************************************
Function Name: get round ended
Purpose:
	- Get round ended flag
Parameters:
	- None
Return Value:
	- Bool
Algorithm:
	- Returns round ended flag
Assistance Received:
	- None
********************************************************************* */
bool Player::get_round_ended()
{
	return round_ended;
}

/* *********************************************************************
Function Name: set round ended
Purpose:
	- Sets round ended flag
Parameters:
	- boolean flag
Return Value:
	- None
Algorithm:
	- Sets round ended flag to new flag
Assistance Received:
	- None
********************************************************************* */
void Player::set_round_ended(bool flag)
{
	round_ended = flag;
}

/* *********************************************************************
Function Name: set engine
Purpose:
	- Sets human player
Parameters:
	- Tile object 
Return Value:
	- None
Algorithm:
	- Sets engine to new engine
Assistance Received:
	- None
********************************************************************* */
void Player::set_engine(Tile ogengine)
{
	engine = ogengine;
}