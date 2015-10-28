#include "pre_header.h"

#include "CustomTimer.h"

USING_NS_CC;

void CustomTimer::timer(const std::string& key, const std::function<void()>& callback, int32_t delay, bool once)
{
    Director::getInstance()->getScheduler()->schedule([callback, once, key](float f) {
        if (once)
            Director::getInstance()->getScheduler()->unschedule(key, CustomTimer::GetInstance());

        if (callback)
            callback();
    }, CustomTimer::GetInstance(), delay / 1000.0f, false, key);
}

void CustomTimer::cancel(const std::string & key)
{
    Director::getInstance()->getScheduler()->unschedule(key, CustomTimer::GetInstance());
}
