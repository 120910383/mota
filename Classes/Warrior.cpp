#include "pre_header.h"

#include "Warrior.h"

USING_NS_CC;
static const float MOVE_SPEED = 200.0f;

WarriorNode * WarriorNode::create(const std::string & name)
{
    cocostudio::ArmatureDataManager::getInstance()->addArmatureFileInfo("Spine/" + name + ".ExportJson");
    WarriorNode* warrior_node = new (std::nothrow) WarriorNode();
    if (warrior_node && warrior_node->init(name))
    {
        warrior_node->autorelease();
        return warrior_node;
    }
    CC_SAFE_DELETE(warrior_node);
    return nullptr;
}

bool WarriorNode::init(const std::string& name)
{
    if (cocostudio::Armature::init(name))
    {
        _current_direction = right;
        _current_state = stand;
        update_ui();
        return true;
    }
    else
    {
        return false;
    }
}

void WarriorNode::move_to(const cocos2d::Vec2& position, const std::function<void()>& callback)
{
    runAction(Sequence::create(MoveTo::create(getPosition().distance(position) / MOVE_SPEED, position),
        CallFunc::create(callback), nullptr));

    direction dir = calucate_direction(position);
    if (_current_direction == dir && _current_state == run)
        return;

    _current_direction = dir;
    _current_state = run;
    update_ui();
}

void WarriorNode::turn_to(const cocos2d::Vec2& position)
{
    direction dir = calucate_direction(position);
    if (_current_direction == dir && _current_state == stand)
        return;

    _current_direction = dir;
    _current_state = stand;
    update_ui();
}

void WarriorNode::stand_auto()
{
    if (_current_state == stand)
        return;

    _current_state = stand;
    update_ui();
}

void WarriorNode::fight_auto(int times, const std::function<void()>& callback)
{
    if (_current_state == fight)
        return;

    _current_state = fight;
    update_ui();

    // 此处要使用不会被释放掉的变量来记录次数，局部变量会导致bug出现
    static int fight_times = 0;  // 用于计算执行多少次fight动画后调用回掉函数
    fight_times = times;
    auto function = [&, callback](cocostudio::Armature *armature, cocostudio::MovementEventType movementType, const std::string& movementID) {
        if ((movementID == "Sfight" || movementID == "Bfight" || movementID == "Ffight")
            && movementType == cocostudio::LOOP_COMPLETE && --fight_times == 0)
        {
            int test = fight_times;
            ++test;
            if (callback)
                callback();
        }
    };
    getAnimation()->setMovementEventCallFunc(function);
}

bool WarriorNode::is_fighting()
{
    std::string movement_id = getAnimation()->getCurrentMovementID();
    return movement_id == "Sfight" || movement_id == "Ffight" || movement_id == "Bfight";
}

WarriorNode::direction WarriorNode::calucate_direction(const cocos2d::Vec2& position)
{
    direction direction_return = right;
    Vec2 current_position = getPosition();
    if (current_position.x != position.x)
        direction_return = position.x > current_position.x ? right : left;
    else if (current_position.y != position.y)
        direction_return = position.y > current_position.y ? up : down;
    else
        assert(false);

    return direction_return;
}

void WarriorNode::update_ui()
{
    const std::string direction_str = _current_direction == up ? "B" :
        (_current_direction == down ? "F" : "S");

    const std::string state_str = _current_state == stand ? "stand" :
        (_current_state == run ? "run" : "fight");

    getAnimation()->play(direction_str + state_str);

    if (_current_direction == left || _current_direction == right)
        setScaleX(_current_direction == right ? -1.0f : 1.0f);
    else
        setScaleX(1.0f);
}