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
    : _current_floor(10)
    , _speed(3.0f)
{

}

player_t::~player_t()
{
}

void player_t::init()
{
    auto floor = get_current_floor();
    _current_pos = floor->get_init_pos(true);
}

const floor_t* player_t::get_current_floor() const
{
    return map_t::GetInstance()->get_floor_info(_current_floor);
}

void player_t::walk_to(const pos_t & pos, turn_call callback)
{
    auto floor = get_current_floor();
    _target_pos = pos;
    _callback = callback;
    _path = floor->get_path(_current_pos, pos);
    reverse(_path.begin(), _path.end());
    move_step();
    uint32_t interval_ms = (uint32_t)(1 / _speed * 1000000);
    _timer_id = timer_queue::GetInstance()->timer_schedule(std::bind(&player_t::move_step, this), interval_ms, false);
}

void player_t::move_step()
{
    if (_path.empty())
        return;

    auto target_pos = _path.back();
    if (_current_pos == target_pos)
    {
        _path.pop_back();
        if (_path.empty())
        {
            // on_end_callback();
            timer_queue::GetInstance()->timer_cancel(_timer_id);
        }
        if (!_path.empty() && _callback)
        {
            auto next_pos = _path.back();
            assert(!_current_pos.slant(next_pos));
            auto distance = abs(_current_pos.x - next_pos.x) + abs(_current_pos.y - next_pos.y);
            _callback(_current_pos, next_pos, distance / _speed);
        }
    }

    if (!_path.empty())
    {
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
}