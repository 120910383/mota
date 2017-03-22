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
    void init();
    const floor_t* get_current_floor() const;
    typedef std::function<void(const pos_t& start, const pos_t& end, float time)> turn_call;
    void walk_to(const pos_t& pos, turn_call callback);

protected:
    void move_step();

protected:
    const int32_t _current_floor;
    pos_t _current_pos;
    pos_t _target_pos;
    double _speed; // pos unit / second
    turn_call _callback;
    std::vector<pos_t> _path;
    int32_t _timer_id;
};
