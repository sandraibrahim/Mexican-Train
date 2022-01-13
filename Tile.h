#ifndef __TILE__
#define __TILE__
#include <string>
#include <iostream>
using namespace std;

class Tile
{
public:
   Tile();
   Tile(int, int);
   void switch_tile();

   // Getters/Setters.
   int getright() const;
   int getleft() const;
   bool operator==(Tile t2);
   bool operator>(Tile t2);
   friend ostream &operator<<(ostream &, Tile &);

private:
   int left;  // left side of domino
   int right; // right side of domino
};

#endif