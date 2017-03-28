/*********************************************
Author: Zhang Hao
GitHub: https://github.com/120910383/mota
*********************************************/

#include "timer_queue.h"

#include <algorithm>

timer_queue::timer_queue()
{
}

timer_queue::~timer_queue()
{
    for (auto elem : _timers)
    {
        delete elem;
    }
}

void timer_queue::timer_handle(uint32_t ms)
{
    _elapsed += ms;
    if (_timers.empty())
        return;

    auto timer = _timers.front();
    if (timer->time_to_call <= _elapsed)
    {
        if (timer->interval_ms == uint32_t(-1))
        {
            timer_cancel((uint32_t)timer);
        }
        else
        {
            timer->time_to_call += timer->interval_ms;
            stable_sort(_timers.begin(), _timers.end());
        }

        auto callback = timer->callback;
        if (callback)
        {
            callback();
        }
    }
}

int32_t timer_queue::timer_schedule(std::function<void()> callback, uint32_t interval_ms, bool once)
{
    auto timer = new timer_t;
    timer->callback = callback;
    timer->interval_ms = once ? uint32_t(-1) : interval_ms;
    timer->time_to_call = _elapsed + interval_ms;
    _timers.push_back(timer);
    stable_sort(_timers.begin(), _timers.end());
    return (int32_t)timer;
}

void timer_queue::timer_cancel(int32_t timer_id)
{
    auto timer = (timer_t*)timer_id;
    auto iter = find(_timers.begin(), _timers.end(), timer);
    if (iter != _timers.end())
    {
        delete *iter;
        _timers.erase(iter);
    }
}
