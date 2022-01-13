#ifndef __COMPUTER__
#define __COMPUTER__
#include "Player.h"
using namespace std;

class Computer : public Player
{
public:
	// Default Constructor
	Computer();

	// Player Logic.
	void play();
	void move();
	void play_tile(Tile);

private:
	pair<Tile, Train *> curr_move;
};
#endif