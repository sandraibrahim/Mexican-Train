#include "GameState.h"
using namespace std;

/* *********************************************************************
	Name:  Sandra Ibrahim                                
   Project:  Project #1 C++ Mexican Train
   Class:  CMPS 366 01: OPL                     
   Date:  10/20/2021
********************************************************************* */


/* *********************************************************************
Function Name: main
Purpose: 
	- Create GameState object asks user what they would like to do: New Game or load saved game and runs it
Parameters: 
	- None
Return Value:
	- None
Algorithm:
	1) Create GameState object.
	2) Ask user if theyd like to play a new game or load a game. If new, starts.
	3) If saved, asks for file name and opens if available.
Assistance Received: 
	- None
********************************************************************* */
int main()
{
	GameState game;
	int input;
	string file;
	do
	{
		cout << endl;
		cout << "Welcome to Sandra's Mexican Train!" << endl;
		cout << "Would you like to play a new game or load a previous game? Choose: 1. New game or 2. Load game: ";
		cin >> input;
	} while (input < 1 || input > 2);
	if (input == 1)
	{
		cout << endl;
		cout << "Good luck and Enjoy! I heard the computer is pretty smart..." << endl;
		cout << "Press any key to continue..." << endl;
		system("read");
		system("clear");

		game.run();
	}
	else
	{
		do
		{
			cout << "What is the name of the file you want to open?" << endl;
			cin >> file;
			if (game.is_empty(file))
			{
				do
				{
					cout << "No saved games under that name. Would you like to try a new file name, start a new game, or exit? 1. New File Name 2. Start a new Game 3. Exit: ";
					cin >> input;
				} while (input < 1 || input > 3);
				if (input == 2)
				{
					cout << "Good luck! I heard the computer is pretty smart..." << endl;
					cout << "Press any key to continue..." << endl;
					system("read");
					system("clear");
					game.run();
				}
				else if (input == 2)
				{
					cout << "Sorry to see you go! Play again soon (:" << endl;
				}
			}
			else
			{

				// read and start.
				cout << "There is a saved game! Enjoy and good luck, I heard the computer is pretty smart...";
				cout << "Press any key to continue..." << endl;
				system("read");
				system("clear");
				game.start_saved_game(file);
				game.run();
				// return;
			}
		} while (input == 1);
	}
}
