/*********************************************
Author: Zhang Hao
GitHub: https://github.com/120910383/mota
*********************************************/

#pragma once

#include "Singleton.h"
#include <cstdint>
#include <vector>
#include <map>

#include "math/Vec2.h"

struct pos_t
{
    int32_t x;
    int32_t y;

    pos_t() {}
    pos_t(int32_t _x, int32_t _y) : x(_x), y(_y) {}
    pos_t(float _x, float _y);
    pos_t(const cocos2d::Vec2& pos) : pos_t(pos.x, pos.y) {}
    inline bool operator == (const pos_t& pos) const { return pos.x == x && pos.y == y; }
    inline bool slant(const pos_t& pos) const { return pos.x != x && pos.y != y; }
    pos_t offset(int32_t _x, int32_t _y) const;
    cocos2d::Vec2 center_pos() const;
    cocos2d::Vec2 origin_pos() const;
};

struct tile_t
{
    int32_t id;
    pos_t pos;
    std::string res;
    bool flip;

    tile_t() {}
    tile_t(int32_t _id, int32_t _x, int32_t _y, const std::string& _res, bool _flip)
        : id(_id), pos(_x, _y), res(_res), flip(_flip) {}
};

struct floor_t
{
    int32_t number;
    std::vector<tile_t> blocks;
    std::vector<tile_t> floors;
    std::vector<tile_t> npcs;
    tile_t stair_down;
    tile_t stair_up;

    floor_t() {}
    bool is_block(const pos_t& pos) const;
    pos_t get_init_pos(bool up) const;
    std::vector<pos_t> get_path(const pos_t& start, const pos_t& end) const;
};

class map_t : public Singleton<map_t>
{
public:
    map_t();
    virtual ~map_t();

public:
    bool load();
    const floor_t* get_floor_info(int32_t index);

protected:
    void parse_block(int32_t index, const std::string& config);
    void parse_floor(int32_t index, const std::string& config);
    void parse_npc(int32_t index, const std::string& config);

protected:
    std::map<int32_t, floor_t> _floors;
};