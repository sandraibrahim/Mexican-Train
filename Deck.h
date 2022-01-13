#ifndef __DECK__
#define __DECK__
#include <string>
#include <iostream>
#include "Tile.h"
#include <vector>
#include <algorithm>
#include <random>
using namespace std;

class Deck
{
public:
    // Default Constructor
    Deck();
    // Logic.
    Tile pop_engine(int);
    void shuffle_boneyard();
    Tile draw_tile_from_boneyard();
    void print_boneyard();
    vector<Tile> get_boneyard();
    void set_boneyard(vector<Tile>);

private:
    vector<Tile> boneyard;
};
#endif