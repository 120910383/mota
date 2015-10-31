#include "pre_header.h"

#include "ActionTweenCallback.h"

ActionTweenCallback * ActionTweenCallback::create(float duration, float from, float to, const update_func& update)
{
    ActionTweenCallback* ret = new (std::nothrow) ActionTweenCallback();
    if (ret && ret->initWithDuration(duration, from, to, update))
    {
        ret->autorelease();
    }
    else
    {
        CC_SAFE_DELETE(ret);
    }

    return ret;
}

bool ActionTweenCallback::initWithDuration(float duration, float from, float to, const update_func& update)
{
    if (ActionInterval::initWithDuration(duration))
    {
        _to = to;
        _from = from;
        _update = update;
        return true;
    }

    return false;
}

void ActionTweenCallback::startWithTarget(cocos2d::Node * target)
{
    ActionInterval::startWithTarget(target);
    _delta = _to - _from;
}

void ActionTweenCallback::update(float dt)
{
    if (_update)
        _update(_to - _delta * (1 - dt));
}

ActionTweenCallback * ActionTweenCallback::reverse() const
{
    return ActionTweenCallback::create(_duration, _to, _from, _update);
}

ActionTweenCallback * ActionTweenCallback::clone() const
{
    // no copy constructor    
    auto a = new (std::nothrow) ActionTweenCallback();
    a->initWithDuration(_duration, _from, _to, _update);
    a->autorelease();
    return a;
}
