﻿#include "FloorMapLayer.h"
#include "spine/spine-cocos2dx.h"
#include "cocostudio/CCArmature.h"
#include "AStar.h"
#include "Player.h"
#include "ModalDialog.h"
#include "Warrior.h"
#include "WarriorInfoPanel.h"
#include "ArrowNode.h"

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
    // 由于直接继承自node，需要做一些处理，现在layer的功能全被废弃了，无意义
    setContentSize(Director::getInstance()->getWinSize());

    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    listener->onTouchBegan = CC_CALLBACK_2(FloorMapLayer::onTouchBegan, this);
    listener->onTouchEnded = CC_CALLBACK_2(FloorMapLayer::onTouchEnded, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

    // 加载地图编辑器文件
    _tiled_map = cocos2d::experimental::TMXTiledMap::create("TileMaps/floor001.tmx");
    if (nullptr != _tiled_map)
    {
        _tiled_map->setPosition(Vec2(-50.0f, -50.0f));
        this->addChild(_tiled_map);
    }

    // 路径节点
    _road_node = Node::create();
    if (nullptr != _road_node)
    {
        _road_node->setPosition(Vec2::ZERO);
        this->addChild(_road_node);
    }

    // 勇士骨骼动画节点
    _warrior = WarriorNode::create();
    /*cocostudio::CCBone* bone = _warrior->getBone("sheild1");
    int index = bone->getDisplayManager()->getCurrentDisplayIndex();
    bone->removeDisplay(1);
    bone->changeDisplayByIndex(-1, true);
    cocostudio::CCBone* bone2 = _warrior->getBone("sheild2");
    index = bone2->getDisplayManager()->getCurrentDisplayIndex();
    bone2->removeDisplay(1);
    bone2->changeDisplayByIndex(-1, true);*/
    _warrior->setPosition(Vec2(62.5f, 62.5f));
    this->addChild(_warrior);

    // 上方背景
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

    // 点击箭头节点
    _arrow_node = ArrowNode::create();
    if (nullptr != _arrow_node)
    {
        _arrow_node->setPosition(Vec2::ZERO);
        _arrow_node->setVisible(false);
        this->addChild(_arrow_node);
    }

    // 勇士属性面板
    _info_panel = WarriorInfoPanel::create();
    if (nullptr != _info_panel)
    {
        _info_panel->setPosition(Vec2(-10.0f, 825.0f));
        this->addChild(_info_panel);
    }
    
    return true;
}

bool FloorMapLayer::onTouchBegan(Touch *touch, Event *e)
{
    return true;
}

void FloorMapLayer::onTouchEnded(Touch *touch, Event *e)
{
    // bugfix: 如果对话框已弹出，就不再执行以下代码，这种情况是在对话框弹出前，touchbegan就已经触发的情况下发生
    // 重现方法: 控制勇士经过怪物，然后鼠标按下等待勇士到达怪物处，弹出提示对话框后，松开鼠标继续弹出
    if (ModalDialogManager::GetInstance()->isShown())
    {
        return;
    }

    // 正在战斗中则不允许重新走动，以免计算混乱
    if (_warrior->is_fighting())
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

void FloorMapLayer::pick_up_item_impl(const npc_t& npc, const cocos2d::Vec2& target_pos, const std::function<void()>& callback)
{
    auto npc_layer = _tiled_map->getLayer("npc");
    auto item = npc_layer->getTileAt(Vec2(npc.x, 11 - npc.y));
    auto start_pos = this->convertToNodeSpace(item->getParent()->convertToWorldSpace(item->getPosition()));
    item->retain();
    item->removeFromParentAndCleanup(false);
    item->setPosition(start_pos);
    this->addChild(item);
    item->release();

    // TODO.. cocos2d-x tiled bug. 如果一个Layer只剩下一个tile，设置gid为0不起作用，目前找不到解决办法
    npc_layer->setTileGID(999, Vec2(npc.x, 11 - npc.y));

    auto duration = item->getPosition().distance(target_pos) / 1000.0f;
    item->runAction(Sequence::create(
        Spawn::createWithTwoActions(MoveTo::create(duration, target_pos), ScaleTo::create(duration, 0.6f)),
        CallFunc::create([item, callback]() {
        item->removeFromParentAndCleanup(true);
        callback();
    }), nullptr));
}

void FloorMapLayer::pick_up_item(const npc_t& npc)
{
    std::function<void()> callback;
    WarriorInfoPanel::node_type type;

    bool need_pick = true;
    auto style = get_tile_prop(npc.gid, "style").asInt();
    switch (style)
    {
    case 2: // 钥匙
        {
            auto color = get_tile_prop(npc.gid, "color").asInt();
            if (0 == color) // 钥匙串
            {
                type = WarriorInfoPanel::key_blue;
                callback = std::bind(&PlayerDelegate::add_key, 1, 1, 1);
            }
            else if (1 == color) // 红钥匙
            {
                type = WarriorInfoPanel::key_red;
                callback = std::bind(&PlayerDelegate::add_key, 1, 0, 0);
            }
            else if (2 == color) // 蓝钥匙
            {
                type = WarriorInfoPanel::key_blue;
                callback = std::bind(&PlayerDelegate::add_key, 0, 1, 0);
            }
            else if (3 == color) // 黄钥匙
            {
                type = WarriorInfoPanel::key_yellow;
                callback = std::bind(&PlayerDelegate::add_key, 0, 0, 1);
            }
        }
        break;
    case 4: // 宝石
        {
            auto color = get_tile_prop(npc.gid, "color").asInt();
            auto value = get_tile_prop(npc.gid, "value").asInt();
            if (1 == color) // 红宝石，攻击
            {
                type = WarriorInfoPanel::attack;
                callback = std::bind(&PlayerDelegate::add_attack, value);
            }
            else if (2 == color) // 蓝宝石，防御
            {
                type = WarriorInfoPanel::defend;
                callback = std::bind(&PlayerDelegate::add_defence, value);
            }
        }
        break;
    case 5: // 血瓶
        {
            auto value = get_tile_prop(npc.gid, "value").asInt();
            type = WarriorInfoPanel::hp;
            callback = std::bind(&PlayerDelegate::add_hp, value);
        }
        break;
    default:
        need_pick = false;
        break;
    }

    if (need_pick)
    {
        auto target_pos = this->convertToNodeSpace(_info_panel->get_node_position_in_world(type));
        target_pos -= Vec2(26.0f, 0);
        pick_up_item_impl(npc, target_pos, callback);
    }
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
            pick_up_item(npc);
        }
    }
    
    // 未走到路径末尾，继续处理下一步行走
    if (_paths.size() > 1)
    {
        auto end_pt = _paths[1];
        auto end_pos = Vec2((end_pt.x + 0.5f) * 75.0f - 50.0f, (end_pt.y + 0.5f) * 75.0f - 50.0f);

        // 如果需要额外走一段到瓦块中心，仅仅执行这段走路，仅在走到块间时重新改变行走路径时有效
        if (current_pos.distance(end_pos) > start_pos.distance(end_pos))
        {
            _warrior->move_to(start_pos, CC_CALLBACK_0(FloorMapLayer::step, this));
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
            walk_pause = interact_item(npc);
        }
        
        if (!walk_pause)
        {
            _warrior->move_to(end_pos, CC_CALLBACK_0(FloorMapLayer::step, this));
            _paths.erase(_paths.begin());
        }
    }
    else // 走到寻路最后一个节点，清理工作
    {
        _road_node->removeAllChildren();
        _warrior->stand_auto();
        _arrow_node->setVisible(false);
    }
}

bool FloorMapLayer::interact_item(const npc_t& npc)
{
    bool walk_pause = false;
    auto style = get_tile_prop(npc.gid, "style").asInt();
    switch (style)
    {
    case 1: // 怪物
    {
        walk_pause = true;
        auto end_pos = Vec2((npc.x + 0.5f) * 75.0f - 50.0f, (npc.y + 0.5f) * 75.0f - 50.0f);
        _warrior->turn_to(end_pos);

        if (_paths.size() > 2) // 怪物为行走路径的最后一个节点，不提示对话框，直接开打
        {
            auto dict = FileUtils::getInstance()->getValueMapFromFile("chinese.xml");
            auto text = dict["isAttack"].asString();
            auto name = dict[get_tile_prop(npc.gid, "name").asString()].asString();
            auto info = text + " " + name;
            auto dialog = OKCancelDialog::create("", info);
            dialog->setCallback(std::bind(&FloorMapLayer::confirm_attack, this, std::placeholders::_1, npc));
            ModalDialogManager::GetInstance()->pushDialog(dialog);
        }
        else
        {
            assert(_paths.size() == 2); // 和npc交互时路径肯定还有至少两个节点，一个自己的，一个对方的
            if (_paths.size() == 2)
                confirm_attack(OKCancelDialog::OK, npc);
        }
    }
    break;
    case 3: // 门
    {

    }
    break;
    default:
        break;
    }

    return walk_pause;
}

void FloorMapLayer::confirm_attack(OKCancelDialog::RETURN_TYPE type, const npc_t& npc)
{
    if (type == OKCancelDialog::CANCEL)
    {
        _road_node->removeAllChildren();
        _warrior->stand_auto();
        _arrow_node->setVisible(false);
        _paths.clear();
    }
    else
    {
        static int attack_count = 3;
        attack_count = 3;
        _warrior->fight_auto(3, std::bind(&FloorMapLayer::confirm_attack_impl, this, npc));
    }
}

void FloorMapLayer::confirm_attack_impl(const npc_t& npc)
{
    auto life = get_tile_prop(npc.gid, "life").asInt();
    auto hun = get_tile_prop(npc.gid, "hun").asInt();
    auto gold = get_tile_prop(npc.gid, "gold").asInt();
    auto defence = get_tile_prop(npc.gid, "defence").asInt();
    auto attack = get_tile_prop(npc.gid, "attack").asInt();

    auto player_info = Player::GetInstance()->get_player_info();
    auto attack_damage = std::max(0, player_info.attack - defence);
    auto damage = std::max(0, attack - player_info.defence);
    if (attack_damage == 0 || (life + attack_damage - 1) / attack_damage * damage >= player_info.hp) // 只要攻击不够无论防御多高都打不过
    {
        auto dict = FileUtils::getInstance()->getValueMapFromFile("chinese.xml");
        auto text = dict["txt7_2"].asString();
        PromptDialog::show(text);
        _road_node->removeAllChildren();
        _warrior->stand_auto();
        _arrow_node->setVisible(false);
        _paths.clear();
        return;
    }

    auto blood = (life + attack_damage - 1) / attack_damage * damage;

    auto target_pos = this->convertToNodeSpace(_info_panel->get_node_position_in_world(WarriorInfoPanel::hun));
    target_pos -= Vec2(26.0f, 0);
    pick_up_item_impl(npc, target_pos, [blood, gold, hun]() {
        PlayerDelegate::add_hp(-blood);
        PlayerDelegate::add_gold_hun(gold, hun);
    });

    auto current_pos = _warrior->getPosition();
    auto end_pt = _paths[1];
    auto end_pos = Vec2((end_pt.x + 0.5f) * 75.0f - 50.0f, (end_pt.y + 0.5f) * 75.0f - 50.0f);
    _warrior->move_to(end_pos, CC_CALLBACK_0(FloorMapLayer::step, this));
    _paths.erase(_paths.begin());
}
