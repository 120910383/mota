/*********************************************
Author: Zhang Hao
GitHub: https://github.com/120910383/mota
*********************************************/

#include "tiles_res.h"
#include "utils.h"

#include "deprecated/CCDictionary.h"

USING_NS_CC;
using namespace std;

tiles_res::tiles_res()
{
}

tiles_res::~tiles_res()
{
}

bool tiles_res::load()
{
    auto plist = __Dictionary::createWithContentsOfFile("data/data0.xml");

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

void tiles_res::parse_block(int32_t index, const std::string& config)
{

}

void tiles_res::parse_floor(int32_t index, const std::string& config)
{

}

void tiles_res::parse_npc(int32_t index, const std::string& config)
{

}
