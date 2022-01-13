#ifndef __PLAYER__
#define __PLAYER__
#include <iostream>
#include <vector>
#include <string>
#include <map>
#include "Deck.h"
#include "Train.h"

using namespace std;

class Player
{
public:
    Player();
    // Player Options.
    virtual void play();
    pair<Tile, Train *> move_calc();
    virtual void move();
    bool isvalid(Tile, Player *, Train *);
    bool if_draw(Player *);
    Player *get_opp_player(Player *);
    void play_valid_tile(Tile, Player *);
    void return_valid_tiles(Player *);
    int find_tile_index(int, map<int, Tile>, Player *);
    void quit();
    Tile get_largest(map<int, Tile>);
    pair<Tile, Train *> compare_largest(Tile, Tile, Tile);
    int find_tile_in_hand(Tile);
    void set_hand(vector<Tile>);

    // Player Logic
    void draw_tile(Tile);
    void add_train(Tile);
    bool operator==(Player &);
    void print_valid_tiles(Player *, int, int);
    void print_options(Player *);
    void print_hands();
    void print_trains();
    void print_output();

    //Input Functions
    int which_map();

    // Getters/Setters.
    void set_human(Player *);
    void set_computer(Player *);
    void set_boneyard(Deck *);
    void set_mexican_train(Train *);
    void set_curr_player(Player *);
    bool get_round_ended();
    void set_round_ended(bool);
    void set_engine(Tile);
    bool get_savegame();
    void set_savegame(bool);
    void reset_maps();
    bool get_skipped();
    void set_skipped(bool);
    bool get_if_double();
    void set_if_double(bool);
    int get_count_double();
    void set_count_double(int);
    Player *get_prev_player();
    void set_prev_player(Player *);
    void update_train(Train *, int);
    void add_score(Player *);
    bool get_identifier();
    void set_identifier(bool);
    vector<Tile> get_hand();
    void remove_from_hand(int);
    int get_score();
    void set_score(int);
    void assign_score(int);
    Train *get_train();
    void set_train(Train *);
    int get_turns_marker();
    void set_turns_marker();
    void reset_turns_marker();

    // NonDouble Valid Tile Maps.
    map<int, Tile> get_human_nondouble_map();
    void set_human_nondouble_map(int, Tile);
    map<int, Tile> get_comp_nondouble_map();
    void set_comp_nondouble_map(int, Tile);
    map<int, Tile> get_mex_nondouble_map();
    void set_mex_nondouble_map(int, Tile);

    // Double Valid Tile Maps.
    map<int, Tile> get_human_double_map();
    void set_human_double_map(int, Tile);
    map<int, Tile> get_comp_double_map();
    void set_comp_double_map(int, Tile);
    map<int, Tile> get_mex_double_map();
    void set_mex_double_map(int, Tile);

protected:
    int score;
    vector<Tile> hand;
    bool is_comp;
    int turnsMarker;
    bool skipped;
    bool round_ended;
    bool if_double;
    int count_double;
    Player *previous_player;
    Player *human;
    Player *computer;
    Train *mexican_train;
    Player *curr_player;
    Deck *boneyard;
    Tile engine;
    Train *train;
    bool second_invalid_double;
    bool savegame;

    map<int, Tile> human_valid_nondoubles;
    map<int, Tile> computer_valid_nondoubles;
    map<int, Tile> mexican_valid_nondoubles;
    map<int, Tile> human_valid_doubles;
    map<int, Tile> computer_valid_doubles;
    map<int, Tile> mexican_valid_doubles;
};

#endif
