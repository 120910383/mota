#pragma once

#include "Singleton.h"

class Floor : public Singleton<Floor>
{
public:
    Floor();
    ~Floor();

public:
    struct position_t
    {
        int32_t x;
        int32_t y;

        position_t() {}
        position_t(int32_t _x, int32_t _y)
            : x(_x), y(_y)
        {}

        bool operator == (const position_t& pos) const
        {
            return x == pos.x && y == pos.y;
        }

        bool operator != (const position_t& pos) const
        {
            return x != pos.x || y != pos.y;
        }
    };

    struct npc_t
    {
        position_t pos;
        int32_t gid;

        npc_t() {}
        npc_t(int32_t _x, int32_t _y, int32_t _gid)
            : pos(_x, _y), gid(_gid)
        {}
    };

    struct floor_t
    {

    };
};