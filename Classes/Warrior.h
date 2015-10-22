#pragma once

#include "cocostudio/CocoStudio.h"

class WarriorNode : public cocostudio::Armature
{
public:
    static WarriorNode* create(const std::string& name = "hero");

public:
    virtual bool init(const std::string& name);
    void move_to(const cocos2d::Vec2& position, const std::function<void()>& callback);
    void turn_to(const cocos2d::Vec2& position);
    void stand_auto();
    void fight_auto(int times, const std::function<void()>& callback);
    bool is_fighting();
    void set_lock(bool lock) { _lock = lock; }
    bool is_lock() { return _lock; }

protected:
    enum direction { left, right, up, down };
    enum state { stand, run, fight };
    direction calucate_direction(const cocos2d::Vec2& position);
    void update_ui();
protected:
    direction _current_direction;
    state _current_state;
    bool _lock; // 自定义标记，外部设置和判断用，当前是否正忙
};