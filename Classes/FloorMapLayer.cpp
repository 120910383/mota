#include "FloorMapLayer.h"
#include "spine/spine-cocos2dx.h"
#include "AStar.h"

USING_NS_CC;

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

    auto player_node = Node::create();
    if (nullptr != player_node)
    {
        player_node->setPosition(Vec2(0.0f, 825.0f));
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

            auto attack_num = LabelAtlas::create("23965", "Images/ps_num_shared.png", 21, 29, '0');
            if (nullptr != attack_num)
            {
                attack_num->setScaleX(0.8f);
                attack_num->setAnchorPoint(Vec2(0.0f, 0.5f));
                attack_num->setPosition(Vec2(40.0f, player_attack_icon->getContentSize().height / 2));
                player_attack_icon->addChild(attack_num);
            }
        }

        auto player_defend_icon = Sprite::create("Images/ui_defend.png");
        if (nullptr != player_defend_icon)
        {
            player_defend_icon->setPosition(Vec2(330.0f, 140.0f));
            player_node->addChild(player_defend_icon);

            auto defend_num = LabelAtlas::create("13432", "Images/ps_num_shared.png", 21, 29, '0');
            if (nullptr != defend_num)
            {
                defend_num->setScaleX(0.8f);
                defend_num->setAnchorPoint(Vec2(0.0f, 0.5f));
                defend_num->setPosition(Vec2(40.0f, player_defend_icon->getContentSize().height / 2));
                player_defend_icon->addChild(defend_num);
            }
        }

        auto player_jb_icon = Sprite::create("Images/ui_jb.png");
        if (nullptr != player_jb_icon)
        {
            player_jb_icon->setPosition(Vec2(180.0f, 90.0f));
            player_node->addChild(player_jb_icon);

            auto jb_num = LabelAtlas::create("33123", "Images/ps_num_shared.png", 21, 29, '0');
            if (nullptr != jb_num)
            {
                jb_num->setScaleX(0.8f);
                jb_num->setAnchorPoint(Vec2(0.0f, 0.5f));
                jb_num->setPosition(Vec2(40.0f, player_jb_icon->getContentSize().height / 2));
                player_jb_icon->addChild(jb_num);
            }
        }

        auto player_hun_icon = Sprite::create("Images/ui_hun.png");
        if (nullptr != player_hun_icon)
        {
            player_hun_icon->setPosition(Vec2(330.0f, 90.0f));
            player_node->addChild(player_hun_icon);

            auto hun_num = LabelAtlas::create("21012", "Images/ps_num_shared.png", 21, 29, '0');
            if (nullptr != hun_num)
            {
                hun_num->setScaleX(0.8f);
                hun_num->setAnchorPoint(Vec2(0.0f, 0.5f));
                hun_num->setPosition(Vec2(40.0f, player_hun_icon->getContentSize().height / 2));
                player_hun_icon->addChild(hun_num);
            }
        }
    }
    
    return true;
}

bool FloorMapLayer::onTouchBegan(Touch *touch, Event *e)
{
    return true;
}

void FloorMapLayer::onTouchEnded(Touch *touch, Event *e)
{
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

    auto end_pos = _tiled_map->convertTouchToNodeSpace(touch);
    auto end_pt = node_t(end_pos.x / 75.0f, end_pos.y / 75.0f);
    if (std::find(blocks.begin(), blocks.end(), end_pt) != blocks.end() || end_pt.x >= 10 || end_pt.y >= 12)
    {
        return;
    }
    auto start_pos = _tiled_map->convertToNodeSpace(_warrior->getPosition());
    auto start_pt = node_t(start_pos.x / 75.0f, start_pos.y / 75.0f);
    AStar astar(10, 12);
    astar.set_start_and_end(start_pt, end_pt);
    astar.set_blocks(blocks);
    vector<node_t> turns;
    AStar::translate_path_to_turns(astar.get_path(), turns);

    _warrior->setTimeScale(2.0f);
    _warrior->setAnimation(0, "walk", true);
    Vector<FiniteTimeAction*> actions;
    for (int i = 0; i < turns.size(); ++i)
    {
        if (i == 0) {
            continue;
        }
        
        auto pre_turn = turns[i - 1];
        auto cur_turn = turns[i];

        actions.pushBack(CallFunc::create([&, pre_turn, cur_turn](){
            if (cur_turn.x != pre_turn.x) {
                _warrior->setScaleX(cur_turn.x > pre_turn.x ? 0.1f : -0.1f);
            }
        }));

        if (i == 1) {
            auto distance = abs((cur_turn.x + 0.5f) * 75.0f - start_pos.x) + abs((cur_turn.y + 0.5f) * 75.0f - start_pos.y);
            actions.pushBack(MoveTo::create(distance / 200.0f, Vec2((cur_turn.x + 0.5f) * 75.0f - 50.0f, (cur_turn.y + 0.5f) * 75.0f - 50.0f)));
        } else {
            auto distance = (abs(cur_turn.x - pre_turn.x) + abs(cur_turn.y - pre_turn.y)) * 75.0f;
            actions.pushBack(MoveTo::create(distance / 200.0f, Vec2((cur_turn.x + 0.5f) * 75.0f - 50.0f, (cur_turn.y + 0.5f) * 75.0f - 50.0f)));
        }
    }
    
    actions.pushBack(CallFunc::create([&](){ _warrior->setAnimation(0, "gungrab", false); }));
    auto action = Sequence::create(actions);
    _warrior->stopAllActions();
    _warrior->runAction(action);
}
