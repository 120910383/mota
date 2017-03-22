/*********************************************
Author: Zhang Hao
GitHub: https://github.com/120910383/mota
*********************************************/

#include "a_star.h"

using namespace std;

#define MOVE_COST 1

a_star::a_star(int32_t w, int32_t h, const pos_t& start, const pos_t& end, const vector<pos_t>& blocks)
    : _width(w), _height(h), _start(start), _end(end), _blocks(blocks)
{
}

a_star::~a_star()
{
}

const vector<pos_t> a_star::get_path()
{
    _path.clear();
    _opens.clear();
    _closes.clear();
    _finished = false;

    if (_start == _end)
        return _path;

    // first put start point into open list as loop start seed.
    _opens.push_back(calculate_fgh(_start));

    while (!_finished)
    {
        if (_opens.empty())
            break;

        // find a min f pos, if multi, select latest one.
        auto it = min_element(_opens.begin(), _opens.end(), [](auto l, auto r) { return l.f < r.f; });
        auto rit = find_if(_opens.rbegin(), _opens.rend(), [it](auto e) { return e.f == it->f; });

        auto node = *rit;
        _closes.push_back(*rit);
        _opens.erase(next(rit).base());

        open_arround(node, 1, 0);
        open_arround(node, 0, 1);
        open_arround(node, -1, 0);
        open_arround(node, 0, -1);
    }

    trim_path(true);

    return _path;
}

void a_star::open_arround(const node_t& node, int32_t offset_x, int32_t offset_y)
{
    auto around = node.pos.offset(offset_x, offset_y);
    if (around.x < 0 || around.x >= _width || around.y < 0 || around.y >= _height)
        return;

    auto pred = [around](const node_t& e) { return e.pos == around; };
    if (find_if(_blocks.begin(), _blocks.end(), pred) != _blocks.end())
        return;

    if (find_if(_closes.begin(), _closes.end(), pred) != _closes.end())
        return;

    if (find_if(_opens.begin(), _opens.end(), pred) != _opens.end())
        return;

    auto new_node = calculate_fgh(around);
    new_node.parent = node.pos;

    // judge if finished
    if (_end == around)
    {
        _closes.push_back(new_node);
        _finished = true;
    }
    else
    {
        _opens.push_back(new_node);
    }
}

void a_star::trim_path(bool turn)
{
    if (_closes.empty())
        return;

    auto node = _closes.back();
    _path.push_back(node.pos); // first

    while (!(node.pos == _start))
    {
        auto last_node = node;
        node = *get_close_node(node.parent);
        // if turn is true, last node and next node is not at a line, then exclude this point.
        if (turn && get_close_node(node.parent) != nullptr && !get_close_node(node.parent)->pos.slant(last_node.pos))
            continue;
        _path.push_back(node.pos);
    }

    reverse(_path.begin(), _path.end());
}

a_star::node_t a_star::calculate_fgh(const pos_t& pos)
{
    node_t node(pos);
    node.g = get_distance(pos, _start);
    node.h = get_distance(pos, _end);
    node.f = node.g + node.h;
    return node;
}

int32_t a_star::get_distance(const pos_t & p1, const pos_t & p2)
{
    // Manhattan Distance
    int32_t delta_x = abs(p1.x - p2.x);
    int32_t delta_y = abs(p1.y - p2.y);
    return (delta_x + delta_y) * MOVE_COST;
}

const a_star::node_t* a_star::get_close_node(const pos_t& pos)
{
    auto iter = find_if(_closes.begin(), _closes.end(), [&pos](auto elem) { return elem.pos == pos; });
    return iter != _closes.end() ? &(*iter) : nullptr;
}