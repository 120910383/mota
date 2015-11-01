#pragma once

#include "Singleton.h"
#include "Npc.h"

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
        int32_t id;
        int32_t gid;
        int32_t floor;
        position_t pos;
        bool gone;
        Npc::npc_info_t info;

        npc_t() {}
        npc_t(int32_t _x, int32_t _y, int32_t _gid)
            : pos(_x, _y), gid(_gid)
        {}

        bool operator == (const npc_t& npc) const
        {
            return pos == npc.pos && gid == npc.gid;
        }
    };

    struct floor_t
    {
        int32_t level;
        std::vector<npc_t> npcs;

        floor_t() : level(-1) {}
        bool is_valid() const { return level > 0; }
        const npc_t* find_npc_by_pos(const position_t& pos) const;
    };

public:
    const floor_t& get_floor_info(int32_t level) const;
    void auto_init_floor(int32_t level, const std::vector<npc_t>& npcs);
    void remove_npc(int32_t level, const npc_t& npc);
    void save();

protected:
    void load();

protected:
    std::map<int32_t, floor_t> _floors;
};