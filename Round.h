#ifndef __ROUND__
#define __ROUND__
#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <stdlib.h>
#include "Player.h"
#include "Human.h"
#include "Computer.h"

using namespace std;

class Round
{
public:
    // Round
    Round();
    void initialize_game();
    void run_round();
    bool round_over();

    // Output Functions.
    void print_round_info();

    // Getter/Setter.
    void reset_boneyard();
    void reset_players();
    void reset_game();
    Player *get_curr_player();
    void set_curr_player(Player *);
    bool get_is_round_over();
    void set_is_round_over(bool);
    int get_engine_num();
    void set_curr_engine_num();
    Tile get_engine();
    void set_engine();
    void set_round_num();
    Player *get_computer();
    Player *get_human();
    int get_round_num();
    void set_saved_boneyard(vector<Tile>);
    void set_saved_curr_player(Player *);
    void set_saved_mex_train(vector<Tile>);
    void set_saved_round_num(int);
    void set_saved_curr_engine_num(int);
    void set_saved_engine(Tile);
    void set_saved_hum_train(vector<Tile>);
    void set_saved_comp_train(vector<Tile>);
    bool get_load_game();
    void set_load_game(bool);
    void set_saved_hum_hand(vector<Tile>);
    void set_saved_comp_hand(vector<Tile>);

    void deal_hands(Player *player);
    Deck *get_boneyard();
    Train *get_mex_train();

private:
    Deck *boneyard;
    Player *computer;
    Player *human;
    Player *curr_player;
    Train *mexican_train;

    int round_num;
    int curr_engine_num;
    Tile engine;
    bool is_round_over;
    bool load_game;
};

#endif
