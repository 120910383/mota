#include "pre_header.h"

#include "AStar.h"

#define ARRAY_SIZE(a) (sizeof(a) / sizeof((a)[0]))

const int32_t MOVE_COST_PER_UNIT = 1;
const AStar::direction_t g_direct[] =
{
	{0, +1}, // up
	{0, -1}, // down
	{-1, 0}, // left
	{+1, 0}, // right
};

//////////////////////////////////////////////////////////////////////////
AStar::AStar(int32_t column, int row)
	: m_column(column)
	, m_row(row)
	, m_is_find(false)
{
}

AStar::~AStar()
{
}

void AStar::init(int32_t column, int32_t row)
{
	m_column = column;
	m_row = row;

	m_start_node = node_t();
	m_end_node = node_t();
	m_is_find = false;
	m_open.clear();
	m_close.clear();
	m_barrier.clear();
	m_path.clear();
}

void AStar::set_start_and_end(const node_t& start_node, const node_t& end_node)
{
	m_start_node = start_node;
	m_end_node = end_node;
}

void AStar::set_blocks(const std::vector<node_t>& vec_block)
{
	m_barrier.clear();
	m_barrier.insert(m_barrier.begin(), vec_block.begin(), vec_block.end());
}

void AStar::translate_path_to_turns(const std::vector<node_t>& path, std::vector<node_t>& turns)
{
	turns.clear();
	for (std::vector<node_t>::const_iterator iter = path.begin(); iter != path.end(); ++iter)
	{
		const node_t& node = *iter;
		if (iter == path.begin() || iter + 1 == path.end())
		{
			turns.push_back(node);
			continue;
		}

		const node_t& last_node = *(iter - 1);
		const node_t& next_node = *(iter + 1);
		if (node.y - last_node.y != next_node.y - node.y || node.x - last_node.x != next_node.x - node.x)
		{
			turns.push_back(node);
		}
	}
}

const std::vector<AStar::node_t>& AStar::get_path()
{
	m_path.clear();
	m_open.clear();
	m_close.clear();
	m_open.push_back(calculate_fgh(m_start_node));

	m_is_find = false;
    std::vector<node_t> min_vec;
	while (!m_is_find)
	{
		min_vec.clear();
		get_min_from_open(min_vec);
		if (min_vec.empty())
			break;

		m_close.insert(m_close.end(), min_vec.begin(), min_vec.end());

        for (const auto& node : min_vec)
            do_neighbors(node);

		if (m_is_find)
		{
			calculate_final_shortest_path();
			break;
		}
	}

	return m_path;
}

AStar::node_t AStar::get_next_turn()
{
    std::vector<node_t> turns;
	translate_path_to_turns(get_path(), turns);
	if (turns.size() > 1)
		return turns[1];
	else
		return node_t();
}

AStar::node_t AStar::calculate_fgh(const node_t& node)
{
	node_t calculate_node(node.x, node.y);

	calculate_node.g = get_distance(node, m_start_node);
	calculate_node.h = get_distance(node, m_end_node);
	calculate_node.f = calculate_node.g + calculate_node.h;

	return calculate_node;
}

int32_t AStar::get_distance(const node_t& rhs_node, const node_t& lhs_node)
{
	int32_t delta_x = abs(rhs_node.x - lhs_node.x);
	int32_t delta_y = abs(rhs_node.y - lhs_node.y);
	return (delta_x + delta_y) * MOVE_COST_PER_UNIT;
}

void AStar::get_min_from_open(std::vector<node_t>& min_vec)
{
	min_vec.clear();
	if (m_open.empty())
		return;

	// 查找open列表中最小f
	int32_t f_min = m_open.begin()->f;
    for (const auto& node : m_open)
        f_min = std::min<int32_t>(f_min, node.f);

	// 将open列表中最小f的节点返回，并从open列表中移除
	for (std::vector<node_t>::iterator iter = m_open.begin(); iter != m_open.end();)
	{
		if (iter->f == f_min)
		{
			min_vec.push_back(*iter);
			iter = m_open.erase(iter);
		}
		else
		{
			++iter;
		}
	}
}

void AStar::do_neighbors(const node_t& node)
{
	for (int32_t index = 0; index < ARRAY_SIZE(g_direct); ++index)
	{
		node_t node_neighbor = node;
		node_neighbor.x += g_direct[index].x;
		node_neighbor.y += g_direct[index].y;
		do_neighbor_node(g_direct[index], node_neighbor);
	}
}

void AStar::do_neighbor_node(const direction_t& direct, const node_t& node)
{
	if (node.x < 0 || node.x >= m_column
		|| node.y < 0 || node.y >= m_row)
		return;

	if (find_node_barrier(node.x, node.y))
		return;

	if (find_node_close(node.x, node.y))
		return;

	if (find_node_open(node.x, node.y))
		return;

	node_t new_neighbor = node;
	new_neighbor.parent = direct;
	new_neighbor.g += MOVE_COST_PER_UNIT;
	new_neighbor.h = get_distance(new_neighbor, m_end_node);
	new_neighbor.f = new_neighbor.g + new_neighbor.h;

	m_open.push_back(new_neighbor);
	if (new_neighbor.x == m_end_node.x && new_neighbor.y == m_end_node.y)
	{
		m_close.push_back(new_neighbor);
		m_is_find = true;
	}
}

bool AStar::find_node_open(int32_t x, int32_t y)
{
	return find_node(m_open, x, y);
}

bool AStar::find_node_close(int32_t x, int32_t y)
{
	return find_node(m_close, x, y);
}

bool AStar::find_node_barrier(int32_t x, int32_t y)
{
	return find_node(m_barrier, x, y);
}

bool AStar::find_node(const std::vector<node_t>& vec_node, int32_t x, int32_t y)
{
    for (const auto& node : vec_node)
    {
        if (x == node.x && y == node.y)
            return true;
    }

	return false;
}

void AStar::calculate_final_shortest_path()
{
	if (m_close.empty())
		return;

	node_t path_node = m_close.back();
	m_path.clear();
	m_path.push_back(m_close.back());
	bool is_done = false;
	while (!is_done)
	{
		direction_t direct = path_node.parent;
		node_t parent_node(path_node.x - direct.x, path_node.y - direct.y);
        for (const auto& close_node : m_close)
        {
            if (close_node.x == parent_node.x && close_node.y == parent_node.y)
            {
                path_node = close_node;
                if (path_node.x == m_start_node.x && path_node.y == m_start_node.y)
                    is_done = true;
                m_path.push_back(path_node);
                break;
            }
        }
	}

	// 反转顺序
    std::vector<node_t> temp_nodes;
	temp_nodes.swap(m_path);
	for (std::vector<node_t>::const_reverse_iterator iter = temp_nodes.rbegin(); iter != temp_nodes.rend(); ++iter)
		m_path.push_back(*iter);
}
