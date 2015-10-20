#pragma once

#include "cocos2d.h"
#include "AStar.h"

#include "ModalDialog.h"

enum OKCancelDialog::RETURN_TYPE;
namespace cocos2d { class LabelAtlas; }
class WarriorNode;
class WarriorInfoPanel;
class FloorMapLayer : public cocos2d::Node
{
public:
    static cocos2d::Scene* scene();
    virtual bool init() override;

    CREATE_FUNC(FloorMapLayer);

public:
    bool onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *e);
    void onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *e);

private:
    struct npc_t;
    void confirm_attack(OKCancelDialog::RETURN_TYPE type, const npc_t& npc);
    void confirm_attack_impl(const npc_t& npc);
    void confirm_open(OKCancelDialog::RETURN_TYPE type, const npc_t& npc);
    void confirm_open_impl(const npc_t& npc);
    cocos2d::Value get_tile_prop(int32_t gid, const string& key);
    void step();
    void pick_up_item_impl(const npc_t& npc, const cocos2d::Vec2& target_pos, const std::function<void()>& callback);
    void pick_up_item(const npc_t& npc);
    bool interact_item(const npc_t& npc);   // 与npc交互，返回值为是否停下来交互，如开门，打斗，对话框提示等
    void stop_and_clear();

private:
    cocos2d::experimental::TMXTiledMap* _tiled_map;
    WarriorNode* _warrior;
    WarriorInfoPanel* _info_panel;
    Node* _arrow_node;
    Node* _road_node;
    vector<node_t> _paths;
    struct npc_t
    {
        int32_t x;
        int32_t y;
        int32_t gid;

        npc_t(int32_t _x, int32_t _y, int32_t _gid)
            : x(_x), y(_y), gid(_gid)
        {}

        bool operator == (const npc_t& npc) const { return x == npc.x && y == npc.y; }
    };
    vector<npc_t> _npcs;
};