#pragma once

#include "Singleton.h"

class Player : public Singleton<Player>
{
public:
    Player();

private:
    int _attack;
    int _defence;
    int _gold;
    int _metal;
};