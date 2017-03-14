#include "utils.h"
/*********************************************
Author: Zhang Hao
GitHub: https://github.com/120910383/mota
*********************************************/

using namespace std;

string utils::replace_str(const string & origin, const string & old, const string & value)
{
    auto result = origin;
    while (true)
    {
        auto pos = result.find(old);
        if (pos != string::npos)
            result.replace(pos, old.length(), value);
        else
            break;
    }
    return result;
}
