/*********************************************
Author: Zhang Hao
GitHub: https://github.com/120910383/mota
*********************************************/

#include "player.h"
#include "timer_queue.h"
#include "base/CCScheduler.h"

#include <functional>
using namespace std;
using namespace std::placeholders;

USING_NS_CC;

player_t::player_t()
    : _current_floor(1)
    , _speed(4.0f)
    , _current_state(stand)
    , _current_direction(left)
{

}

player_t::~player_t()
{
}

void player_t::init()
{
    auto floor = get_floor();
    _current_pos = floor->stair_down.pos.offset(floor->stair_down.flip ? -1 : 1, 0);
    _current_direction = floor->stair_down.flip ? left : right;
}

const floor_t* player_t::get_floor() const
{
    return map_t::GetInstance()->get_floor_info(_current_floor);
}

void player_t::walk_to(const pos_t & pos, turn_call turn, function<void()> end)
{
    auto floor = get_floor();
    _turn_call = turn;
    _end_call = end;
    _path = floor->get_path(_current_pos, pos);
    reverse(_path.begin(), _path.end());

    if (_current_state == stand)
    {
        move_step();
        uint32_t interval_ms = (uint32_t)(1 / _speed * 1000000);
        _timer_id = timer_queue::GetInstance()->timer_schedule(std::bind(&player_t::move_step, this), interval_ms, false);
        _current_state = walk;
    }
}

void player_t::move_step()
{
    if (_path.empty())
    {
        move_end();
        return;
    }

    auto floor = get_floor();
    // pick up handle
    if (_current_pos == floor->stair_down.pos)
    {
        move_end();
        stair_down();
        return;
    }
    else if (_current_pos == floor->stair_up.pos)
    {
        move_end();
        stair_up();
        return;
    }

    auto target_pos = _path.back();
    if (_current_pos == target_pos)
    {
        _path.pop_back();
        if (_path.empty())
        {
            move_end();
            return;
        }

        auto next_pos = _path.back();
        assert(!_current_pos.slant(next_pos));
        if (next_pos.x > _current_pos.x)
            _current_direction = right;
        else if (next_pos.x < _current_pos.x)
            _current_direction = left;
        else if (next_pos.y > _current_pos.y)
            _current_direction = up;
        else
            _current_direction = down;

        if (_turn_call)
        {
            _turn_call(next_pos);
        }
    }

    auto next_pos = _path.back();
    if (next_pos.y > _current_pos.y)
        _current_pos.y += 1;
    else if (next_pos.y < _current_pos.y)
        _current_pos.y -= 1;

    if (next_pos.x > _current_pos.x)
        _current_pos.x += 1;
    else if (next_pos.x < _current_pos.x)
        _current_pos.x -= 1;
}

void player_t::move_end()
{
    timer_queue::GetInstance()->timer_cancel(_timer_id);
    _current_state = stand;
    if (_end_call)
    {
        _end_call();
    }
}

void player_t::stair_up()
{
    _current_floor += 1;
    auto floor = get_floor();
    _current_pos = floor->stair_down.pos.offset(floor->stair_down.flip ? -1 : 1, 0);
    _current_direction = floor->stair_down.flip ? left : right;
    if (_stair_call)
    {
        _stair_call();
    }
}

void player_t::stair_down()
{
    _current_floor -= 1;
    auto floor = get_floor();
    _current_pos = floor->stair_up.pos.offset(floor->stair_up.flip ? -1 : 1, 0);
    _current_direction = floor->stair_up.flip ? left : right;
    if (_stair_call)
    {
        _stair_call();
    }
}