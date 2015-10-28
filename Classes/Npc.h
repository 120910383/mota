#pragma once

#include "Singleton.h"

class Npc : public Singleton<Npc>
{
public:
    Npc();
    ~Npc();

public:
    enum Category {
        category_unknown = 0,   // 未定义
        monster = 1,            // 怪物
        key = 2,                // 钥匙
        potion = 3,             // 药水
        door = 4,               // 普通门
        stair = 5,              // 楼梯
        jewel = 6,              // 宝石
        gallery = 7,            // 神秘通道
    };

    // 钥匙类型
    enum KeyColor {
        key_color_unknown = 0,  // 未定义
        red = 1,                // 红
        blue = 2,               // 蓝
        yellow = 3,             // 黄
        all = 4,                // 一串
    };

    // 宝石加强类型
    enum Enhance {
        enhance_unknown = 0,    // 未定义
        attack = 1,             // 加攻击(红色)
        defence = 2,            // 加防御(蓝色)
    };

    // 楼梯类型
    enum Direction {
        direction_unknown = 0,  // 未定义
        up = 1,                 // 上楼梯
        down = 2,               // 下楼梯
    };

    // NPC基础结构
    struct npc_info_t
    {
        int32_t id;
        Category type;
        std::string name;       // 名称key
        std::string description;    // 描述key
    };

    // 怪物
    struct monster_t : public npc_info_t
    {
        int32_t attack;
        int32_t defence;
        int32_t life;
        int32_t win_gold;
        int32_t win_hun;
    };

    // 钥匙
    struct key_t : public npc_info_t
    {
        KeyColor color;
    };

    // 药水
    struct potion_t : public npc_info_t
    {
        int32_t value;
    };

    // 普通门
    struct door_t : public npc_info_t
    {
        KeyColor color; // all无意义
    };

    // 楼梯
    struct stair_t : public npc_info_t
    {
        bool slate; // 楼梯还是楼梯前的石板，寻路用，上下楼梯必须从石板上经过
        Direction direct;
    };

    // 宝石
    struct jewel_t : public npc_info_t
    {
        Enhance type;
        int32_t value;
    };

public:
    const std::map<int32_t, npc_info_t>& get_npc_infos() const { return _npc_infos; }

protected:
    std::map<int32_t, npc_info_t> _npc_infos;   // id => info
};