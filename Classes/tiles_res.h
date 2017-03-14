/*********************************************
Author: Zhang Hao
GitHub: https://github.com/120910383/mota
*********************************************/

#pragma once

#include "Singleton.h"
#include <cstdint>
#include <vector>
#include <map>

// tile
class tiles_res : public Singleton<tiles_res>
{
public:
    tiles_res();
    virtual ~tiles_res();

public:
    struct tile_t
    {
        uint8_t x;
        uint8_t y;
        const char* res;
    };

    struct floor_t
    {
        std::vector<tile_t> blocks;
        std::vector<tile_t> floors;
        std::vector<tile_t> npcs;
        tile_t stair_down;
        tile_t stair_up;
    };

public:
    bool load();

protected:
    void parse_block(int32_t index, const std::string& config);
    void parse_floor(int32_t index, const std::string& config);
    void parse_npc(int32_t index, const std::string& config);

protected:
    std::map<uint32_t, floor_t> _floors;
};