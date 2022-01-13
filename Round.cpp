#include "Round.h"

/* *********************************************************************
Class Name: Round
Purpose:
	- Class Round holds all game information relating to the round. The boneyard,
	  players, mexican/whole train, engine and round details are stored in class.
Parameters:
	- None
Return Value:
	- Returns a Round object after constructing private members.
Algorithm:
	- None
Assistance Received:
	- None
********************************************************************* */
Round::Round()
{
	boneyard = new Deck();
	round_num = 1;
	curr_engine_num = 9;
	engine = Tile(9, 9);
	human = new Human();
	computer = new Computer();
	mexican_train = new Train();
	curr_player = new Player();
	load_game = false;
}

/* *********************************************************************
Function Name: initialize_game
Purpose:
	- Initializes all informations for players and round to begin the game.
Parameters:
	- None
Return Value:
	- New Gamestate Object
Algorithm:
	1) Define the round's current engine.
	2) Initialize player members.
	3) Initialize round information.
	4) Print results of Initialization.
Assistance Received:
	- None
********************************************************************* */
void Round::initialize_game()
{
	reset_game();
	set_engine();
	human->get_train()->set_num(get_engine_num());
	computer->get_train()->set_num(get_engine_num());
	get_mex_train()->set_num(get_engine_num());
	human->set_identifier(false);
	human->get_train()->set_side(false);
	computer->set_identifier(true);
	computer->get_train()->set_side(true);
	human->add_train(get_engine());
	computer->add_train(get_engine());
	get_mex_train()->set_side(false);
	boneyard->shuffle_boneyard();
	deal_hands(human);
	deal_hands(computer);

	// Change engine num.
	set_curr_engine_num();
}

/* *********************************************************************
Function Name: run round
Purpose: 
	- run begins a game session which calls all necesary functions and
	  game logic to play through the game.
Parameters: 
	- None
Return Value:
	- None
Algorithm:
	1) Initialize game by calling the initialize_game function
	2) Determine who the current player is.
	3) Print current Player's Information.
	4) Calls play function
	5) Repeat until round is over.
	6) If player wants to play again, reinitialize round attributes and repeat
	   until player does not want to play anymore.
Assistance Received: 
	- None
********************************************************************* */
void Round::run_round()
{
	if (!load_game)
	{
		initialize_game();
	}
	do
	{
		if (curr_player == human)
		{
			cout << "It is the human's turn!" << endl;
		}
		else
		{
			cout << "It is the computer's turn!" << endl;
		}
		cout << "---------------------------------------------------------------------------------------------------------" << endl;
		cout << "Round: " << get_round_num() << endl;
		cout << endl;

		cout << "Computer:" << endl;
		cout << "	Score: " << computer->get_score() << endl;
		cout << "	Hand: ";
		computer->print_hands();
		cout << "	Train: ";
		computer->print_trains();
		cout << endl;

		cout << "Human:" << endl;
		cout << "	Score: " << human->get_score() << endl;
		cout << "	Hand: ";
		human->print_hands();
		cout << "	Train: ";
		human->print_trains();
		cout << endl;

		cout << "Mexican Train: ";
		mexican_train->print_train();
		cout << endl;
		cout << endl;

		cout << "Boneyard: ";
		if (boneyard->get_boneyard().size() == 0)
		{
			cout << "No more tiles." << endl;
		}
		else
		{
			cout << boneyard->get_boneyard().at(0) << endl;
		}
		cout << endl;

		cout << "Next Player: ";
		if (curr_player == human)
		{
			cout << "Computer" << endl;
			curr_player->set_prev_player(computer);
		}
		else
		{
			cout << "Human" << endl;
			curr_player->set_prev_player(human);
		}
		cout << endl;

		curr_player->set_curr_player(curr_player);

		// Update Player's pointers.
		curr_player->set_human(human);
		curr_player->set_computer(computer);
		curr_player->set_boneyard(boneyard);
		curr_player->set_mexican_train(mexican_train);
		curr_player->set_engine(engine);

		// Play Turn
		curr_player->play();

		// Check if game is over.
		if (round_over() || curr_player->get_round_ended())
		{
			human->add_score(human);
			computer->add_score(computer);
			set_is_round_over(true);
		}
		// Save game if flag true and quit.
		else if (curr_player->get_savegame())
		{
			set_is_round_over(true);
		}
		// Switch Players at end of round.
		else
		{
			if (curr_player == human)
			{
				curr_player = computer;
				curr_player->set_prev_player(human);
			}
			else
			{
				curr_player = human;
				curr_player->set_prev_player(computer);
			}

			// Reset valid tiles at end of round.
			curr_player->reset_maps();
		}
		system("clear");
		cout << "---------------------------------------------------------------------------------------------------------" << endl;

	} while (!get_is_round_over());
}

/* *********************************************************************
Function Name: round_over
Purpose:
	- Determines if the game is over and adds the appropriate scores to players.
Parameters:
	- Round object containing all information of the current round.
Return Value:
	- Bool that represents whether the current round is over or not.
Algorithm:
	1) Check if any player's hand is empty. If so, add to appropriate
	   Player's score then return true.
	2) Check if boneyard is empty and if both players have been skipped.
	3) Return True
	4) If end of function is reached return false.
Assistance Received:
	- None
********************************************************************* */
bool Round::round_over()
{
	// If one player runs out of tiles.
	if (human->get_hand().size() == 0)
	{
		computer->add_score(computer);
		return true;
	}
	else if (computer->get_hand().size() == 0)
	{
		human->add_score(human);
		return true;
	}
	// Check if boneyard is empty AND if both players have been skipped once.
	// Add skipped is_round_over to players to see if they have been skipped.
	// Check if boneyard is empty first then if both players "skipped" is_round_over are true.
	// If so add score to both(?) players then return true.
	if (get_boneyard()->get_boneyard().size() == 0 && curr_player->get_skipped() && curr_player->get_opp_player(curr_player)->get_skipped())
	{
		cout << "Boneyard is empty and both players have skipped their turns. Round is over." << endl;
		human->add_score(human);
		computer->add_score(computer);
		return true;
	}

	return false;
}

/* *********************************************************************
Function Name: reset_game
Purpose:
	- Reinitialize Round attributes to reset the game.
Parameters:
	- None
Return Value:
	- None
Algorithm:
	- Call reset_players and reset_boneyard to wipe existing objects.
Assistance Received:
	- None
********************************************************************* */
void Round::reset_game()
{
	reset_boneyard();
	mexican_train = new Train();
	set_is_round_over(false);
}

/* *********************************************************************
Function Name: reset_boneyard
Purpose:
	- Reinitialize the Boneyard.
Parameters:
	- None
Return Value:
	- None
Algorithm:
	- Access Round object's private integer data member, 'boneyard' and
	  set equal to a new Deck object.
Assistance Received:
	- None
********************************************************************* */
void Round::reset_boneyard()
{
	boneyard = new Deck();
}

/* *********************************************************************
Function Name: reset_players
Purpose:
	- Reset both the Human and Computer player objects.
Parameters:
	- None
Return Value:
	- None
Algorithm:
	- Access Round object's private integer data members, 'human' and 'computer'
	  and set them equal to new Player objects
Assistance Received:
	- None
********************************************************************* */
void Round::reset_players()
{
	human = new Human();
	computer = new Computer();
}

/* *********************************************************************
Function Name: deal_hands
Purpose:
	- Deal hands to current player from tiles from the boneyard.
Parameters:
	- None
Return Value:
	- None
Algorithm:
	1) Call Player object's draw_tile funciton
	2) Pass in popped Tile from boneyard.
Assistance Received:
	- None
********************************************************************* */
void Round::deal_hands(Player *player)
{
	for (int i = 0; i < 16; i++)
	{
		player->draw_tile(boneyard->draw_tile_from_boneyard());
	}
}

// -------------------Output Helper Functions-------------------
/* *********************************************************************
Function Name: print_round_info
Purpose:
	- Prints all related round information.
Parameters:
	- None
Return Value:
	- None
Algorithm:
	1) Print the Round number, Engine number, engine, and call print_boneyard
Assistance Received:
	- None
********************************************************************* */
void Round::print_round_info()
{
	cout << "Round: " << round_num << endl;
	cout << "Current Engine Number: " << curr_engine_num << endl;
	cout << "Current Engine: " << engine.getleft() << " - " << engine.getright() << endl;
	boneyard->print_boneyard();
}

// -------------------Getter/Setter Functions-------------------
/* *********************************************************************
Function Name: set_round_num
Purpose:
	- Adds one to the current round number.
Parameters:
	- None
Return Value:
	- None
Algorithm:
	- Access Round object's private integer data member, 'curr_engine_num' and add one.
Assistance Received:
	- None
********************************************************************* */
void Round::set_round_num()
{
	round_num += 1;
}

/* *********************************************************************
Function Name: get_engine_num
Purpose:
	- Returns an integer that represents the current engine of the Round.
Parameters:
	- None
Return Value:
	- Integer that represent the current Round object's engine.
Algorithm:
	- Access Round object's private data member, 'curr_engine_num'
Assistance Received:
	- None
********************************************************************* */
int Round::get_engine_num()
{
	return curr_engine_num;
}

/* *********************************************************************
Function Name: set_curr_engine_num
Purpose:
	- Decrement the current engine number. If the current engine number is at 0, start again from 9.
Parameters:
	- None
Return Value:
	- None
Algorithm:
	1) If the current engine number is greater than 0, subtract the engine number by 1.
	2) Else reset the current engine number to 9.
Assistance Received:
	- None
********************************************************************* */
void Round::set_curr_engine_num()
{
	if (curr_engine_num > 0)
	{
		curr_engine_num -= 1;
	}
	else
	{
		curr_engine_num = 9;
	}
}

/* *********************************************************************
Function Name: get_engine
Purpose:
	- Return the engine of the Round.
Parameters:
	- None
Return Value:
	- Tile that represents the Round's engine.
Algorithm:
	- Access Round object's private data member, 'engine'
Assistance Received:
	- None
********************************************************************* */
Tile Round::get_engine()
{
	return engine;
}

/* *********************************************************************
Function Name: set_engine
Purpose:
	- Remove the engine tile from the boneyard and set it as the current engine of the round.
Parameters:
	- None
Return Value:
	- None
Algorithm:
	- Access Round object's private integer data member, 'engine' and
	  call pop_engine to remove the tile from the boneyard and set equal to engine.
Assistance Received:
	- None
********************************************************************* */
void Round::set_engine()
{
	engine = boneyard->pop_engine(curr_engine_num);
}

/* *********************************************************************
Function Name: get_engine_num
Purpose:
	- Returns the round's Mexican Train.
Parameters:
	- None
Return Value:
	- Train object that represents the Mexican train.
Algorithm:
	- Access Round object's private data member, 'mexican_train'
Assistance Received:
	- None
********************************************************************* */
Train *Round::get_mex_train()
{
	return mexican_train;
}

/* *********************************************************************
Function Name: get_boneyard
Purpose:
	- Returns the object storing the boneyard.
Parameters:
	- None
Return Value:
	- Deck object that represents the game's boneyard.
Algorithm:
	- Access Round object's private data member, 'boneyard'
Assistance Received:
	- None
********************************************************************* */
Deck *Round::get_boneyard()
{
	return boneyard;
}

/* *********************************************************************
Function Name: get_is round over
Purpose:
	- Returns a boolean that represents if the round is over
Parameters:
	- None
Return Value:
	- A boolean that represents if the round is over
Algorithm:
	- Return is round over flag
Assistance Received:
	- None
********************************************************************* */
bool Round::get_is_round_over()
{
	return is_round_over;
}

/* *********************************************************************
Function Name: set_is round over
Purpose:
	- Set flag to specific boolean.
Parameters:
	- None
Return Value:
	- None
Algorithm:
	- Access Round object's private integer data member, 'flag' and
	  set equal to parameter
Assistance Received:
	- None
********************************************************************* */
void Round::set_is_round_over(bool newflag)
{
	is_round_over = newflag;
}

/* *********************************************************************
Function Name: get_curr_player
Purpose:
	- Return the current player of this turn.
Parameters:
	- None
Return Value:
	- Pointer to Player object that represents the current player
Algorithm:
	- Access Round object's private data member, 'curr_player'
Assistance Received:
	- None
********************************************************************* */
Player *Round::get_curr_player()
{
	return curr_player;
}

/* *********************************************************************
Function Name: set_curr_player
Purpose:
	- Set current player to specific Player object.
Parameters:
	- Pointer to a player object that represents the current player of this turn.
Return Value:
	- None
Algorithm:
	- Access Round object's private integer data member, 'curr_player' and
	  set equal to parameter
Assistance Received:
	- None
********************************************************************* */
void Round::set_curr_player(Player *curr)
{
	curr_player = curr;
}

/* *********************************************************************
Function Name: get_computer
Purpose:
	- Return computer object player pointer
Parameters:
	- None
Return Value:
	- Pointer to Player object that represents the computer player
Algorithm:
	- Returns the current computer player object player pointer
Assistance Received:
	- None
********************************************************************* */
Player *Round::get_computer()
{
	return this->computer;
}

/* *********************************************************************
Function Name: get_human
Purpose:
	- Return human object player pointer
Parameters:
	- None
Return Value:
	- Pointer to Player object that represents the human player
Algorithm:
	- Returns the current human player object player pointer
Assistance Received:
	- None
********************************************************************* */
Player *Round::get_human()
{
	return this->human;
}

/* *********************************************************************
Function Name: get_round_num
Purpose:
	- Return round number
Parameters:
	- None
Return Value:
	- integer round number
Algorithm:
	- Returns round number
Assistance Received:
	- None
********************************************************************* */
int Round::get_round_num()
{
	return round_num;
}

/* *********************************************************************
Function Name: set_saved_boneyard
Purpose:
	- Set saved boneyard
Parameters:
	- vector of tiles (new boneyard)
Return Value:
	- None
Algorithm:
	- Sets the boneyard to new one
Assistance Received:
	- None
********************************************************************* */
void Round::set_saved_boneyard(vector<Tile> bone)
{
	boneyard->set_boneyard(bone);
}

/* *********************************************************************
Function Name: set_saved_curr_player
Purpose:
	- Set current player
Parameters:
	- Player pointer
Return Value:
	- None
Algorithm:
	- Sets current player to new curr player
Assistance Received:
	- None
********************************************************************* */
void Round::set_saved_curr_player(Player *curr)
{
	curr_player = curr;
}

/* *********************************************************************
Function Name: set_saved_mex_train
Purpose:
	- Set saved mexican train
Parameters:
	- vector of tiles (new mex train)
Return Value:
	- None
Algorithm:
	- Sets the mex train to new one
Assistance Received:
	- None
********************************************************************* */
void Round::set_saved_mex_train(vector<Tile> mex)
{
	mexican_train->set_train(mex);
}

/* *********************************************************************
Function Name: set_saved_round_num
Purpose:
	- Set saved round number
Parameters:
	- integer new round num
Return Value:
	- None
Algorithm:
	- Sets the round number to new one
Assistance Received:
	- None
********************************************************************* */
void Round::set_saved_round_num(int num)
{
	round_num = num;
}

/* *********************************************************************
Function Name: set_saved_curr_engine_num
Purpose:
	- Set saved engine number
Parameters:
	- integer new engine num
Return Value:
	- None
Algorithm:
	- Sets curr engine num to new one
Assistance Received:
	- None
********************************************************************* */
void Round::set_saved_curr_engine_num(int num)
{
	curr_engine_num = num;
}

/* *********************************************************************
Function Name: set_saved_engine
Purpose:
	- Set saved engine
Parameters:
	- Tile new engine
Return Value:
	- None
Algorithm:
	- Sets the engine to new one
Assistance Received:
	- None
********************************************************************* */
void Round::set_saved_engine(Tile eng)
{
	engine = eng;
}

/* *********************************************************************
Function Name: set_saved_hum_train
Purpose:
	- Set human train to new one
Parameters:
	- vector of tiles (new train)
Return Value:
	- None
Algorithm:
	- Sets the human train to new one
Assistance Received:
	- None
********************************************************************* */
void Round::set_saved_hum_train(vector<Tile> humtrain)
{
	human->get_train()->set_train(humtrain);
}

/* *********************************************************************
Function Name: set_saved_comp_train
Purpose:
	- Set comp train to new one
Parameters:
	- vector of tiles (new train)
Return Value:
	- None
Algorithm:
	- Sets the comp train to new one
Assistance Received:
	- None
********************************************************************* */
void Round::set_saved_comp_train(vector<Tile> comptrain)
{
	computer->get_train()->set_train(comptrain);
}

/* *********************************************************************
Function Name: get_load_game
Purpose:
	- Return load game flag
Parameters:
	- None
Return Value:
	- boolean load game
Algorithm:
	- Returns load game flag
Assistance Received:
	- None
********************************************************************* */
bool Round::get_load_game()
{
	return load_game;
}

/* *********************************************************************
Function Name: set_load_game
Purpose:
	- Set load game flag
Parameters:
	- boolean new flag
Return Value:
	- None
Algorithm:
	- Sets the load game to new flag
Assistance Received:
	- None
********************************************************************* */
void Round::set_load_game(bool flag)
{
	load_game = flag;
}

/* *********************************************************************
Function Name: set_saved_hum_hand
Purpose:
	- Set human hand to new one
Parameters:
	- vector of tiles (new hand)
Return Value:
	- None
Algorithm:
	- Sets the human hand to new one
Assistance Received:
	- None
********************************************************************* */
void Round::set_saved_hum_hand(vector<Tile> hum_hand)
{
	human->set_hand(hum_hand);
}

/* *********************************************************************
Function Name: set_saved_comp_train
Purpose:
	- Set comp hand to new one
Parameters:
	- vector of tiles (new train)
Return Value:
	- None
Algorithm:
	- Sets the comp hand to new one
Assistance Received:
	- None
********************************************************************* */
void Round::set_saved_comp_hand(vector<Tile> comp_hand)
{
	computer->set_hand(comp_hand);
}