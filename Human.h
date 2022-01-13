#ifndef __HUMAN__
#define __HUMAN__
#include "Player.h"
using namespace std;

class Human : public Player
{
    // Player Logic.
public:
    Human();
    void play();
    void menu();
    void help();
    void move();
    void save();
};
#endif