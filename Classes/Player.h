#pragma once

#include "Singleton.h"

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
    inline void set_current_floor(int32_t floor) { _last_floor = floor; }
    inline int32_t get_current_floor() { return _last_floor; }
    void save();

protected:
    void load();

private:
    player_t _player;
    int32_t _last_floor;
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
    static void add_attack(int32_t addition, int32_t delay = 0);
    static void add_defence(int32_t addition, int32_t delay = 0);
    static void add_gold_hun(int32_t gold_addition, int32_t hun_addition, int32_t delay = 0);
    static void add_hp(int32_t addition, int32_t delay = 0);
    static void add_key(int32_t key_red_addition, int32_t key_blue_addition, int32_t key_yellow_addition, int32_t delay = 0);

public:
    virtual void on_player_attr_changed() {}

protected:
    static void notify();
};