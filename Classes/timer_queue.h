#pragma once

#include "Singleton.h"

#include <functional>
#include <vector>

class timer_queue : public Singleton<timer_queue>
{
public:
    timer_queue();
    ~timer_queue();
public:
    struct timer_t
    {
        std::function<void()> callback;
        uint32_t interval_ms;
        uint64_t time_to_call;
        inline bool operator < (const timer_t* elem)
        {
            return time_to_call < elem->time_to_call;
        }
    };

public:
    void timer_handle(uint32_t ms);
    int32_t timer_schedule(std::function<void()> callback, uint32_t interval_ms, bool once);
    void timer_cancel(int32_t timer_id);

protected:
    std::vector<timer_t*> _timers;
    uint64_t _elapsed;
};
