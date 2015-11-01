#include "pre_header.h"

#include "Floor.h"

Floor::Floor()
{
    load();
}

Floor::~Floor()
{
    save();
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

void Floor::save()
{
    cocos2d::ValueMap floors;
    for (const auto& floor : _floors)
    {
        cocos2d::ValueVector npcs;
        for (const auto& npc : floor.second.npcs)
        {
            cocos2d::ValueMap npc_info;
            npc_info["x"] = npc.pos.x;
            npc_info["y"] = npc.pos.y;
            npc_info["gid"] = npc.gid;
            npcs.push_back(cocos2d::Value(npc_info));
        }
        
        auto key = cocos2d::String::createWithFormat("%d", floor.first)->_string;
        floors[key] = npcs;
    }
    
    auto full_path = cocos2d::FileUtils::getInstance()->getWritablePath() + "floor.xml";
    if (!cocos2d::FileUtils::getInstance()->writeToFile(floors, full_path))
    {
        assert(false);
    }
}

void Floor::load()
{
    auto full_path = cocos2d::FileUtils::getInstance()->getWritablePath() + "floor.xml";
    auto floors_value_map = cocos2d::FileUtils::getInstance()->getValueMapFromFile(full_path);
    for (const auto& floor_value : floors_value_map)
    {
        floor_t floor;
        floor.level = cocos2d::String(floor_value.first).intValue();
        for (const auto& npc_value : floor_value.second.asValueVector())
        {
            npc_t npc;
            auto temp = npc_value.asValueMap();
            npc.pos.x = temp["x"].asInt();
            npc.pos.y = temp["y"].asInt();
            npc.gid = temp["gid"].asInt();
            floor.npcs.push_back(npc);
        }

        _floors[floor.level] = floor;
    }
}

const Floor::npc_t* Floor::floor_t::find_npc_by_pos(const position_t& pos) const
{
    auto iter = std::find_if(npcs.begin(), npcs.end(), [pos](const Floor::npc_t& elem) {
        return elem.pos == pos;
    });

    return iter != npcs.end() ? &(*iter) : nullptr;
}
