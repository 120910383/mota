﻿#pragma once

#include "Singleton.h"
#include <set>
typedef int int32_t;

class PlayerDelegate;
class Player : public Singleton<Player>
{
public:
    Player();
    ~Player();

public:
    struct player_t
    {
        int32_t attack;
        int32_t defence;
        int32_t gold;
        int32_t hun;
        int32_t hp;
        int32_t key_blue;
        int32_t key_red;
        int32_t key_yellow;

        player_t() : attack(10), defence(10), gold(0), hun(0), hp(500), key_blue(0), key_red(0), key_yellow(0) {}
    };
    
public:
    inline const player_t& get_player_info() { return _player; }

private:
    player_t _player;
    std::set<PlayerDelegate*> _delegates;

private:
    friend class PlayerDelegate;
};

class PlayerDelegate
{
public:
    PlayerDelegate();
    ~PlayerDelegate();

public:
    static void add_attack(int32_t addition);
    static void add_defence(int32_t addition);
    static void add_gold_hun(int32_t gold_addition, int32_t hun_addition);
    static void add_hp(int32_t addition);
    static void add_key(int32_t key_red_addition, int32_t key_blue_addition, int32_t key_yellow_addition);

public:
    virtual void on_player_attr_changed() {}
};