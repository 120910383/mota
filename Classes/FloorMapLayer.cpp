#include "FloorMapLayer.h"
#include "spine/spine-cocos2dx.h"
#include "cocostudio/CCArmature.h"
#include "AStar.h"
#include "Player.h"
#include "ModalDialog.h"
#include "Warrior.h"
#include "WarriorInfoPanel.h"

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
    // 暂时bug修正，如果对话框已弹出，就不再执行以下代码，这种情况是在对话框弹出前，touchbegan就已经触发的情况下发生，随后重构时应该不会有这个问题
    if (ModalDialogManager::GetInstance()->isShown())
    {
        return;
    }

    // 正在战斗中则不允许重新走动，以免计算混乱
    if (_warrior->is_fighting())
    {
        return;
    }

    //auto npc_layer1 = _tiled_map->getLayer("npc");
    //auto npc_layer_size1 = npc_layer1->getLayerSize();
    //int tilesAmount = npc_layer_size1.width * npc_layer_size1.height;
    //uint32_t *tiles1 = (uint32_t*)malloc(tilesAmount*sizeof(uint32_t));
    //memset(tiles1, 0, tilesAmount*sizeof(int));
    //tiles1[85] = 30;
    //tiles1[103] = 29;
    //npc_layer1->setTiles(tiles1);

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
                    // TODO.. cocos2d-x tiled bug. 如果一个Layer只剩下一个tile，设置gid为0不起作用，目前找不到解决办法
                    npc_layer->setTileGID(999, Vec2(npc.x, 11 - npc.y));

                    auto color = get_tile_prop(npc.gid, "color").asInt();
                    switch (color)
                    {
                    case 0:
                        {
                            auto target_pos = this->convertToNodeSpace(_info_panel->get_node_position_in_world(WarriorInfoPanel::key_blue));
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
                            auto target_pos = this->convertToNodeSpace(_info_panel->get_node_position_in_world(WarriorInfoPanel::key_red));
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
                            auto target_pos = this->convertToNodeSpace(_info_panel->get_node_position_in_world(WarriorInfoPanel::key_blue));
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
                            auto target_pos = this->convertToNodeSpace(_info_panel->get_node_position_in_world(WarriorInfoPanel::key_yellow));
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
                    default:
                        break;
                    }
                }
                break;
            case 4: // 宝石
                {
                    auto npc_layer = _tiled_map->getLayer("npc");
                    auto diamond = npc_layer->getTileAt(Vec2(npc.x, 11 - npc.y));
                    auto start_pos = this->convertToNodeSpace(diamond->getParent()->convertToWorldSpace(diamond->getPosition()));
                    diamond->retain();
                    diamond->removeFromParentAndCleanup(false);
                    diamond->setPosition(start_pos);
                    this->addChild(diamond);
                    diamond->release();
                    // TODO.. cocos2d-x tiled bug. 如果一个Layer只剩下一个tile，设置gid为0不起作用，目前找不到解决办法
                    npc_layer->setTileGID(999, Vec2(npc.x, 11 - npc.y));

                    auto color = get_tile_prop(npc.gid, "color").asInt();
                    switch (color)
                    {
                    case 1:
                        {
                            auto target_pos = this->convertToNodeSpace(_info_panel->get_node_position_in_world(WarriorInfoPanel::attack));
                            target_pos -= Vec2(26.0f, 0);
                            auto duration = diamond->getPosition().distance(target_pos) / 1000.0f;
                            auto value = get_tile_prop(npc.gid, "value").asInt();
                            diamond->runAction(Sequence::create(
                                Spawn::createWithTwoActions(MoveTo::create(duration, target_pos), ScaleTo::create(duration, 0.6f)),
                                CallFunc::create([diamond, value]() {
                                diamond->removeFromParentAndCleanup(true);
                                PlayerDelegate::add_attack(value);
                            }), nullptr));
                        }
                        break;
                    case 2:
                        {
                            auto target_pos = this->convertToNodeSpace(_info_panel->get_node_position_in_world(WarriorInfoPanel::defend));
                            target_pos -= Vec2(26.0f, 0);
                            auto duration = diamond->getPosition().distance(target_pos) / 1000.0f;
                            auto value = get_tile_prop(npc.gid, "value").asInt();
                            diamond->runAction(Sequence::create(
                                Spawn::createWithTwoActions(MoveTo::create(duration, target_pos), ScaleTo::create(duration, 0.6f)),
                                CallFunc::create([diamond, value]() {
                                diamond->removeFromParentAndCleanup(true);
                                PlayerDelegate::add_defence(value);
                            }), nullptr));
                        }
                        break;
                    default:
                        break;
                    }
                }
                break;
            case 5: // 血瓶
                {
                    auto npc_layer = _tiled_map->getLayer("npc");
                    auto blood = npc_layer->getTileAt(Vec2(npc.x, 11 - npc.y));
                    auto start_pos = this->convertToNodeSpace(blood->getParent()->convertToWorldSpace(blood->getPosition()));
                    blood->retain();
                    blood->removeFromParentAndCleanup(false);
                    blood->setPosition(start_pos);
                    this->addChild(blood);
                    blood->release();
                    // TODO.. cocos2d-x tiled bug. 如果一个Layer只剩下一个tile，设置gid为0不起作用，目前找不到解决办法
                    npc_layer->setTileGID(999, Vec2(npc.x, 11 - npc.y));

                    auto target_pos = this->convertToNodeSpace(_info_panel->get_node_position_in_world(WarriorInfoPanel::hp));
                    target_pos -= Vec2(26.0f, 0);
                    auto duration = blood->getPosition().distance(target_pos) / 1000.0f;
                    auto value = get_tile_prop(npc.gid, "value").asInt();
                    blood->runAction(Sequence::create(
                        Spawn::createWithTwoActions(MoveTo::create(duration, target_pos), ScaleTo::create(duration, 0.6f)),
                        CallFunc::create([blood, value]() {
                        blood->removeFromParentAndCleanup(true);
                        PlayerDelegate::add_hp(value);
                    }), nullptr));
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

        // 如果需要额外走一段到瓦块中心，仅仅执行这段走路
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
            auto style = get_tile_prop(npc.gid, "style").asInt();
            switch (style)
            {
            case 1: // 怪物
                {
                    walk_pause = true;
                    _warrior->turn_to(end_pos);
                    //auto dict = Dictionary::createWithContentsOfFile("chinese.xml");
                    //auto text = (static_cast<String*>(dict->objectForKey("isAttack")))->getCString();
                    //auto name = (static_cast<String*>(dict->objectForKey(get_tile_prop(npc.gid, "name").asString())))->getCString();
					auto dict = FileUtils::getInstance()->getValueMapFromFile("chinese.xml");
					auto text = dict["isAttack"].asString();
					auto name = dict[get_tile_prop(npc.gid, "name").asString()].asString();
					auto info = text + " " + name;
                    auto dialog = OKCancelDialog::create("", info);
                    dialog->setCallback(std::bind(&FloorMapLayer::confirm_attack, this, std::placeholders::_1, npc));
                    ModalDialogManager::GetInstance()->pushDialog(dialog);
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
            _warrior->move_to(end_pos, CC_CALLBACK_0(FloorMapLayer::step, this));
            _paths.erase(_paths.begin());
        }
    }
    else
    {
        _road_node->removeAllChildren();
        _warrior->stand_auto();
        _arrow_node->setVisible(false);
    }
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
        auto dict = Dictionary::createWithContentsOfFile("chinese.xml");
        auto text = (static_cast<String*>(dict->objectForKey("txt7_2")))->getCString();
        PromptDialog::show(text);
        _road_node->removeAllChildren();
        _warrior->stand_auto();
        _arrow_node->setVisible(false);
        _paths.clear();
        return;
    }

    auto blood = (life + attack_damage - 1) / attack_damage * damage;

    auto npc_layer = _tiled_map->getLayer("npc");
    auto monster = npc_layer->getTileAt(Vec2(npc.x, 11 - npc.y));
    auto start_pos = this->convertToNodeSpace(monster->getParent()->convertToWorldSpace(monster->getPosition()));
    monster->retain();
    monster->removeFromParentAndCleanup(false);
    monster->setPosition(start_pos);
    this->addChild(monster);
    monster->release();
    // TODO.. cocos2d-x tiled bug. 如果一个Layer只剩下一个tile，设置gid为0不起作用，目前找不到解决办法
    npc_layer->setTileGID(999, Vec2(npc.x, 11 - npc.y));

    auto target_pos = this->convertToNodeSpace(_info_panel->get_node_position_in_world(WarriorInfoPanel::hun));
    target_pos -= Vec2(26.0f, 0);
    auto duration = monster->getPosition().distance(target_pos) / 1000.0f;
    monster->runAction(Sequence::create(
        Spawn::createWithTwoActions(MoveTo::create(duration, target_pos), ScaleTo::create(duration, 0.6f)),
        CallFunc::create([monster, blood, gold, hun]() {
        monster->removeFromParentAndCleanup(true);
        PlayerDelegate::add_hp(-blood);
        PlayerDelegate::add_gold_hun(gold, hun);
    }), nullptr));

    auto current_pos = _warrior->getPosition();
    auto end_pt = _paths[1];
    auto end_pos = Vec2((end_pt.x + 0.5f) * 75.0f - 50.0f, (end_pt.y + 0.5f) * 75.0f - 50.0f);
    _warrior->move_to(end_pos, CC_CALLBACK_0(FloorMapLayer::step, this));
    _paths.erase(_paths.begin());
}
