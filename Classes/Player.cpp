#include "pre_header.h"

#include "Player.h"
#include "CustomTimer.h"

Player::Player()
{
}

Player::~Player()
{
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
