#pragma once

#include "cocos2d.h"
#include "AStar.h"

namespace spine { class SkeletonAnimation; }
class FloorMapLayer : public cocos2d::Layer
{
public:
    static cocos2d::Scene* scene();
    virtual bool init() override;

    CREATE_FUNC(FloorMapLayer);

public:
    virtual bool onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *e) override;
    virtual void onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *e) override;

private:
    void confirm_attack(Node* node);
    cocos2d::Value get_tile_prop(int32_t gid, const string& key);
    void step();

private:
    cocos2d::experimental::TMXTiledMap* _tiled_map;
    spine::SkeletonAnimation* _warrior;
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