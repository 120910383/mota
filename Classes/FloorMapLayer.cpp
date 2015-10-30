#include "pre_header.h"

#include "FloorMapLayer.h"
#include "Player.h"
#include "ModalDialog.h"
#include "Warrior.h"
#include "WarriorInfoPanel.h"
#include "ArrowNode.h"

#include "cocostudio/CCArmature.h"

USING_NS_CC;

Scene* FloorMapLayer::scene(int floor, bool up)
{
    FloorMapLayer* floor_layer = new (std::nothrow) FloorMapLayer();
    if (floor_layer && floor_layer->init(floor, up))
    {
        floor_layer->autorelease();
        auto ret_scene = Scene::create();
        ret_scene->addChild(floor_layer);
        return ret_scene;
    }
    else
    {
        CC_SAFE_DELETE(floor_layer);
        return nullptr;
    }
}

bool FloorMapLayer::init(int floor, bool up)
{
    _floor = floor;
    // 由于直接继承自node，需要做一些处理，现在layer的功能全被废弃了，无意义
    setContentSize(Director::getInstance()->getWinSize());

    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    listener->onTouchBegan = CC_CALLBACK_2(FloorMapLayer::onTouchBegan, this);
    listener->onTouchEnded = CC_CALLBACK_2(FloorMapLayer::onTouchEnded, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

    // 加载地图编辑器文件
    auto res = "TileMaps/floor00" + String::createWithFormat("%d", floor)->_string + ".tmx";
    _tiled_map = cocos2d::experimental::TMXTiledMap::create(res);
    if (nullptr != _tiled_map)
    {
        _tiled_map->setPosition(Vec2(-50.0f, -50.0f));
        this->addChild(_tiled_map);
    }

    auto npc_layer = _tiled_map->getLayer("npc");
    auto npc_layer_size = npc_layer->getLayerSize();
    auto npc_tiles = npc_layer->getTiles();
    std::vector<Floor::npc_t> npcs;
    for (uint32_t i = 0; i < (uint32_t)(npc_layer_size.height); ++i)
    {
        for (uint32_t j = 0; j < (uint32_t)(npc_layer_size.width); ++j)
        {
            int32_t gid = (int32_t)(npc_tiles[i * (uint32_t)(npc_layer_size.width) + j]);
            if (gid != 0)
            {
                const Floor::floor_t& floor = Floor::GetInstance()->get_floor_info(_floor);
                Floor::npc_t npc(j, npc_layer_size.height - 1 - i, gid);
                if (floor.is_valid())
                {
                    auto iter = std::find(floor.npcs.begin(), floor.npcs.end(), npc);
                    if (iter == floor.npcs.end())
                    {
                        npc_layer->setTileGID(999, Vec2(j, i));
                    }
                }
                npcs.push_back(npc);
                if (get_tile_prop(gid, "style").asInt() == 6)
                {
                    if (get_tile_prop(gid, "type").asInt() == 2)
                        _stair_up = npcs.back();
                    else
                        _stair_down = npcs.back();
                }
            }
        }
    }

    Floor::GetInstance()->auto_init_floor(_floor, npcs);

    // 楼层数字
    
    TMXTileFlags flags = (TMXTileFlags)0;
    Vec2 tilePos(_stair_down.pos.x, npc_layer_size.height - 1 - _stair_down.pos.y);
    npc_layer->getTileGIDAt(tilePos, &flags);
    tilePos.x += flags == kTMXTileHorizontalFlag ? -1 : 1;
    auto floor_layer = _tiled_map->getLayer("floor");
    Vec2 floor_pos = floor_layer->getPositionAt(tilePos);
    auto num_label = LabelAtlas::create(String::createWithFormat("%d", floor)->_string, "Images/lv_num.png", 24, 38, '0');
    if (nullptr != num_label)
    {
        Size tile_size = floor_layer->getMapTileSize();
        num_label->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        num_label->setPosition(floor_pos + Vec2(tile_size.width / 2, tile_size.height / 2));
        floor_layer->addChild(num_label);
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
    Vec2 stair_pos = up ? Vec2(_stair_down.pos.x, _stair_down.pos.y) :
        Vec2(_stair_up.pos.x, _stair_up.pos.y);
    flags = (TMXTileFlags)0;
    npc_layer->getTileGIDAt(Vec2(stair_pos.x, npc_layer_size.height - 1 - stair_pos.y), &flags);
    stair_pos.x += flags == kTMXTileHorizontalFlag ? -1 : 1;
    _warrior->setPosition(Vec2((stair_pos.x + 0.5f) * 75.0f - 50.0f, (stair_pos.y + 0.5f) * 75.0f - 50.0f));
    Vec2 target_pos = stair_pos;
    target_pos.x += flags == kTMXTileHorizontalFlag ? -1 : 1;
    _warrior->turn_to(Vec2((target_pos.x + 0.5f) * 75.0f - 50.0f, (target_pos.y + 0.5f) * 75.0f - 50.0f));
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

    // 正在战斗中或自定义忙碌（如开门等待）中则不允许重新走动，以免计算混乱
    if (_warrior->is_fighting() || _warrior->is_lock())
    {
        return;
    }

    // 获取起始点
    auto end_vec2 = _tiled_map->convertTouchToNodeSpace(touch) / 75.0f;
    auto end_pt = Floor::position_t(end_vec2.x, end_vec2.y);
    if (!_paths.empty() && _paths.back() == end_pt)
    {
        return;
    }
    
    auto start_vec2 = _tiled_map->convertToNodeSpace(_warrior->getPosition()) / 75.0f;
    auto start_pt = Floor::position_t(start_vec2.x, start_vec2.y);
    if (start_pt == end_pt)
    {
        return;
    }

    // 获取阻碍点
    auto wall_layer = _tiled_map->getLayer("wall");
    auto wall_layer_size = wall_layer->getLayerSize();
    auto pos = touch->getLocation();
    auto tiles = wall_layer->getTiles();
    std::vector<AStar::node_t> blocks;
    for (uint32_t i = 0; i < (uint32_t)(wall_layer_size.height); ++i)
    {
        for (uint32_t j = 0; j < (uint32_t)(wall_layer_size.width); ++j)
        {
            if (tiles[i * (uint32_t)(wall_layer_size.width) + j] != 0)
            {
                blocks.push_back(AStar::node_t(j, (uint32_t)(wall_layer_size.height) - 1 - i));
            }
        }
    }

    if (std::find(blocks.begin(), blocks.end(), AStar::node_t(end_pt.x, end_pt.y)) != blocks.end() || end_pt.x >= 10 || end_pt.y >= 12)
    {
        return;
    }

    // A星路径
    AStar astar(10, 12);
    astar.set_start_and_end(AStar::node_t(start_pt.x, start_pt.y), AStar::node_t(end_pt.x, end_pt.y));
    astar.set_blocks(blocks);
    std::vector<AStar::node_t> paths = astar.get_path();
    auto stair_iter = std::find_if(paths.begin(), paths.end(), [&](AStar::node_t node) {
        // 找到除了首节点的第一个楼梯节点
        Floor::position_t pos(node.x, node.y);
        return pos != start_pt && (pos == _stair_up.pos || pos == _stair_down.pos);
    });

    if (stair_iter != paths.end())  // 如果路径经过楼梯，则特殊处理
    {
        paths.erase(stair_iter + 1, paths.end());     // 首先楼梯之后的路径节点无效删除

        auto npc_layer = _tiled_map->getLayer("npc");
        Vec2 stay_pos = Vec2(stair_iter->x, stair_iter->y);
        TMXTileFlags flags = (TMXTileFlags)0;
        npc_layer->getTileGIDAt(Vec2(stay_pos.x, npc_layer->getLayerSize().height - 1 - stay_pos.y), &flags);
        stay_pos.x += flags == kTMXTileHorizontalFlag ? -1 : 1;

        auto floor_iter = std::find_if(paths.begin(), paths.end(), [&](AStar::node_t node) {
            // 找到该楼梯前停留处地板
            return node.x == stay_pos.x && node.y == stay_pos.y;
        });
        if (floor_iter == paths.end())  // 如果经过楼梯的路径没有经过楼梯前的停留地板，重新寻路到地板节点
        {
            AStar astar(10, 12);
            astar.set_start_and_end(AStar::node_t(start_pt.x, start_pt.y), AStar::node_t(stay_pos.x, stay_pos.y));
            blocks.push_back(AStar::node_t(stair_iter->x, stair_iter->y));  // 楼梯不能经过，添加到阻碍点列表中
            astar.set_blocks(blocks);
            AStar::node_t stair_node(stair_iter->x, stair_iter->y); // 先保存一下，因为下一句会导致iter无效
            paths = astar.get_path();
            paths.push_back(stair_node);   // 寻路后添加要到达的楼梯节点
        }
    }

    _paths.clear();
    for (const auto& node : paths)
    {
        _paths.push_back(Floor::position_t(node.x, node.y));
    }

    // 箭头
    _arrow_node->setVisible(true);
    _arrow_node->setPosition(Vec2((end_pt.x + 0.5f) * 75.0f - 50.0f, (end_pt.y + 0.5f) * 75.0f - 50.0f));

    // 路线
    uint32_t index = 0;
    _road_node->removeAllChildren();
    for (auto node : _paths)
    {
        if (index == _paths.size() - 1)
            break;
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

Value FloorMapLayer::get_tile_prop(int32_t gid, const std::string& key)
{
    Value properties = _tiled_map->getPropertiesForGID(gid & kTMXFlippedMask);
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

void FloorMapLayer::pick_up_item_impl(const Floor::npc_t& npc, const cocos2d::Vec2& target_pos, const std::function<void(int32_t)>& callback)
{
    auto npc_layer = _tiled_map->getLayer("npc");
    auto item = npc_layer->getTileAt(Vec2(npc.pos.x, 11 - npc.pos.y));
    npc_layer->setupTileSprite(item, Vec2(npc.pos.x, 11 - npc.pos.y), npc.gid);
    auto start_pos = this->convertToNodeSpace(item->getParent()->convertToWorldSpace(item->getPosition()));
    item->retain();
    item->removeFromParentAndCleanup(false);
    item->setPosition(start_pos);
    this->addChild(item);
    item->release();

    // TODO.. cocos2d-x tiled bug. 如果一个Layer只剩下一个tile，getTileAt内部设置gid为0不起作用，目前找不到解决办法
    npc_layer->setTileGID(999, Vec2(npc.pos.x, 11 - npc.pos.y));

    auto duration = item->getPosition().distance(target_pos) / 1000.0f;
    // 此处不再延迟生效，而是数据立即更新，界面延迟更新，否则会出现交互后再次判断交互目标，出现未被改变的bug
    callback((int32_t)(duration * 1000));
    item->runAction(Sequence::create(
        Spawn::createWithTwoActions(MoveTo::create(duration, target_pos), ScaleTo::create(duration, 0.6f)),
        CCRemoveSelf::create(), nullptr));
}

void FloorMapLayer::pick_up_item(const Floor::npc_t& npc)
{
    std::function<void(int32_t)> callback;
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
                callback = std::bind(&PlayerDelegate::add_key, 1, 1, 1, std::placeholders::_1);
            }
            else if (1 == color) // 红钥匙
            {
                type = WarriorInfoPanel::key_red;
                callback = std::bind(&PlayerDelegate::add_key, 1, 0, 0, std::placeholders::_1);
            }
            else if (2 == color) // 蓝钥匙
            {
                type = WarriorInfoPanel::key_blue;
                callback = std::bind(&PlayerDelegate::add_key, 0, 1, 0, std::placeholders::_1);
            }
            else if (3 == color) // 黄钥匙
            {
                type = WarriorInfoPanel::key_yellow;
                callback = std::bind(&PlayerDelegate::add_key, 0, 0, 1, std::placeholders::_1);
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
                callback = std::bind(&PlayerDelegate::add_attack, value, std::placeholders::_1);
            }
            else if (2 == color) // 蓝宝石，防御
            {
                type = WarriorInfoPanel::defend;
                callback = std::bind(&PlayerDelegate::add_defence, value, std::placeholders::_1);
            }
        }
        break;
    case 5: // 血瓶
        {
            auto value = get_tile_prop(npc.gid, "value").asInt();
            type = WarriorInfoPanel::hp;
            callback = std::bind(&PlayerDelegate::add_hp, value, std::placeholders::_1);
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
        pick_up_item_impl(npc, target_pos, [&, callback](int32_t delay) {
            if (callback)
                callback(delay);
            Floor::GetInstance()->remove_npc(_floor, npc);
        });
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
        auto npc = Floor::GetInstance()->get_floor_info(_floor).find_npc_by_pos(start_pt);
        if (nullptr != npc)
            pick_up_item(*npc);
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
        auto child = dynamic_cast<Node*>(road_children.at(road_children.size() + 1 - _paths.size()));
        if (nullptr != child)
        {
            child->runAction(FadeOut::create(0.2f));
        }

        bool walk_pause = false;
        auto npc = Floor::GetInstance()->get_floor_info(_floor).find_npc_by_pos(end_pt);
        if (nullptr != npc)
            walk_pause = interact_item(*npc);
        
        if (!walk_pause)
        {
            _warrior->move_to(end_pos, CC_CALLBACK_0(FloorMapLayer::step, this));
            _paths.erase(_paths.begin());
        }
    }
    else // 走到寻路最后一个节点，清理工作
    {
        stop_and_clear();
    }
}

bool FloorMapLayer::interact_item(const Floor::npc_t& npc)
{
    bool walk_pause = false;
    auto style = get_tile_prop(npc.gid, "style").asInt();
    switch (style)
    {
    case 1: // 怪物
    {
        walk_pause = true;
        auto end_pos = Vec2((npc.pos.x + 0.5f) * 75.0f - 50.0f, (npc.pos.y + 0.5f) * 75.0f - 50.0f);
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
            walk_pause = true;
            auto end_pos = Vec2((npc.pos.x + 0.5f) * 75.0f - 50.0f, (npc.pos.y + 0.5f) * 75.0f - 50.0f);
            _warrior->turn_to(end_pos);

            auto color = get_tile_prop(npc.gid, "color").asInt();
            const auto& player = Player::GetInstance()->get_player_info();
            if (1 == color && player.key_red < 1 || 2 == color && player.key_blue < 1 || 3 == color && player.key_yellow < 1)
            {
                auto dict = FileUtils::getInstance()->getValueMapFromFile("chinese.xml");
                auto text = dict["buyKeyTip"].asString();
                PromptDialog::show(text);
                stop_and_clear();
                break;
            }

            auto dict = FileUtils::getInstance()->getValueMapFromFile("chinese.xml");
            auto prefix = dict["open_msg1"].asString();
            auto sufix = dict["open_msg2"].asString();
            auto textkey = 1 == color ? "rKey" : (2 == color ? "bKey" : "yKey");
            auto info = prefix + dict[textkey].asString() + sufix;
            auto dialog = OKCancelDialog::create("", info);
            dialog->setCallback(std::bind(&FloorMapLayer::confirm_open, this, std::placeholders::_1, npc));
            ModalDialogManager::GetInstance()->pushDialog(dialog);
        }
        break;
    case 6: // 楼梯
    {
        walk_pause = false; // 无需停下来，直接切换楼层，切换场景的持续时间的一半让勇士正好走入下一个楼梯格子
        auto up = get_tile_prop(npc.gid, "type").asInt() == 2;
        int32_t next_floor = _floor + (up ? 1 : -1);
        next_floor = next_floor == 0 ? 8 : (next_floor == 9 ? 1 : next_floor);
        auto next_scene = PromptLayer::scene(next_floor, up);
        auto transition = TransitionFade::create(0.5f, next_scene);
        Director::getInstance()->replaceScene(transition);
    }
    break;
    default:
        break;
    }

    return walk_pause;
}

void FloorMapLayer::confirm_open(OKCancelDialog::RETURN_TYPE type, const Floor::npc_t& npc)
{
    if (type == OKCancelDialog::CANCEL)
    {
        stop_and_clear();
    }
    else
    {
        _warrior->stand_auto();
        _warrior->set_lock(true);   // 设置等待开门的过程中不可重新寻路，以免出现计算混乱
        auto npc_layer = _tiled_map->getLayer("npc");
        auto door = npc_layer->getTileAt(Vec2(npc.pos.x, 11 - npc.pos.y));
        npc_layer->setupTileSprite(door, Vec2(npc.pos.x, 11 - npc.pos.y), npc.gid);
        const float OPEN_DURATION = 0.5f;
        door->runAction(CCSequence::create(FadeOut::create(OPEN_DURATION), RemoveSelf::create(),
            CCCallFunc::create(std::bind(&FloorMapLayer::confirm_open_impl, this, npc)), nullptr));

        // 如果是竖门需要将其上方的附加节点一并去除
        auto uplink = get_tile_prop(npc.gid, "uplink").asInt();
        if (1 == uplink)
        {
            auto npc_up = Floor::GetInstance()->get_floor_info(_floor).find_npc_by_pos(Floor::position_t(npc.pos.x, npc.pos.y + 1));
            if (nullptr != npc_up)
            {
                auto door_up = npc_layer->getTileAt(Vec2(npc_up->pos.x, 11 - npc_up->pos.y));
                npc_layer->setupTileSprite(door_up, Vec2(npc_up->pos.x, 11 - npc_up->pos.y), npc_up->gid);
                door_up->runAction(CCSequence::create(FadeOut::create(OPEN_DURATION), RemoveSelf::create(), nullptr));
            }
        }
    }
}

void FloorMapLayer::confirm_open_impl(const Floor::npc_t& npc)
{
    auto color = get_tile_prop(npc.gid, "color").asInt();
    PlayerDelegate::add_key(1 == color ? -1 : 0, 2 == color ? -1 : 0, 3 == color ? -1 : 0);
    Floor::GetInstance()->remove_npc(_floor, npc);

    _warrior->set_lock(false);
    auto end_pos = Vec2((npc.pos.x + 0.5f) * 75.0f - 50.0f, (npc.pos.y + 0.5f) * 75.0f - 50.0f);
    _warrior->move_to(end_pos, CC_CALLBACK_0(FloorMapLayer::step, this));
    _paths.erase(_paths.begin());
}

void FloorMapLayer::confirm_attack(OKCancelDialog::RETURN_TYPE type, const Floor::npc_t& npc)
{
    if (type == OKCancelDialog::CANCEL)
    {
        stop_and_clear();
    }
    else
    {
        static int attack_count = 3;
        attack_count = 3;
        _warrior->fight_auto(3, std::bind(&FloorMapLayer::confirm_attack_impl, this, npc));
    }
}

void FloorMapLayer::confirm_attack_impl(const Floor::npc_t& npc)
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
        stop_and_clear();
        return;
    }

    auto blood = (life + attack_damage - 1) / attack_damage * damage;

    auto target_pos = this->convertToNodeSpace(_info_panel->get_node_position_in_world(WarriorInfoPanel::hun));
    target_pos -= Vec2(26.0f, 0);
    pick_up_item_impl(npc, target_pos, [&, blood, gold, hun](int32_t delay) {
        PlayerDelegate::add_hp(-blood, delay);
        PlayerDelegate::add_gold_hun(gold, hun, delay);
        Floor::GetInstance()->remove_npc(_floor, npc);
    });

    auto end_pos = Vec2((npc.pos.x + 0.5f) * 75.0f - 50.0f, (npc.pos.y + 0.5f) * 75.0f - 50.0f);
    _warrior->move_to(end_pos, CC_CALLBACK_0(FloorMapLayer::step, this));
    _paths.erase(_paths.begin());
}

void FloorMapLayer::stop_and_clear()
{
    _road_node->removeAllChildren();
    _warrior->stand_auto();
    _arrow_node->setVisible(false);
    _paths.clear();
}

//////////////////////////////////////////////////////////////////////////
cocos2d::Scene * PromptLayer::scene(int floor, bool up)
{
    PromptLayer* prompt_layer = new (std::nothrow) PromptLayer();
    if (prompt_layer && prompt_layer->init(floor, up))
    {
        prompt_layer->autorelease();
        auto ret_scene = Scene::create();
        ret_scene->addChild(prompt_layer);
        return ret_scene;
    }
    else
    {
        CC_SAFE_DELETE(prompt_layer);
        return nullptr;
    }
}

bool PromptLayer::init(int floor, bool up)
{
    _floor = floor;
    _up = up;
    auto size = Director::getInstance()->getWinSize();
    auto label = Label::createWithSystemFont("Floor" + String::createWithFormat("%d", floor)->_string, "", 52);
    label->setPosition(Vec2(size.width / 2, size.height / 2));
    addChild(label);
    return true;
}

void PromptLayer::onEnterTransitionDidFinish()
{
    runAction(Sequence::createWithTwoActions(DelayTime::create(0.5f), CCCallFunc::create([&]() {
        auto next_scene = FloorMapLayer::scene(_floor, _up);
        auto transition = TransitionFade::create(0.5f, next_scene);
        Director::getInstance()->replaceScene(transition);
    })));
}
