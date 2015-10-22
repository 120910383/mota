#include "pre_header.h"

#include "Floor.h"

Floor::Floor()
{
}

Floor::~Floor()
{
}

const Floor::floor_t & Floor::get_floor_info(int32_t level) const
{
    static Floor::floor_t floor;
    auto iter = _floors.find(level);
    return iter != _floors.end() ? iter->second : floor;
}

void Floor::auto_init_floor(int32_t level, const std::vector<npc_t>& npcs)
{
    if (_floors.find(level) == _floors.end())
    {
        _floors[level].level = level;
        _floors[level].npcs = npcs;
    }
    else
    {
        return; // 只能初始化一次
    }
}

void Floor::remove_npc(int32_t level, const npc_t& npc)
{
    if (_floors.find(level) != _floors.end())
    {
        auto& npcs = _floors[level].npcs;
        auto iter = std::find(npcs.begin(), npcs.end(), npc);
        if (iter != npcs.end())
        {
            npcs.erase(iter);
        }
    }
}

const Floor::npc_t* Floor::floor_t::find_npc_by_pos(const position_t& pos) const
{
    auto iter = std::find_if(npcs.begin(), npcs.end(), [pos](const Floor::npc_t& elem) {
        return elem.pos == pos;
    });

    return iter != npcs.end() ? &(*iter) : nullptr;
}
