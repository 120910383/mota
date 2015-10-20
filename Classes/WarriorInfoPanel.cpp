﻿#include "WarriorInfoPanel.h"

USING_NS_CC;

bool WarriorInfoPanel::init()
{
    auto player_bg = Sprite::create("Images/ui_bg.png");
    if (nullptr != player_bg)
    {
        player_bg->setAnchorPoint(Vec2::ZERO);
        player_bg->setPosition(Vec2::ZERO);
        this->addChild(player_bg);
    }

    auto player_eye1 = Sprite::create("Images/tou_eye1.png");
    if (nullptr != player_eye1)
    {
        player_eye1->setPosition(Vec2(86.0f, 100.0f));
        this->addChild(player_eye1);
    }

    auto player_eye2 = Sprite::create("Images/tou_eye2.png");
    if (nullptr != player_eye2)
    {
        player_eye2->setPosition(Vec2(86.0f, 100.0f));
        this->addChild(player_eye2);
    }

    auto player_head = Sprite::create("Images/tou_base.png");
    if (nullptr != player_head)
    {
        player_head->setPosition(Vec2(86.0f, 100.0f));
        this->addChild(player_head);
    }

    auto player_attack_icon = Sprite::create("Images/ui_attack.png");
    if (nullptr != player_attack_icon)
    {
        player_attack_icon->setPosition(Vec2(180.0f, 140.0f));
        this->addChild(player_attack_icon);

        _attack_num = LabelAtlas::create("10", "Images/ps_num_shared.png", 21, 29, '0');
        if (nullptr != _attack_num)
        {
            _attack_num->setScaleX(0.8f);
            _attack_num->setAnchorPoint(Vec2(0.0f, 0.5f));
            _attack_num->setPosition(Vec2(40.0f, player_attack_icon->getContentSize().height / 2));
            player_attack_icon->addChild(_attack_num);
        }
    }

    auto player_defend_icon = Sprite::create("Images/ui_defend.png");
    if (nullptr != player_defend_icon)
    {
        player_defend_icon->setPosition(Vec2(330.0f, 140.0f));
        this->addChild(player_defend_icon);

        _defend_num = LabelAtlas::create("10", "Images/ps_num_shared.png", 21, 29, '0');
        if (nullptr != _defend_num)
        {
            _defend_num->setScaleX(0.8f);
            _defend_num->setAnchorPoint(Vec2(0.0f, 0.5f));
            _defend_num->setPosition(Vec2(40.0f, player_defend_icon->getContentSize().height / 2));
            player_defend_icon->addChild(_defend_num);
        }
    }

    auto player_jb_icon = Sprite::create("Images/ui_jb.png");
    if (nullptr != player_jb_icon)
    {
        player_jb_icon->setPosition(Vec2(180.0f, 90.0f));
        this->addChild(player_jb_icon);

        _jb_num = LabelAtlas::create("0", "Images/ps_num_shared.png", 21, 29, '0');
        if (nullptr != _jb_num)
        {
            _jb_num->setScaleX(0.8f);
            _jb_num->setAnchorPoint(Vec2(0.0f, 0.5f));
            _jb_num->setPosition(Vec2(40.0f, player_jb_icon->getContentSize().height / 2));
            player_jb_icon->addChild(_jb_num);
        }
    }

    auto player_hun_icon = Sprite::create("Images/ui_hun.png");
    if (nullptr != player_hun_icon)
    {
        player_hun_icon->setPosition(Vec2(330.0f, 90.0f));
        this->addChild(player_hun_icon);

        _hun_num = LabelAtlas::create("0", "Images/ps_num_shared.png", 21, 29, '0');
        if (nullptr != _hun_num)
        {
            _hun_num->setScaleX(0.8f);
            _hun_num->setAnchorPoint(Vec2(0.0f, 0.5f));
            _hun_num->setPosition(Vec2(40.0f, player_hun_icon->getContentSize().height / 2));
            player_hun_icon->addChild(_hun_num);
        }
    }

    const auto LENGHT = 280.0f;
    auto progress_bg = Node::create();
    if (nullptr != progress_bg)
    {
        progress_bg->setPosition(Vec2(160.0f, 34.0f));
        this->addChild(progress_bg);

        auto head = Sprite::create("Images/ui_hp_bg_a.png");
        auto tail = Sprite::create("Images/ui_hp_bg_a.png");
        auto body = Sprite::create("Images/ui_hp_bg_b.png");
        head->setAnchorPoint(Vec2::ZERO);
        head->setPosition(Vec2::ZERO);
        progress_bg->addChild(head);
        auto scale = (LENGHT - 2 * head->getContentSize().width) / body->getContentSize().width;
        body->setScaleX(scale);
        body->setAnchorPoint(Vec2::ZERO);
        body->setPosition(Vec2(head->getContentSize().width, 0));
        progress_bg->addChild(body);
        tail->setFlippedX(true);
        tail->setAnchorPoint(Vec2::ZERO);
        tail->setPosition(Vec2(head->getContentSize().width + body->getBoundingBox().size.width, 0));
        progress_bg->addChild(tail);

        _hp_num = LabelAtlas::create("500", "Images/ps_num_shared.png", 21, 29, '0');
        if (nullptr != _hp_num)
        {
            _hp_num->setScaleX(0.8f);
            _hp_num->setAnchorPoint(Vec2(0.5f, 0.5f));
            _hp_num->setPosition(Vec2(LENGHT / 2, body->getContentSize().height / 2));
            progress_bg->addChild(_hp_num);
        }
    }

    for (int32_t i = 0; i < 3; ++i)
    {
        auto key_bg = Sprite::create("Images/ui_btn2.png");
        if (nullptr != key_bg)
        {
            key_bg->setAnchorPoint(Vec2(0, 0.5f));
            key_bg->setPosition(Vec2(456.0f + i * 66.0f, 142.0f));
            this->addChild(key_bg);

            auto key_res = "Images/key" + String::createWithFormat("%d", i)->_string + ".png";
            auto key_sprite = Sprite::create(key_res);
            key_sprite->setScale(0.6f);
            key_sprite->setPosition(Vec2(key_bg->getContentSize().width / 2, key_bg->getContentSize().height / 2));
            key_bg->addChild(key_sprite);
        }
    }

    _key_red_num = LabelAtlas::create("0", "Images/ps_num_key.png", 17, 22, '0');
    if (_key_red_num)
    {
        _key_red_num->setPosition(Vec2(490.0f, 120.0f));
        this->addChild(_key_red_num);
    }
    _key_blue_num = LabelAtlas::create("0", "Images/ps_num_key.png", 17, 22, '0');
    if (_key_blue_num)
    {
        _key_blue_num->setPosition(Vec2(556.0f, 120.0f));
        this->addChild(_key_blue_num);
    }
    _key_yellow_num = LabelAtlas::create("0", "Images/ps_num_key.png", 17, 22, '0');
    if (_key_yellow_num)
    {
        _key_yellow_num->setPosition(Vec2(622.0f, 120.0f));
        this->addChild(_key_yellow_num);
    }

    for (int32_t i = 0; i < 2; ++i)
    {
        auto daoju_bg = Sprite::create("Images/ui_btn.png");
        if (nullptr != daoju_bg)
        {
            daoju_bg->setAnchorPoint(Vec2(0, 0.5f));
            daoju_bg->setPosition(Vec2(466.0f + i * 94.0f, 76.0f));
            this->addChild(daoju_bg);
        }
    }
    return true;
}

void WarriorInfoPanel::on_player_attr_changed()
{
    const auto& player_info = Player::GetInstance()->get_player_info();
    _attack_num->setString(String::createWithFormat("%d", player_info.attack)->_string);
    _defend_num->setString(String::createWithFormat("%d", player_info.defence)->_string);
    _jb_num->setString(String::createWithFormat("%d", player_info.gold)->_string);
    _hun_num->setString(String::createWithFormat("%d", player_info.hun)->_string);
    _hp_num->setString(String::createWithFormat("%d", player_info.hp)->_string);
    _key_red_num->setString(String::createWithFormat("%d", player_info.key_red)->_string);
    _key_blue_num->setString(String::createWithFormat("%d", player_info.key_blue)->_string);
    _key_yellow_num->setString(String::createWithFormat("%d", player_info.key_yellow)->_string);
}

cocos2d::Vec2 WarriorInfoPanel::get_node_position_in_world(node_type type)
{
    Vec2 position;
    switch (type)
    {
    case WarriorInfoPanel::attack:
        position = _attack_num->getParent()->convertToWorldSpace(_attack_num->getPosition());
        break;
    case WarriorInfoPanel::defend:
        position = _defend_num->getParent()->convertToWorldSpace(_defend_num->getPosition());
        break;
    case WarriorInfoPanel::jb:
        position = _jb_num->getParent()->convertToWorldSpace(_jb_num->getPosition());
        break;
    case WarriorInfoPanel::hun:
        position = _hun_num->getParent()->convertToWorldSpace(_hun_num->getPosition());
        break;
    case WarriorInfoPanel::hp:
        position = _hp_num->getParent()->convertToWorldSpace(_hp_num->getPosition());
        break;
    case WarriorInfoPanel::key_red:
        position = _key_red_num->getParent()->convertToWorldSpace(_key_red_num->getPosition());
        break;
    case WarriorInfoPanel::key_blue:
        position = _key_blue_num->getParent()->convertToWorldSpace(_key_blue_num->getPosition());
        break;
    case WarriorInfoPanel::key_yellow:
        position = _key_yellow_num->getParent()->convertToWorldSpace(_key_yellow_num->getPosition());
        break;
    default:
        break;
    }
    return position;
}
