/*********************************************
Author: Zhang Hao
GitHub: https://github.com/120910383/mota
*********************************************/

#pragma once

#include "Singleton.h"
#include "maps.h"

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
    typedef std::function<void(const pos_t& end)> turn_call;
    void walk_to(const pos_t& pos, turn_call turn, std::function<void()> end);
    void set_stair_change_call(std::function<void()> change) { _stair_call = change; }

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
    turn_call _turn_call;
    std::function<void()> _end_call;
    std::function<void()> _stair_call;
    std::vector<pos_t> _path;
    int32_t _timer_id;
};
