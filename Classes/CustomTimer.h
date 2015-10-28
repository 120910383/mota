#pragma once

#include "Singleton.h"

class CustomTimer : public Singleton<CustomTimer>
{
public:
    void timer(const std::string& key, const std::function<void()>& callback, int32_t delay = 0, bool once = true);
    void cancel(const std::string& key);
};