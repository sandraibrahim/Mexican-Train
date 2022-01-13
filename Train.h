#ifndef __TRAIN__
#define __TRAIN__
#include <string>
#include <iostream>
#include <vector>
#include "Tile.h"

using namespace std;

class Train
{
public:
   Train();
   // Train Logic.
   void add_to_train(Tile, bool);
   void change_marker(bool);
   bool operator==(Train &t2);
   bool equal_to(vector<Tile>, vector<Tile>);

   // Output Functions.
   void print_train();

   // Getters/Setters.
   int get_num();
   void set_num(int);
   vector<Tile> get_curr_train();
   bool get_marker();
   int get_side();
   void set_side(bool);
   bool get_is_double();
   void set_is_double(bool);
   void set_train(vector<Tile>);

private:
   vector<Tile> curr_train;
   bool is_marked;
   bool tile_side;
   int wanted_num;
   bool is_double;
};

#endif