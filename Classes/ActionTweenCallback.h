#pragma once

class ActionTweenCallback : public cocos2d::ActionInterval
{
public:
    typedef std::function<void(float)> update_func;
    static ActionTweenCallback* create(float duration, float from, float to, const update_func& update);

    bool initWithDuration(float duration, float from, float to, const update_func& update);
    void startWithTarget(cocos2d::Node *target) override;
    void update(float dt) override;
    ActionTweenCallback* reverse() const override;
    ActionTweenCallback *clone() const override;

protected:
    float _from, _to;
    float _delta;
    update_func _update;
};