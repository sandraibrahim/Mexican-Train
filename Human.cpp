#include "Human.h"

/* *********************************************************************
Class Name: Human
Purpose:
	- Doesn't have specific private variables but inhertis from the player class.
Parameters:
	- None
Return Value:
	- Returns a player object
Algorithm:
	- None
Assistance Received:
	- None
********************************************************************* */
Human::Human()
{
}

/* *********************************************************************
Function Name: play
Purpose:
	- calls the menu function to start the humans turn
Parameters:
	- None
Return Value:
	- None
Algorithm:
	- None
Assistance Received:
	- None
********************************************************************* */
void Human::play()
{
	// Enter menu and use standard player logic.
	menu();
}

/* *********************************************************************
Function Name: menu
Purpose:
	- Prompts user to choose what they would like to do and calls the proper function to execute. Loops as needed
Parameters:
	- None
Return Value:
	- None
Algorithm:
	1) Prompt player to choose between 4 different menu options.
	   - 1 will save the game.
	   - 2 will make a move.
	   - 3 will make a suggestion.
	   - 4 will quit the round.
	2) The chosen option will be put into a switch case.
	3) Calls function related to the menu option
	4) Loops as nessesary
Assistance Received:
	- None
********************************************************************* */
void Human::menu()
{
	int option;
	bool repeat = false;
	do
	{
		cout << "What would you like to do?" << endl;
		cout << "1. Save the game." << endl;
		cout << "2. Make a move. " << endl;
		cout << "3. Ask for help. " << endl;
		cout << "4. Quit the game" << endl;
		cout << "Please enter a number: ";
		cin >> option;
		cout << endl;

		switch (option)
		{
		case 1:
			save();
			repeat = false;
			break;
		case 2:
			move();
			repeat = false;
			break;
		case 3:
			help();
			repeat = true;
			break;
		case 4:
			quit();
			repeat = false;
			break;

		default:
			cout << "Invalid Input" << endl;
			repeat = true;
			break;
		}
	} while (repeat);
}

/* *********************************************************************
Function Name: help
Purpose:
	- Prints out a suggestion for the user to play tile using the computers logic
Parameters:
	- None
Return Value:
	- None
Algorithm:
	1) Gets all the valid tiles for the current player
	2) Calls the move calculator function which will calulate which move the player shoud make and output it. 
Assistance Received:
	- None
********************************************************************* */
void Human::help()
{
	cout << "I suggest you ";
	return_valid_tiles(curr_player);
	pair<Tile, Train *> curr_move = move_calc();
}

/* *********************************************************************
Function Name: move
Purpose:
	- Make appropriate move
Parameters:
	- None
Return Value:
	- None
Algorithm:
	1) Checks if the previous player is the same as the current to see if they are playing a double. 
	2) If they arent it resets the turns marker and then gets all the valid tiles for the player
	3) It then checks if we need to draw from the boneyard by seeing if theres no available tiles to play.
	4) If so, it goes through the logic for drawing from the boneyard and checks if it's playable and plays it,
	else, it adds a marker and moves on.
	5) It then prompts user to choose which tile theyd like to play, and plays it.
	6) If player was initially skipped, reset skip flag.
	7) Add Tile to train and remove tile from hand.
	8) If tile played is a double, increment number of doubles and make another move.
	9) If cannot play more than 2 doubles in a row or it's not a double, change double flag to false and the count to 0.
Assistance Received:
	- None
********************************************************************* */
void Human::move()
{
	int tile_num;
	int train_type;
	Train *type = new Train();
	bool repeat = false;
	Tile tile = Tile();
	map<int, Tile> temp;
	int input;

	if (get_prev_player() != curr_player)
	{
		curr_player->reset_turns_marker();
	}
	return_valid_tiles(curr_player);

	// Check if we need to grab from boneyard.
	if (if_draw(curr_player))
	{
		if (curr_player->get_hand().size() == 0 && get_if_double())
		{
			cout << "Player has exhausted hand after double." << endl;
			round_ended = true;
			add_score(human);
			add_score(computer);
			return;
		}
		cout << "You must draw from boneyard, no available tiles to play." << endl;
		if (boneyard->get_boneyard().size() > 0)
		{
			tile = boneyard->draw_tile_from_boneyard();
			curr_player->draw_tile(tile);
			cout << "Tile " << tile << " was drawn." << endl;
			if (isvalid(tile, curr_player, human->get_train()) || isvalid(tile, curr_player, computer->get_train()) || isvalid(tile, curr_player, mexican_train))
			{
				play_valid_tile(tile, curr_player);
				cout << "Tile drawn can be played on an available train. Tile has been added to train." << endl;
				cout << "Press any key to continue..." << endl;
				system("read");
				return;
			}
			else
			{
				curr_player->get_train()->change_marker(true);
				curr_player->set_turns_marker();
				cout << "Tile drawed is not valid to play on any trains. Marker has been added to your train." << endl;
				cout << "Press any key to continue..." << endl;
				system("read");
				return;
			}
		}
		else
		{
			cout << "Boneyard is empty, Marker has been added to your train. Turn passed." << endl;
			curr_player->get_train()->change_marker(true);
			curr_player->set_skipped(true);
			cout << "Press any key to continue..." << endl;
			system("read");
			return;
		}
	}
	if (second_invalid_double)
	{
		cout << "You have a valid double to play next, however no valid follow up tile. Unable to play tile." << endl;
	}

	do
	{
		print_options(curr_player);
		set_if_double(false);
		cout << "What train would you like to play on: ";
		cin >> train_type;
		cout << endl;
		switch (train_type)
		{
			// Human Train chosen.
		case 1:
			type = human->get_train();
			input = which_map();
			if (input == 1)
			{
				temp = curr_player->get_human_nondouble_map();
			}
			else
			{
				temp = curr_player->get_human_double_map();
				set_if_double(true);
			}
			repeat = false;
			break;
			// Computer Train chosen.
		case 2:
			type = computer->get_train();
			input = which_map();
			if (input == 1)
			{
				temp = curr_player->get_comp_nondouble_map();
			}
			else
			{
				temp = curr_player->get_comp_double_map();
				set_if_double(true);
			}
			repeat = false;
			break;
			// Mexican Train chosen.
		case 3:
			type = mexican_train;
			input = which_map();
			if (input == 1)
			{
				temp = curr_player->get_mex_nondouble_map();
			}
			else
			{
				temp = curr_player->get_mex_double_map();
				set_if_double(true);
			}
			repeat = false;
			break;
			// Invalid Choice.
		default:
			cout << "Invalid Input" << endl;
			repeat = true;
			break;
		}
	} while (repeat || temp.size() == 0);
	print_valid_tiles(curr_player, train_type, input);
	cout << endl;

	// Pick from available tiles and retrieve tile's index.
	do
	{
		cout << "Please pick a tile from the options." << endl;
		cout << "Tile #: ";
		cin >> tile_num;
	} while (tile_num < 0 || tile_num > temp.size());
	int location = find_tile_index(tile_num, temp, curr_player);

	// If tile played was on personal marked train, remove marker.
	if (type == curr_player->get_train() && curr_player->get_train()->get_marker())
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
	type->add_to_train(curr_player->get_hand().at(location), type->get_side());
	curr_player->remove_from_hand(location);

	// If tile played is a double, increment number of doubles and make another move.
	if (if_double && count_double < 3)
	{
		set_prev_player(curr_player);
		type->set_is_double(true);
		set_count_double(get_count_double() + 1);
		reset_maps();
		cout << endl;
		cout << "Player played a Double! You get another move!" << endl;
		cout << endl;
		move();
	}
	// Cannot play more than 2 doubles in a row or it's not a double.
	else
	{
		type->set_is_double(false);
		set_count_double(0);
	}
	curr_player->set_turns_marker();
	second_invalid_double = false;
}

/* *********************************************************************
Function Name: save
Purpose:
	- Set save game flag to true
Parameters:
	- None
Return Value:
	- None
Algorithm:
	1) Set save game flag to true
Assistance Received:
	- None
********************************************************************* */
void Human::save()
{
	savegame = true;
}
