/*********************************************
Author: Zhang Hao
GitHub: https://github.com/120910383/mota
*********************************************/

#pragma once

#include "Singleton.h"
#include "maps.h"
#include <set>

class player_delegate;
class player_t : public Singleton<player_t>
{
public:
    player_t();
    ~player_t();

public:
    enum direction { left, right, up, down };
    enum state { stand, walk, fight };

public:
    void init();
    direction get_direction() const { return _current_direction; }
    state get_state() const { return _current_state; }
    pos_t get_pos() const { return _current_pos; }
    float get_speed() const { return _speed; }
    const floor_t* get_floor() const;
    void walk_to(const pos_t& pos);

protected:
    void move_step();
    void move_end();
    void stair_up();
    void stair_down();

protected:
    int32_t _current_floor;
    direction _current_direction;
    state _current_state;
    pos_t _current_pos;
    pos_t _target_pos;
    float _speed; // pos unit / second
    std::vector<pos_t> _path;
    int32_t _timer_id;

    friend class player_delegate;
    std::set<player_delegate*> _delegates;
};

//////////////////////////////////////////////////////////////////////////

class player_delegate
{
public:
    player_delegate();
    virtual ~player_delegate();

public:
    void pause();
    void resume();

public:
    virtual void on_walk_turn(const pos_t& next) {}
    virtual void on_walk_stop() {}
    virtual void on_stair_change() {}
    virtual void on_pick_up_item() {}
    virtual void on_battle() {}
};