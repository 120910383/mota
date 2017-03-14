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

#include "tiles_res.h"
#include "utils.h"

#include "deprecated/CCDictionary.h"

USING_NS_CC;
using namespace std;

#define POS_X(i) ((i) / 2 % 10)
#define POS_Y(i) (11 - (i) / 2 / 10)

tiles_res::tiles_res()
{
}

tiles_res::~tiles_res()
{
}

bool tiles_res::load()
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

    auto tt = _floors[10].floors.front().res;
    return true;
}

void tiles_res::parse_block(int32_t index, const std::string& config)
{
    auto& floor = _floors[index];
    floor.blocks.clear();
    const char* cstr = config.c_str();
    for (int32_t i = 0; i < 240; i += 2)
    {
        if (cstr[i] == '1' || cstr[i] == ';')
        {
            auto res = "block/zaw" + to_string(cstr[i + 1] - 66) + ".png";
            auto pos_x = static_cast<uint8_t>(POS_X(i));
            auto pos_y = static_cast<uint8_t>(POS_Y(i));
            bool flip = cstr[i] == ';';
            tile_t tile = { pos_x, pos_y, res, flip };
            floor.blocks.push_back(tile);
        }
    }
}

void tiles_res::parse_floor(int32_t index, const std::string& config)
{
    auto& floor = _floors[index];
    floor.floors.clear();
    const char* cstr = config.c_str();
    for (int32_t i = 0; i < 240; i += 2)
    {
        if (cstr[i] == '1')
        {
            auto res = "floor/bg" + to_string(cstr[i + 1] - 66) + ".png";
            auto pos_x = static_cast<uint8_t>(POS_X(i));
            auto pos_y = static_cast<uint8_t>(POS_Y(i));
            tile_t tile = { pos_x, pos_y, res, false };
            floor.floors.push_back(move(tile));
        }
    }
}

void tiles_res::parse_npc(int32_t index, const std::string& config)
{
    auto& floor = _floors[index];
    floor.npcs.clear();
    const char* cstr = config.c_str();
    for (int32_t i = 0; i < 220; i += 2)
    {
        if (cstr[i] == '1' || cstr[i] == ';')
        {
            auto res = "npc/rw" + to_string(cstr[i + 1] - 66) + ".png";
            auto pos_x = static_cast<uint8_t>(POS_X(i));
            auto pos_y = static_cast<uint8_t>(POS_Y(i));
            bool flip = cstr[i] == ';';
            tile_t tile = { pos_x, pos_y, res, flip };

            if (cstr[i + 1] == 'B')
                floor.stair_down = move(tile);
            else if (cstr[i + 1] == 'I')
                floor.stair_up = move(tile);
            else
                floor.npcs.push_back(move(tile));
        }
        else if (cstr[i] == '2' || cstr[i] == '<' || cstr[i] == 'F')
        {
            auto res = "npc/rw" + to_string(cstr[i + 1] - 36) + ".png";
            auto pos_x = static_cast<uint8_t>(POS_X(i));
            auto pos_y = static_cast<uint8_t>(POS_Y(i));
            bool flip = cstr[i] == '<';
            tile_t tile = { pos_x, pos_y, res, flip };
            floor.npcs.push_back(move(tile));
        }
        else if (cstr[i] == '5' || cstr[i] == '?')
        {
            auto res = "npc/rw" + to_string(cstr[i + 1] + 24) + ".png";
            auto pos_x = static_cast<uint8_t>(POS_X(i));
            auto pos_y = static_cast<uint8_t>(POS_Y(i));
            bool flip = cstr[i] == '?';
            tile_t tile = { pos_x, pos_y, res, flip };
            floor.npcs.push_back(move(tile));
        }
    }
}
