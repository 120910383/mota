#include "pre_header.h"

#include "Player.h"
#include "CustomTimer.h"

Player::Player()
    : _last_floor(1)
{
    load();
}

Player::~Player()
{
    save();
}

void Player::save()
{
    cocos2d::ValueMap player;
    player["attack"] = _player.attack;
    player["defence"] = _player.defence;
    player["gold"] = _player.gold;
    player["hun"] = _player.hun;
    player["hp"] = _player.hp;
    player["key_blue"] = _player.key_blue;
    player["key_red"] = _player.key_red;
    player["key_yellow"] = _player.key_yellow;
    player["last_floor"] = _last_floor;

    auto full_path = cocos2d::FileUtils::getInstance()->getWritablePath() + "player.xml";
    if (!cocos2d::FileUtils::getInstance()->writeToFile(player, full_path))
    {
        assert(false);
    }
}

void Player::load()
{
    auto full_path = cocos2d::FileUtils::getInstance()->getWritablePath() + "player.xml";
    auto value_map = cocos2d::FileUtils::getInstance()->getValueMapFromFile(full_path);
    if (!value_map.empty())
    {
        _player.attack = value_map["attack"].asInt();
        _player.defence = value_map["defence"].asInt();
        _player.gold = value_map["gold"].asInt();
        _player.hun = value_map["hun"].asInt();
        _player.hp = value_map["hp"].asInt();
        _player.key_blue = value_map["key_blue"].asInt();
        _player.key_red = value_map["key_red"].asInt();
        _player.key_yellow = value_map["key_yellow"].asInt();
        auto value = value_map["last_floor"];
        if (!value.isNull())
        {
            _last_floor = value.asInt();
        }
    }
}

PlayerDelegate::PlayerDelegate()
{
    Player::GetInstance()->_delegates.insert(this);
}

PlayerDelegate::~PlayerDelegate()
{
    Player::GetInstance()->_delegates.erase(this);
}

void PlayerDelegate::add_attack(int32_t addition, int32_t delay)
{
    Player::GetInstance()->_player.attack += addition;
    CustomTimer::GetInstance()->timer("attack_delay", std::bind(&PlayerDelegate::notify), delay, true);
}

void PlayerDelegate::add_defence(int32_t addition, int32_t delay)
{
    Player::GetInstance()->_player.defence += addition;
    CustomTimer::GetInstance()->timer("defence_delay", std::bind(&PlayerDelegate::notify), delay, true);
}

void PlayerDelegate::add_gold_hun(int32_t gold_addition, int32_t hun_addition, int32_t delay)
{
    Player::GetInstance()->_player.gold += gold_addition;
    Player::GetInstance()->_player.hun += hun_addition;
    CustomTimer::GetInstance()->timer("hun_delay", std::bind(&PlayerDelegate::notify), delay, true);
}

void PlayerDelegate::add_hp(int32_t addition, int32_t delay)
{
    Player::GetInstance()->_player.hp += addition;
    CustomTimer::GetInstance()->timer("hp_delay", std::bind(&PlayerDelegate::notify), delay, true);
}

void PlayerDelegate::add_key(int32_t key_red_addition, int32_t key_blue_addition, int32_t key_yellow_addition, int32_t delay)
{
    Player::GetInstance()->_player.key_red += key_red_addition;
    Player::GetInstance()->_player.key_blue += key_blue_addition;
    Player::GetInstance()->_player.key_yellow += key_yellow_addition;
    CustomTimer::GetInstance()->timer("key_delay", std::bind(&PlayerDelegate::notify), delay, true);
}

void PlayerDelegate::notify()
{
    std::for_each(Player::GetInstance()->_delegates.begin(), Player::GetInstance()->_delegates.end(), std::bind(&PlayerDelegate::on_player_attr_changed, std::placeholders::_1));
}
