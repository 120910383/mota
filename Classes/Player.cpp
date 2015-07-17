#include "Player.h"
#include <functional>
#include <algorithm>
using namespace std;

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

void PlayerDelegate::add_attack(int32_t addition)
{
    Player::GetInstance()->_player.attack += addition;
    std::for_each(Player::GetInstance()->_delegates.begin(), Player::GetInstance()->_delegates.end(), std::bind(&PlayerDelegate::on_player_attr_changed, std::placeholders::_1));
}

void PlayerDelegate::add_defence(int32_t addition)
{
    Player::GetInstance()->_player.defence += addition;
    std::for_each(Player::GetInstance()->_delegates.begin(), Player::GetInstance()->_delegates.end(), std::bind(&PlayerDelegate::on_player_attr_changed, std::placeholders::_1));
}

void PlayerDelegate::add_gold_hun(int32_t gold_addition, int32_t hun_addition)
{
    Player::GetInstance()->_player.gold += gold_addition;
    Player::GetInstance()->_player.hun += hun_addition;
    std::for_each(Player::GetInstance()->_delegates.begin(), Player::GetInstance()->_delegates.end(), std::bind(&PlayerDelegate::on_player_attr_changed, std::placeholders::_1));
}

void PlayerDelegate::add_key(int32_t key_red_addition, int32_t key_blue_addition, int32_t key_yellow_addition)
{
    Player::GetInstance()->_player.key_red += key_red_addition;
    Player::GetInstance()->_player.key_blue += key_blue_addition;
    Player::GetInstance()->_player.key_yellow += key_yellow_addition;
    std::for_each(Player::GetInstance()->_delegates.begin(), Player::GetInstance()->_delegates.end(), std::bind(&PlayerDelegate::on_player_attr_changed, std::placeholders::_1));
}