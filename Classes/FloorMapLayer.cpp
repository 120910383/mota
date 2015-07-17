﻿#include "FloorMapLayer.h"
#include "spine/spine-cocos2dx.h"
#include "AStar.h"
#include "PopupLayer.h"
#include "Player.h"

USING_NS_CC;
static const float MOVE_SPEED = 200.0f;


Scene* FloorMapLayer::scene()
{
    auto ret_scene = Scene::create();
    FloorMapLayer* floor_layer = FloorMapLayer::create();
    ret_scene->addChild(floor_layer);
    return ret_scene;
}

bool FloorMapLayer::init()
{
    if (!Layer::init())
    {
        return false;
    }
    setTouchMode(Touch::DispatchMode::ONE_BY_ONE);
    setTouchEnabled(true);
    _tiled_map = cocos2d::experimental::TMXTiledMap::create("TileMaps/floor001.tmx");
    if (nullptr != _tiled_map)
    {
        _tiled_map->setPosition(Vec2(-50.0f, -50.0f));
        this->addChild(_tiled_map);
    }

    _road_node = Node::create();
    if (nullptr != _road_node)
    {
        _road_node->setPosition(Vec2::ZERO);
        this->addChild(_road_node);
    }

    _warrior = spine::SkeletonAnimation::createWithFile("Spine/raptor.json", "Spine/raptor.atlas", 1.0f);
    if (nullptr != _warrior)
    {
        _warrior->setScale(0.1f);
        //_warrior->setAnchorPoint(Vec2(0.5f, 0.5f));
        _warrior->setPosition(Vec2(62.5f, 62.5f));
        this->addChild(_warrior);
    }



    auto inside_bg = Sprite::create("Images/bg_top2.png");
    auto outside_bg = Sprite::create("Images/bg_top_1.png");
    if (nullptr != outside_bg && nullptr != inside_bg)
    {
        auto win_size = Director::getInstance()->getWinSize();
        outside_bg->setScale(inside_bg->getContentSize().width / outside_bg->getContentSize().width);
        outside_bg->setPosition(Vec2(win_size.width / 2, 1092.0f));
        this->addChild(outside_bg);

        inside_bg->setPosition(Vec2(win_size.width / 2, 1076.0f));
        this->addChild(inside_bg);
    }

    _arrow_node = Node::create();
    if (nullptr != _arrow_node)
    {
        _arrow_node->setPosition(Vec2::ZERO);
        _arrow_node->setVisible(false);
        this->addChild(_arrow_node);

        auto a5 = Sprite::create("Images/a5.png");
        if (nullptr != a5)
        {
            a5->setPosition(Vec2::ZERO);
            _arrow_node->addChild(a5);
        }
        auto a11 = Sprite::create("Images/a11.png");
        if (nullptr != a11)
        {
            a11->setPosition(Vec2::ZERO);
            a11->setScale(0.6f);
            auto fade = Sequence::createWithTwoActions(FadeOut::create(1.0f), FadeIn::create(0.0f));
            auto scale = Sequence::createWithTwoActions(ScaleTo::create(1.0f, 2.0f), ScaleTo::create(0.0f, 0.6f));
            a11->runAction(RepeatForever::create(Spawn::createWithTwoActions(fade, scale)));
            _arrow_node->addChild(a11);
        }
        auto arrow = Sprite::create("Images/goal_jian.png");
        if (nullptr != arrow)
        {
            arrow->setScale(0.5f);
            arrow->setAnchorPoint(Vec2(0.5f, 0.0f));
            arrow->setPosition(Vec2::ZERO);
            arrow->runAction(RepeatForever::create(Sequence::createWithTwoActions(MoveBy::create(0.5f, Vec2(0, 10.0f)), MoveBy::create(0.5f, Vec2(0, -10.0f)))));
            _arrow_node->addChild(arrow);
        }
    }

    auto player_node = Node::create();
    if (nullptr != player_node)
    {
        player_node->setPosition(Vec2(-10.0f, 825.0f));
        this->addChild(player_node);

        auto player_bg = Sprite::create("Images/ui_bg.png");
        if (nullptr != player_bg)
        {
            player_bg->setAnchorPoint(Vec2::ZERO);
            player_bg->setPosition(Vec2::ZERO);
            player_node->addChild(player_bg);
        }

        auto player_eye1 = Sprite::create("Images/tou_eye1.png");
        if (nullptr != player_eye1)
        {
            player_eye1->setPosition(Vec2(86.0f, 100.0f));
            player_node->addChild(player_eye1);
        }

        auto player_eye2 = Sprite::create("Images/tou_eye2.png");
        if (nullptr != player_eye2)
        {
            player_eye2->setPosition(Vec2(86.0f, 100.0f));
            player_node->addChild(player_eye2);
        }

        auto player_head = Sprite::create("Images/tou_base.png");
        if (nullptr != player_head)
        {
            player_head->setPosition(Vec2(86.0f, 100.0f));
            player_node->addChild(player_head);
        }

        auto player_attack_icon = Sprite::create("Images/ui_attack.png");
        if (nullptr != player_attack_icon)
        {
            player_attack_icon->setPosition(Vec2(180.0f, 140.0f));
            player_node->addChild(player_attack_icon);

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
            player_node->addChild(player_defend_icon);

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
            player_node->addChild(player_jb_icon);

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
            player_node->addChild(player_hun_icon);

            _hun_num = LabelAtlas::create("0", "Images/ps_num_shared.png", 21, 29, '0');
            if (nullptr != _hun_num)
            {
                _hun_num->setScaleX(0.8f);
                _hun_num->setAnchorPoint(Vec2(0.0f, 0.5f));
                _hun_num->setPosition(Vec2(40.0f, player_hun_icon->getContentSize().height / 2));
                player_hun_icon->addChild(_hun_num);
            }
        }

        const auto LENGHT = 266.0f;
        auto progress_bg = Node::create();
        if (nullptr != progress_bg)
        {
            progress_bg->setPosition(Vec2(160.0f, 34.0f));
            player_node->addChild(progress_bg);

            auto head = Sprite::create("Images/ui_hp_bg_a.png");
            auto tail = Sprite::create("Images/ui_hp_bg_a.png");
            auto body = Sprite::create("Images/ui_hp_bg_b.png");
            head->setAnchorPoint(Vec2::ZERO);
            head->setPosition(Vec2::ZERO);
            progress_bg->addChild(head);
            body->setScaleX(LENGHT / body->getContentSize().width);
            body->setAnchorPoint(Vec2::ZERO);
            body->setPosition(Vec2(head->getContentSize().width, 0));
            progress_bg->addChild(body);
            tail->setFlippedX(true);
            tail->setAnchorPoint(Vec2::ZERO);
            tail->setPosition(Vec2(head->getContentSize().width + body->getBoundingBox().size.width, 0));
            progress_bg->addChild(tail);
        }

        for (int32_t i = 0; i < 3; ++i)
        {
            auto key_bg = Sprite::create("Images/ui_btn2.png");
            if (nullptr != key_bg)
            {
                key_bg->setAnchorPoint(Vec2(0, 0.5f));
                key_bg->setPosition(Vec2(456.0f + i * 66.0f, 142.0f));
                player_node->addChild(key_bg);

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
            player_node->addChild(_key_red_num);
        }
        _key_blue_num = LabelAtlas::create("0", "Images/ps_num_key.png", 17, 22, '0');
        if (_key_blue_num)
        {
            _key_blue_num->setPosition(Vec2(556.0f, 120.0f));
            player_node->addChild(_key_blue_num);
        }
        _key_yellow_num = LabelAtlas::create("0", "Images/ps_num_key.png", 17, 22, '0');
        if (_key_yellow_num)
        {
            _key_yellow_num->setPosition(Vec2(622.0f, 120.0f));
            player_node->addChild(_key_yellow_num);
        }

        for (int32_t i = 0; i < 2; ++i)
        {
            auto daoju_bg = Sprite::create("Images/ui_btn.png");
            if (nullptr != daoju_bg)
            {
                daoju_bg->setAnchorPoint(Vec2(0, 0.5f));
                daoju_bg->setPosition(Vec2(466.0f + i * 94.0f, 76.0f));
                player_node->addChild(daoju_bg);
            }
        }
    }
    
    return true;
}

void FloorMapLayer::on_player_attr_changed()
{
    const auto& player_info = Player::GetInstance()->get_player_info();
    _attack_num->setString(String::createWithFormat("%d", player_info.attack)->_string);
    _defend_num->setString(String::createWithFormat("%d", player_info.defence)->_string);
    _jb_num->setString(String::createWithFormat("%d", player_info.gold)->_string);
    _hun_num->setString(String::createWithFormat("%d", player_info.hun)->_string);
    _key_red_num->setString(String::createWithFormat("%d", player_info.key_red)->_string);
    _key_blue_num->setString(String::createWithFormat("%d", player_info.key_blue)->_string);
    _key_yellow_num->setString(String::createWithFormat("%d", player_info.key_yellow)->_string);
}

bool FloorMapLayer::onTouchBegan(Touch *touch, Event *e)
{
    return true;
}

void FloorMapLayer::onTouchEnded(Touch *touch, Event *e)
{
    // 暂时bug修正，如果对话框已弹出，就不再执行以下代码，这种情况是在对话框弹出前，touchbegan就已经触发的情况下发生，随后重构时应该不会有这个问题
    if (nullptr != this->getChildByTag(999))
    {
        return;
    }

    // 获取起始点
    auto end_vec2 = _tiled_map->convertTouchToNodeSpace(touch) / 75.0f;
    auto end_pt = node_t(end_vec2.x, end_vec2.y);
    if (!_paths.empty() && _paths.back() == end_pt)
    {
        return;
    }
    
    auto start_vec2 = _tiled_map->convertToNodeSpace(_warrior->getPosition()) / 75.0f;
    auto start_pt = node_t(start_vec2.x, start_vec2.y);
    if (start_pt == end_pt)
    {
        return;
    }


    // 获取阻碍点
    auto wall_layer = _tiled_map->getLayer("wall");
    auto wall_layer_size = wall_layer->getLayerSize();
    auto pos = touch->getLocation();
    auto tiles = wall_layer->getTiles();
    std::vector<node_t> blocks;
    for (uint32_t i = 0; i < (uint32_t)(wall_layer_size.height); ++i)
    {
        for (uint32_t j = 0; j < (uint32_t)(wall_layer_size.width); ++j)
        {
            if (tiles[i * (uint32_t)(wall_layer_size.width) + j] != 0)
            {
                blocks.push_back(node_t(j, (uint32_t)(wall_layer_size.height) - 1 - i));
            }
        }
    }

    if (std::find(blocks.begin(), blocks.end(), end_pt) != blocks.end() || end_pt.x >= 10 || end_pt.y >= 12)
    {
        return;
    }
    
    // npc列表
    auto npc_layer = _tiled_map->getLayer("npc");
    auto npc_layer_size = npc_layer->getLayerSize();
    auto npc_tiles = npc_layer->getTiles();
    _npcs.clear();
    for (uint32_t i = 0; i < (uint32_t)(npc_layer_size.height); ++i)
    {
        for (uint32_t j = 0; j < (uint32_t)(npc_layer_size.width); ++j)
        {
            int32_t gid = (int32_t)(npc_tiles[i * (uint32_t)(npc_layer_size.width) + j]);
            if (gid != 0)
            {
                _npcs.push_back(npc_t(j, npc_layer_size.height - 1 - i, gid));
            }
        }
    }

    // A星路径
    AStar astar(10, 12);
    astar.set_start_and_end(start_pt, end_pt);
    astar.set_blocks(blocks);
    _paths = astar.get_path();

    // 箭头
    _arrow_node->setVisible(true);
    _arrow_node->setPosition(Vec2((end_pt.x + 0.5f) * 75.0f - 50.0f, (end_pt.y + 0.5f) * 75.0f - 50.0f));

    // 路线
    uint32_t index = 0;
    _road_node->removeAllChildren();
    for (auto node : _paths)
    {
        auto pos = Vec2((node.x + 0.5f) * 75.0f - 50.0f, (node.y + 0.5f) * 75.0f - 50.0f);
        auto res = "Images/diban" + String::createWithFormat("%d", index % 22)->_string + ".png";
        auto sprite = Sprite::create(res);
        sprite->setPosition(pos);
        _road_node->addChild(sprite);
        ++index;
    }

    // 勇士
    _warrior->stopAllActions();
    _warrior->setTimeScale(2.0f);
    _warrior->setAnimation(0, "walk", true);
    step();
}

Value FloorMapLayer::get_tile_prop(int32_t gid, const string& key)
{
    Value properties = _tiled_map->getPropertiesForGID(gid);
    if (!properties.isNull())
    {
        auto tile_props = properties.asValueMap();
        if (tile_props.find(key) != tile_props.end())
        {
            return tile_props[key];
        }
    }

    return Value();
}

void FloorMapLayer::step()
{
    if (_paths.empty())
    {
        return;
    }

    auto current_pos = _warrior->getPosition();
    auto start_pt = _paths[0];
    auto start_pos = Vec2((start_pt.x + 0.5f) * 75.0f - 50.0f, (start_pt.y + 0.5f) * 75.0f - 50.0f);

    // 勇士位置在开始位置正中心时触发拾取操作，如果还没到，代表未到触发时机，如果已走过，代表触发前取消当前操作并改变路径
    if (current_pos.equals(start_pos))
    {
        auto npc_iter = find(_npcs.begin(), _npcs.end(), npc_t(start_pt.x, start_pt.y, 0));
        if (npc_iter != _npcs.end())
        {
            const auto& npc = *npc_iter;
            auto style = get_tile_prop(npc.gid, "style").asInt();
            switch (style)
            {
            case 2: // 钥匙
            {
                auto npc_layer = _tiled_map->getLayer("npc");
                auto key = npc_layer->getTileAt(Vec2(npc.x, 11 - npc.y));
                auto start_pos = this->convertToNodeSpace(key->getParent()->convertToWorldSpace(key->getPosition()));
                key->retain();
                key->removeFromParentAndCleanup(false);
                key->setPosition(start_pos);
                this->addChild(key);
                key->release();

                auto color = get_tile_prop(npc.gid, "color").asInt();
                switch (color)
                {
                case 0:
                    {
                        auto target_pos = this->convertToNodeSpace(_key_blue_num->getParent()->convertToWorldSpace(_key_blue_num->getPosition()));
                        target_pos -= Vec2(26.0f, 0);
                        auto duration = key->getPosition().distance(target_pos) / 1000.0f;
                        key->runAction(Sequence::create(
                            Spawn::createWithTwoActions(MoveTo::create(duration, target_pos), ScaleTo::create(duration, 0.6f)),
                            CallFunc::create([key]() {
                                key->removeFromParentAndCleanup(true);
                                PlayerDelegate::add_key(1, 1, 1);
                        }), nullptr));
                    }
                    break;
                    case 1:
                    {
                        auto target_pos = this->convertToNodeSpace(_key_red_num->getParent()->convertToWorldSpace(_key_red_num->getPosition()));
                        target_pos -= Vec2(26.0f, 0);
                        auto duration = key->getPosition().distance(target_pos) / 1000.0f;
                        key->runAction(Sequence::create(
                            Spawn::createWithTwoActions(MoveTo::create(duration, target_pos), ScaleTo::create(duration, 0.6f)),
                            CallFunc::create([key]() {
                                key->removeFromParentAndCleanup(true);
                                PlayerDelegate::add_key(1, 0, 0);
                        }), nullptr));
                    }
                    break;
                case 2:
                    {
                        auto target_pos = this->convertToNodeSpace(_key_blue_num->getParent()->convertToWorldSpace(_key_blue_num->getPosition()));
                        target_pos -= Vec2(26.0f, 0);
                        auto duration = key->getPosition().distance(target_pos) / 1000.0f;
                        key->runAction(Sequence::create(
                            Spawn::createWithTwoActions(MoveTo::create(duration, target_pos), ScaleTo::create(duration, 0.6f)),
                            CallFunc::create([key]() {
                                key->removeFromParentAndCleanup(true);
                                PlayerDelegate::add_key(0, 1, 0);
                        }), nullptr));
                    }
                    break;
                case 3:
                    {
                        auto target_pos = this->convertToNodeSpace(_key_yellow_num->getParent()->convertToWorldSpace(_key_yellow_num->getPosition()));
                        target_pos -= Vec2(26.0f, 0);
                        auto duration = key->getPosition().distance(target_pos) / 1000.0f;
                        key->runAction(Sequence::create(
                            Spawn::createWithTwoActions(MoveTo::create(duration, target_pos), ScaleTo::create(duration, 0.6f)),
                            CallFunc::create([key]() {
                                key->removeFromParentAndCleanup(true);
                                PlayerDelegate::add_key(0, 0, 1);
                        }), nullptr));
                    }
                    break;
                }
                
            }
            break;
            default:
                break;
            }
        }
    }
    
    if (_paths.size() > 1)
    {
        auto end_pt = _paths[1];
        auto end_pos = Vec2((end_pt.x + 0.5f) * 75.0f - 50.0f, (end_pt.y + 0.5f) * 75.0f - 50.0f);

        // 在没有四方向动画情况下，防止上下走动时频繁改变方向，加个判断
        if (current_pos.x != end_pos.x)
        {
            _warrior->setScaleX(end_pos.x > current_pos.x ? 0.1f : -0.1f);
        }

        // 如果需要额外走一段到瓦块中心，仅仅执行这段走路
        if (current_pos.distance(end_pos) > start_pos.distance(end_pos))
        {
            _warrior->runAction(Sequence::create(MoveTo::create(current_pos.distance(start_pos) / MOVE_SPEED, start_pos),
                CallFunc::create(CC_CALLBACK_0(FloorMapLayer::step, this)), nullptr));
            return;
        }

        // 淡出脚下的路径标识
        auto road_children = _road_node->getChildren();
        auto child = dynamic_cast<Node*>(road_children.at(road_children.size() - _paths.size()));
        if (nullptr != child)
        {
            child->runAction(FadeOut::create(0.2f));
        }

        bool walk_pause = false;
        auto npc_iter = find(_npcs.begin(), _npcs.end(), npc_t(end_pt.x, end_pt.y, 0));
        if (npc_iter != _npcs.end())
        {
            const auto& npc = *npc_iter;
            auto style = get_tile_prop(npc.gid, "style").asInt();
            switch (style)
            {
            case 1: // 怪物
                {
                    walk_pause = true;
                    _warrior->setAnimation(0, "gungrab", false);
                    auto dialog = PopupLayer::create("Images/UI_shared_bg.png", Size(600, 300));
                    dialog->setTag(999);
                    dialog->setTitle("confirm");
                    dialog->setContentText("Do you wang to attack this guy?");
                    dialog->setCallbackFunc(this, CC_CALLFUNCN_SELECTOR(FloorMapLayer::confirm_attack));
                    dialog->addButton("Images/UI_tip_fangqi.png", "Images/UI_tip_fangqi.png", "", 0);
                    dialog->addButton("Images/UI_tip_queding.png", "Images/UI_tip_queding.png", "", 1);
                    this->addChild(dialog);
                }
                break;
            case 3: // 门
                {

                }
                break;
            default:
                break;
            }
        }
        
        if (!walk_pause)
        {
            _warrior->runAction(Sequence::createWithTwoActions(MoveTo::create(current_pos.distance(end_pos) / MOVE_SPEED, end_pos),
                CallFunc::create(CC_CALLBACK_0(FloorMapLayer::step, this))));
            _paths.erase(_paths.begin());
        }
    }
    else
    {
        _road_node->removeAllChildren();
        _warrior->setAnimation(0, "gungrab", false);
        _arrow_node->setVisible(false);
    }
}

void FloorMapLayer::confirm_attack(Node* node)
{
    if (node->getTag() == 0)
    {
        _road_node->removeAllChildren();
        _warrior->setAnimation(0, "gungrab", false);
        _arrow_node->setVisible(false);
    }
    else
    {
        _warrior->setAnimation(1, "gungrab", false);
        _warrior->setEndListener([&](int trackIndex) {
            if (trackIndex == 1)
            {
                _warrior->setAnimation(0, "walk", true);
                auto current_pos = _warrior->getPosition();
                auto end_pt = _paths[1];
                auto end_pos = Vec2((end_pt.x + 0.5f) * 75.0f - 50.0f, (end_pt.y + 0.5f) * 75.0f - 50.0f);
                _warrior->runAction(Sequence::createWithTwoActions(MoveTo::create(current_pos.distance(end_pos) / MOVE_SPEED, end_pos),
                    CallFunc::create(CC_CALLBACK_0(FloorMapLayer::step, this))));
                _paths.erase(_paths.begin());
            }
        });
    }
}