/*********************************************
Author: Zhang Hao
GitHub: https://github.com/120910383/mota
Description: Part of xml config structure
map<string, string> wall_infos = {
{"1B", "zaw0"},
{"1C", "zaw1"},
{"1D", "zaw2"},
{"1E", "zaw3"},
{"1F", "zaw4"},
{"1G", "zaw5"},
{"1H", "zaw6"},
{"1I", "zaw7"},
{"1J", "zaw8"},
};

map<string, string> floor_infos = {
{"1B", "bg0"},
{"1C", "bg1"},
{"1D", "bg2"},
{"1E", "bg3"},
};

map<string, string> npc_infos = {
{"1S", "rw17"},
{"2E", "rw33"},
{"1R", "rw16"},
{"1P", "rw14"},
{"2G", "rw35"},
{";H", "rw6"},
{"2F", "rw34"},
{"1Q", "rw15"},
{"1T", "rw18"},
{"1G", "rw5"},
{"1U", "rw19"},
{"&lt;G", "rw35"},
{"2B", "rw30"},
{"1C", "rw1"},
{";I", "rw7"},
{"1O", "rw13"},
{";B", "rw0"},
};

*********************************************/

#include "maps.h"
#include "utils.h"
#include "a_star.h"

#include "deprecated/CCDictionary.h"

#include <algorithm>

USING_NS_CC;
using namespace std;

#define POS_X(i) ((i) / 2 % 10)
#define POS_Y(i) (11 - (i) / 2 / 10)
#define FLOOR_TILE_START_ID 1000
#define BLOCK_TILE_START_ID 2000
#define NPC_TILE_START_ID 3000
#define TILE_SIZE 75.0f
#define WIDTH 10
#define HEIGHT 12

map_t::map_t()
{
}

map_t::~map_t()
{
}

bool map_t::load()
{
    auto plist = __Dictionary::createWithContentsOfFile("data0.xml");

    DictElement* elem;
    CCDICT_FOREACH(plist, elem)
    {
        if (elem == nullptr || elem->getObject() == nullptr)
            return false;

        auto str = dynamic_cast<__String*>(elem->getObject());
        if (str == nullptr || str->getCString() == nullptr)
            return false;

        const int32_t floor = atoi(elem->getStrKey());

        // use origin game config file structure, i do not have the data editor.
        const string config(str->getCString());

        string block_info = config.substr(0, 220) + "1B1B1C1B1B1C1B1C1B1B";
        parse_block(floor, block_info);

        string floor_info = config.substr(220, 220) + "1B1B1B1B1B1B1B1B1B1B";
        parse_floor(floor, floor_info);

        string trim = config.substr(440);
        trim = utils::replace_str(trim, "&lt;", "<");
        trim = utils::replace_str(trim, "&gt;", ">");
        string npc_info = trim;
        parse_npc(floor, npc_info);
    }

    return true;
}

const floor_t* map_t::get_floor_info(int32_t index)
{
    if (_floors.find(index) != _floors.end())
        return &_floors[index];
    else
        return nullptr;
}

void map_t::parse_block(int32_t index, const std::string& config)
{
    auto& floor = _floors[index];
    floor.number = index;
    floor.blocks.clear();
    const char* cstr = config.c_str();
    int32_t id = BLOCK_TILE_START_ID;
    for (int32_t i = 0; i < 240; i += 2)
    {
        if (cstr[i] == '1' || cstr[i] == ';')
        {
            auto res = "block/zaw" + to_string(cstr[i + 1] - 66) + ".png";
            auto pos_x = static_cast<uint8_t>(POS_X(i));
            auto pos_y = static_cast<uint8_t>(POS_Y(i));
            bool flip = cstr[i] == ';';
            tile_t tile = { id, pos_x, pos_y, res, flip };
            floor.blocks.push_back(tile);
            ++id;
        }
    }
}

void map_t::parse_floor(int32_t index, const std::string& config)
{
    auto& floor = _floors[index];
    floor.number = index;
    floor.floors.clear();
    const char* cstr = config.c_str();
    int32_t id = FLOOR_TILE_START_ID;
    for (int32_t i = 0; i < 240; i += 2)
    {
        if (cstr[i] == '1')
        {
            auto res = "floor/bg" + to_string(cstr[i + 1] - 66) + ".png";
            auto pos_x = static_cast<uint8_t>(POS_X(i));
            auto pos_y = static_cast<uint8_t>(POS_Y(i));
            tile_t tile = { id, pos_x, pos_y, res, false };
            floor.floors.push_back(move(tile));
            ++id;
        }
    }
}

void map_t::parse_npc(int32_t index, const std::string& config)
{
    auto& floor = _floors[index];
    floor.number = index;
    floor.npcs.clear();
    const char* cstr = config.c_str();
    int32_t id = NPC_TILE_START_ID;
    for (int32_t i = 0; i < 220; i += 2)
    {
        if (cstr[i] == '1' || cstr[i] == ';')
        {
            auto res = "npc/rw" + to_string(cstr[i + 1] - 66) + ".png";
            auto pos_x = static_cast<uint8_t>(POS_X(i));
            auto pos_y = static_cast<uint8_t>(POS_Y(i));
            bool flip = cstr[i] == ';';
            tile_t tile = { id, pos_x, pos_y, res, flip };

            if (cstr[i + 1] == 'B')
                floor.stair_down = move(tile);
            else if (cstr[i + 1] == 'I')
                floor.stair_up = move(tile);
            else
                floor.npcs.push_back(move(tile));
            ++id;
        }
        else if (cstr[i] == '2' || cstr[i] == '<' || cstr[i] == 'F')
        {
            auto res = "npc/rw" + to_string(cstr[i + 1] - 36) + ".png";
            auto pos_x = static_cast<uint8_t>(POS_X(i));
            auto pos_y = static_cast<uint8_t>(POS_Y(i));
            bool flip = cstr[i] == '<';
            tile_t tile = { id, pos_x, pos_y, res, flip };
            floor.npcs.push_back(move(tile));
            ++id;
        }
        else if (cstr[i] == '5' || cstr[i] == '?')
        {
            auto res = "npc/rw" + to_string(cstr[i + 1] + 24) + ".png";
            auto pos_x = static_cast<uint8_t>(POS_X(i));
            auto pos_y = static_cast<uint8_t>(POS_Y(i));
            bool flip = cstr[i] == '?';
            tile_t tile = { id, pos_x, pos_y, res, flip };
            floor.npcs.push_back(move(tile));
            ++id;
        }
    }
}

pos_t::pos_t(float _x, float _y)
{
    x = (int32_t)(_x / TILE_SIZE);
    y = (int32_t)(_y / TILE_SIZE);
}

pos_t pos_t::offset(int32_t _x, int32_t _y) const
{
    return pos_t(x + _x, y + _y);
}

cocos2d::Vec2 pos_t::center_pos() const
{
    return (Point(x, y) + Point(0.5f, 0.5f)) * TILE_SIZE;
}

cocos2d::Vec2 pos_t::origin_pos() const
{
    return Point(x, y) * TILE_SIZE;
}

bool floor_t::is_block(const pos_t & pos) const
{
    auto it = find_if(blocks.begin(), blocks.end(),
        [&pos](const tile_t& tile){ return tile.pos == pos; });
    
    return it != blocks.end();
}

pos_t floor_t::get_init_pos(bool up) const
{
    auto pos = up ? stair_down.pos : stair_up.pos;
    return pos.offset(stair_down.flip ? -1 : 1, 0);
}

vector<pos_t> floor_t::get_path(const pos_t & start, const pos_t & end) const
{
    vector<pos_t> blks;
    for (auto elem : blocks)
        blks.push_back(elem.pos);
    a_star as(WIDTH, HEIGHT, start, end, blks);
    return as.get_path();
}
