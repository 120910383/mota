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
        int32_t id;
        uint8_t x;
        uint8_t y;
        std::string res;
        bool flip;

        inline bool operator == (const tile_t& tile) const
        {
            return tile.x == x && tile.y == y;
        }
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
    const floor_t* get_floor_info(int32_t index);

protected:
    void parse_block(int32_t index, const std::string& config);
    void parse_floor(int32_t index, const std::string& config);
    void parse_npc(int32_t index, const std::string& config);

protected:
    std::map<int32_t, floor_t> _floors;
};