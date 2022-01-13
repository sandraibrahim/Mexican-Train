#ifndef __GAMESTATE__
#define __GAMESTATE__
#include "Train.h"
#include "Deck.h"
#include <fstream>
#include <sstream>
#include "Round.h"
#include <iostream>
#include <string>
#include <cstring>
#include <stdlib.h>

using namespace std;

class GameState
{
public:
    GameState();

    // Logic
    void run();
    int coin_toss();
    void goes_first();

    // Getters/Setters
    void set_game_over();
    int get_human_total();
    void add_human_total();
    int get_comp_total();
    void add_comp_total();
    void save_to_file();
    string convert_to_string(vector<Tile>);
    bool is_empty(string);
    void start_saved_game(string);
    vector<Tile> parse_tile_vector(string, vector<Tile>);

private:
    Round *curr_round;
    bool game_over;
    int human_total_score;
    int computer_total_score;
};

#endif
