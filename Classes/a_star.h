/*********************************************
Author: Zhang Hao
GitHub: https://github.com/120910383/mota
*********************************************/

#pragma once

#include "maps.h"

class a_star
{
public:
    struct node_t
    {
        pos_t pos;
        pos_t parent;
        int32_t f, g, h;

        node_t() : parent(nullptr) {}
        node_t(const pos_t& _pos) : pos(_pos) {}
    };

public:
    a_star(int32_t w, int32_t h, const pos_t& start, const pos_t& end, const std::vector<pos_t>& blocks);
    ~a_star();

    const std::vector<pos_t> get_path();

protected:
    void open_arround(const node_t& node, int32_t offset_x, int32_t offset_y);
    void trim_path(bool turn);
    node_t calculate_fgh(const pos_t& pos);
    static int32_t get_distance(const pos_t& p1, const pos_t& p2);
    const node_t* get_close_node(const pos_t& pos);

protected:
    int32_t _width;
    int32_t _height;
    pos_t _start;
    pos_t _end;
    std::vector<pos_t> _blocks;

    std::vector<node_t> _opens;
    std::vector<node_t> _closes;

    std::vector<pos_t> _path;
    bool _finished;
};