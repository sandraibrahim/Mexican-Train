#include "GameState.h"

/* *********************************************************************
Class Name: GameState
Purpose: 
	- GameState is the highest scope of the game that contains all data
	- related to the game.
Parameters: 
	- None
Return Value:
	- New Gamestate Object
Algorithm:
	1) Define private members.
Assistance Received: 
	- None
********************************************************************* */
GameState::GameState()
{
	curr_round = new Round();
	game_over = false;
	human_total_score = 0;
	computer_total_score = 0;
}

/* *********************************************************************
Function Name: run
Purpose: 
	- run begins a game session 
Parameters: 
	- None
Return Value:
	- None
Algorithm:
	1) Starts the game loop
	2) Checks who goes first only if we arent loading a saved game
	3) Calls the run round function which will essentially run a full round and then will set the load game flag to false
	4) If the user decided to save their game, it will call the nessesary functions
	5) Adds the nessesary scores
	6) Prints round information once it ends and prompts user if they want to play again
	7) If yes, itll loop and play again, if not itll pop out of the game loop and print round final results
Assistance Received: 
	- None
********************************************************************* */
void GameState::run()
{
	int input;
	do
	{
		// Game Loop.
		if (!curr_round->get_load_game())
		{
			goes_first();
		}
		curr_round->run_round();
		curr_round->set_load_game(false);

		// save to file
		if (curr_round->get_curr_player()->get_savegame())
		{
			save_to_file();
			cout << "Your game has been saved! The file name is mextraingame.txt. Please make sure to refer to this when coming back to this game!" << endl;
			cout << "Hope you enjoyed, come finish your game soon!" << endl;
			return;
		}

		// Add local scores
		add_human_total();
		add_comp_total();

		// Round is over. Print Results.
		cout << "Human's Round Score:" << curr_round->get_human()->get_score() << endl;
		cout << "Computer's Round Score:" << curr_round->get_computer()->get_score() << endl;
		if (curr_round->get_human()->get_score() == curr_round->get_computer()->get_score())
		{
			cout << "This round is a draw!" << endl;
		}
		else if (curr_round->get_human()->get_score() < curr_round->get_computer()->get_score())
		{
			cout << "The winner of the round is the Human!" << endl;
		}
		else
		{
			cout << "The winner of the round is the Computer!" << endl;
		}
		cout << endl;

		// Prompt player if they would like to play again.
		do
		{
			cout << "Would you like to play again? 1: Yes, 2: No" << endl;
			cin >> input;
		} while (input < 1 || input > 2);

		// Player chooses to stop playing game. Else reset players and play again.
		if (input == 2)
		{
			set_game_over();
		}
		else
		{
			curr_round->set_round_num();
			curr_round->reset_players();
			curr_round->get_human()->assign_score(human_total_score);
			curr_round->get_computer()->assign_score(computer_total_score);
		}
	} while (!game_over);

	cout << "Game is over!" << endl;
	cout << "Final Scores:" << endl;
	cout << "Human: " << get_human_total() << endl;
	cout << "Computer: " << get_comp_total() << endl;

	if (get_human_total() < get_comp_total())
	{
		cout << "The human player has won! Congrats! (:" << endl;
	}
	else if (get_human_total() > get_comp_total())
	{
		cout << "The computer player has won! Congrats! (:" << endl;
	}
	else
	{
		cout << "Both scores are equal. It's a tie! :O" << endl;
	}
}

/* *********************************************************************
Function Name: coin_toss
Purpose:
	- Returns the result of a coin toss to determine first player.
Parameters:
	- None
Return Value:
	- Integer where 1 is heads and 2 is tails.
Algorithm:
	1) Call random number generator to generate a number
	2) Mod the number by 2 and if it is equal to 1, return 1, else return 2.
Assistance Received:
	- None
********************************************************************* */
int GameState::coin_toss()
{
	int coin;
	srand(time(NULL));
	coin = rand() % 2;
	if (coin == 1)
	{
		return 1;
	}
	else
	{
		return 2;
	}
}

/* *********************************************************************
Function Name: goes_first
Purpose:
	- Determines which player goes first and assigns first player to the
	  curr_player round attribute.
Parameters:
	- None
Return Value:
	- None
Algorithm:
	1) Determines which player has the higher score. The one with the highest score goes first.
	2) If it is a draw, both players will do a coin toss to determine who will go first.
	3) Set the current player member variable wihtin the round class to be the winner
	   of the coin toss.
Assistance Received:
	- None
********************************************************************* */
void GameState::goes_first()
{
	int toss;
	int call;
	//if scores =, coin toss
	if (get_human_total() == get_comp_total())
	{
		do
		{
			cout << "The scores are currently tied, we will flip a coin to determine who goes first!" << endl;
			cout << "Please call the coin toss by entering the number." << endl;
			cout << "1. Heads" << endl;
			cout << "2. Tails" << endl;
			cin >> call;
		} while (call != 1 && call != 2);
		toss = coin_toss();
		// if toss is equal to call, set to player 1/human
		if (toss == call)
		{
			curr_round->set_curr_player(curr_round->get_human());
			curr_round->get_curr_player()->set_prev_player(curr_round->get_human());
			cout << "The Human won the coin toss! They will be going first." << endl;
			cout << "Press any key to continue..." << endl;
			system("read");
			system("clear");
		}
		else
		{
			curr_round->set_curr_player(curr_round->get_computer());
			curr_round->get_curr_player()->set_prev_player(curr_round->get_computer());
			cout << "The Computer won the coin toss! They will be going first." << endl;
			cout << "Press any key to continue..." << endl;
			system("read");
			system("clear");
		}
	}
	// if player 1 score < player 2 score, player 1 goes first
	else if (get_human_total() < get_comp_total())
	{
		curr_round->set_curr_player(curr_round->get_human());
		cout << "The Human has a lower score! They will be going first." << endl;
	}
	else
	{
		curr_round->set_curr_player(curr_round->get_computer());
		cout << "The Computer has a lower score! They will be going first." << endl;
	}
}

/* *********************************************************************
Function Name: save to file
Purpose:
	- Saves a game to a file for a player to come back and play
Parameters:
	- None
Return Value:
	- None
Algorithm:
	1) Uses ofstream to save the current game state to a file in a specific format
Assistance Received:
	- None
********************************************************************* */
void GameState::save_to_file()
{
	// Writes to file
	ofstream SaveGame("mextraingame.txt");
	SaveGame << "Round: " << to_string(curr_round->get_round_num()) << endl;
	SaveGame << endl;
	SaveGame << "Computer: " << endl;
	SaveGame << "  Score: " << to_string(computer_total_score) << endl;
	SaveGame << "  Hand: " << convert_to_string(curr_round->get_computer()->get_hand()) << endl;
	SaveGame << "  Train: " << convert_to_string(curr_round->get_computer()->get_train()->get_curr_train());
	if (curr_round->get_computer()->get_train()->get_marker())
	{
		SaveGame << "M" << endl;
	}
	else
	{
		cout << endl;
	}
	SaveGame << endl;
	SaveGame << "Human: " << endl;
	SaveGame << "  Score: " << to_string(human_total_score) << endl;
	SaveGame << "  Hand: " << convert_to_string(curr_round->get_human()->get_hand()) << endl;
	SaveGame << "  Train: " << convert_to_string(curr_round->get_human()->get_train()->get_curr_train()) << endl;
	if (curr_round->get_human()->get_train()->get_marker())
	{
		SaveGame << "M" << endl;
	}
	else
	{
		cout << endl;
	}
	SaveGame << endl;
	SaveGame << "Mexican Train: " << convert_to_string(curr_round->get_mex_train()->get_curr_train()) << endl;
	SaveGame << endl;
	SaveGame << "Boneyard: " << convert_to_string(curr_round->get_boneyard()->get_boneyard()) << endl;
	SaveGame << endl;
	SaveGame << "Next Player: ";
	if (curr_round->get_curr_player() == curr_round->get_human())
	{
		SaveGame << "Computer" << endl;
	}
	else
	{
		SaveGame << "Human" << endl;
	}
	SaveGame.close();
}

/* *********************************************************************
Function Name: start saved game
Purpose:
	- Read everything in a file and populates the nessesary objects and variables
Parameters:
	- string variable names game
Return Value:
	- None
Algorithm:
	1) Uses if stream to get each specific piece of information (round number, boneyard, etc)
	2) Uses .find to find the : character and copies everything after it into temporary variables
	3) Re-fills variables and returns to start the game
Assistance Received:
	- None
********************************************************************* */
void GameState::start_saved_game(string game)
{

	ifstream saved_game(game);
	vector<string> objects;
	string temp;
	string delimiter = ":";
	int round;
	vector<Tile> comp_hand;
	vector<Tile> hum_hand;
	vector<Tile> comp_train;
	vector<Tile> hum_train;
	vector<Tile> mex_train;
	vector<Tile> bone;
	int hum_score;
	int comp_score;
	Player *curr_player;
	Player *next_player;
	Tile engine;
	int engine_num;
	int wanted_mex_num;
	bool markflag1 = false;
	bool markflag2 = false;
	string data;

	// Parse Data. Put into big string variable, handle line by line, etc.
	while (!saved_game.eof())
	{
		getline(saved_game, data);
		objects.push_back(data);
	}

	// find round number
	int pos = objects.at(0).find(delimiter);
	temp = objects.at(0).substr(pos + 2);
	round = stoi(temp);

	// find comp score
	pos = objects.at(3).find(delimiter);
	temp = objects.at(3).substr(pos + 2);
	comp_score = stoi(temp);

	// find comp hand
	pos = objects.at(4).find(delimiter);
	temp = objects.at(4).substr(pos + 2);
	comp_hand = parse_tile_vector(temp, comp_hand);

	// find comp train
	pos = objects.at(5).find(delimiter);
	temp = objects.at(5).substr(pos + 2);
	char begin_of_string = temp.at(0);
	if (begin_of_string == 'M')
	{
		markflag1 = true;
	}
	comp_train = parse_tile_vector(temp, comp_train);

	// find human score
	pos = objects.at(7).find(delimiter);
	temp = objects.at(7).substr(pos + 2);
	hum_score = stoi(temp);

	// find human hand
	pos = objects.at(8).find(delimiter);
	temp = objects.at(8).substr(pos + 2);
	hum_hand = parse_tile_vector(temp, hum_hand);

	// find human train
	pos = objects.at(9).find(delimiter);
	temp = objects.at(9).substr(pos + 2);
	char end_of_string = temp.at(temp.length() - 2);
	if (end_of_string == 'M')
	{
		markflag2 = true;
	}
	hum_train = parse_tile_vector(temp, hum_train);

	// find mex train
	pos = objects.at(11).find(delimiter);
	temp = objects.at(11).substr(pos + 2);
	mex_train = parse_tile_vector(temp, mex_train);

	// find boneyard
	pos = objects.at(13).find(delimiter);
	temp = objects.at(13).substr(pos + 2);
	bone = parse_tile_vector(temp, bone);

	// find next player
	size_t found = objects.at(15).find("Computer");
	if (found != string::npos)
	{
		curr_round->set_curr_player(curr_round->get_human());
		curr_round->get_curr_player()->set_prev_player(curr_round->get_computer());
	}
	else
	{
		curr_round->set_curr_player(curr_round->get_computer());
		curr_round->get_curr_player()->set_prev_player(curr_round->get_human());
	}

	engine = hum_train.at(0);
	engine_num = hum_train.at(0).getleft();
	int wanted_comp_num = comp_train.at(0).getleft();
	int wanted_hum_num = hum_train.at(hum_train.size() - 1).getright();

	if (mex_train.size() == 0)
	{
		wanted_mex_num = 9;
	}
	else
	{
		wanted_mex_num = mex_train.at(mex_train.size() - 1).getright();
	}

	curr_round->set_saved_boneyard(bone);
	curr_round->set_saved_curr_player(curr_round->get_curr_player());
	curr_round->set_saved_mex_train(mex_train);
	curr_round->set_saved_round_num(round);
	curr_round->set_saved_curr_engine_num(engine_num);
	curr_round->set_saved_engine(engine);
	curr_round->set_saved_hum_train(hum_train);
	if (markflag2)
	{
		curr_round->get_human()->get_train()->change_marker(true);
		curr_round->get_human()->set_turns_marker();
	}
	curr_round->set_saved_comp_train(comp_train);
	if (markflag1)
	{
		curr_round->get_computer()->get_train()->change_marker(true);
		curr_round->get_computer()->set_turns_marker();
	}
	curr_round->set_saved_hum_hand(hum_hand);
	curr_round->set_saved_comp_hand(comp_hand);
	curr_round->get_human()->get_train()->set_num(wanted_hum_num);
	curr_round->get_computer()->get_train()->set_num(wanted_comp_num);
	curr_round->get_mex_train()->set_num(wanted_mex_num);
	curr_round->set_load_game(true);
	curr_round->get_human()->assign_score(hum_score);
	curr_round->get_computer()->assign_score(comp_score);
}

/* *********************************************************************
Function Name: convert to string
Purpose:
	- Converts vector of Tiles into strings
Parameters:
	- Vector of tiles
Return Value:
	- None
Algorithm:
	1) Loops and adds to a string the left and right side of a tile seperated with a - character
Assistance Received:
	- None
********************************************************************* */
string GameState::convert_to_string(vector<Tile> obj)
{
	string converted;
	for (int i = 0; i < obj.size(); i++)
	{
		converted += to_string(obj.at(i).getleft()) + " - " + to_string(obj.at(i).getright()) + " ";
	}
	return converted;
}

/* *********************************************************************
Function Name: is empty
Purpose:
	- Checks if a file is empty
Parameters:
	- string which is the name of the file
Return Value:
	- boolean
Algorithm:
	1) Uses ifstream and checks if the file is empty using peek.
	2) If it returns EOF, it is empty and returns true. If it returns anything else, it returns false.
Assistance Received:
	- None
********************************************************************* */
bool GameState::is_empty(string game)
{
	ifstream file(game);
	return (file.peek() == EOF);
}

/* *********************************************************************
Function Name: parse tile vector
Purpose:
	- Parses the data string that has all of the tiles as strings and parses it into a vector of Tile objects
Parameters:
	- string variable named data which has the tiles in string format, vector of Tiles
Return Value:
	- Vector of Tiles
Algorithm:
	1) Goes through the string character by character and checks if the character is a digit
	2) If it is a digit, it converts it to an integer and adds to the Tile left and right, and adds to vector
	3) Returns finished vector
Assistance Received:
	- None
********************************************************************* */
vector<Tile> GameState::parse_tile_vector(string data, vector<Tile> vect)
{
	int left_tile = -1;
	int right_tile = -1;

	for (int i = 0; i < data.length(); i++)
	{
		if (isdigit(data.at(i)))
		{
			if (left_tile == -1)
			{
				left_tile = stoi(string(1, data.at(i)));
			}
			else
			{
				right_tile = stoi(string(1, data.at(i)));
				vect.push_back(Tile(left_tile, right_tile));
				left_tile = -1;
				right_tile = -1;
			}
		}
	}
	return vect;
}

// -------------------Getters/Setters-------------------
/* *********************************************************************
Function Name: set_game_over
Purpose:
	- Flips game over boolean flag within Gamestate.
Parameters:
	- None
Return Value:
	- None
Algorithm:
	1) Convert the boolean member function to opposite.
Assistance Received:
	- None
********************************************************************* */
void GameState::set_game_over()
{
	game_over = !(game_over);
}

/* *********************************************************************
Function Name: get_human_total
Purpose:
	- Retrieves the total score for the human player.
Parameters:
	- None
Return Value:
	- Returns the integer representing total human player score.
Algorithm:
	- Access GameState object's private variable, 'human_total_score'
Assistance Received:
	- None
********************************************************************* */
int GameState::get_human_total()
{
	return human_total_score;
}

/* *********************************************************************
Function Name: add_human_total
Purpose:
	- Retrieves the total score for the human player.
Parameters:
	- None
Return Value:
	- None
Algorithm:
	- Access GameState object's private variable, 'human_total_score' and 
	  add Human player's score.
Assistance Received:
	- None
********************************************************************* */
void GameState::add_human_total()
{
	human_total_score = curr_round->get_human()->get_score();
}

/* *********************************************************************
Function Name: get_comp_total
Purpose:
	- Retrieves the total score for the computer player.
Parameters:
	- None
Return Value:
	- Returns the integer representing total computer player score.
Algorithm:
	- Access GameState object's private variable, 'computer_total_score'
Assistance Received:
	- None
********************************************************************* */
int GameState::get_comp_total()
{
	return computer_total_score;
}

/* *********************************************************************
Function Name: add_comp_total
Purpose:
	- Retrieves the total score for the computer player.
Parameters:
	- None
Return Value:
	- None
Algorithm:
	- Access GameState object's private variable, 'computer_total_score' and 
	  add Computer player's score.
Assistance Received:
	- None
********************************************************************* */
void GameState::add_comp_total()
{
	computer_total_score = curr_round->get_computer()->get_score();
}
