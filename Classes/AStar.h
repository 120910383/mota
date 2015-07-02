#pragma once

#include <vector>

typedef int int32_t;
using namespace std;

struct direction_t
{
	int32_t x;
	int32_t y;
};

struct node_t
{
	int32_t x;
	int32_t y;
	direction_t parent;
	int32_t f;
	int32_t g;
	int32_t h;

	node_t()
		: x(0)
		, y(0)
		, f(0)
		, g(0)
		, h(0)
	{}
	node_t(int32_t _x, int32_t _y)
		: x(_x)
		, y(_y)
		, f(0)
		, g(0)
		, h(0)
	{}

    bool operator == (const node_t& node) const { return x == node.x && y == node.y; }
};

class AStar
{
public:
	AStar() {}
	AStar(int32_t column, int32_t row);
	virtual ~AStar();

	void init(int32_t column, int32_t row);
	void set_start_and_end(const node_t& start_node, const node_t& end_node);
	void set_blocks(const vector<node_t>& vec_block);
	const vector<node_t>& get_path();
	node_t get_next_turn();
	static void translate_path_to_turns(const vector<node_t>& path, vector<node_t>& turns);

protected:
	node_t calculate_fgh(const node_t& node);
	int32_t get_distance(const node_t& rhs_node, const node_t& lhs_node);
	void get_min_from_open(vector<node_t>& min_vec);
	void do_neighbors(const node_t& node);
	void do_neighbor_node(const direction_t& direct, const node_t& node);

	bool find_node_open(int32_t x, int32_t y);
	bool find_node_close(int32_t x, int32_t y);
	bool find_node_barrier(int32_t x, int32_t y);
	bool find_node(const vector<node_t>& vec_node, int32_t x, int32_t y);

	void calculate_final_shortest_path();

protected:
	int32_t m_column;
	int32_t m_row;

	node_t m_start_node;
	node_t m_end_node;

	vector<node_t> m_open;
	vector<node_t> m_close;
	vector<node_t> m_barrier;
	vector<node_t> m_path;

	bool m_is_find;
};

class AStarGroup
{
public:
	AStarGroup();
	virtual ~AStarGroup();

public:
	struct position_t
	{
		int32_t x;
		int32_t y;
	};

	struct move_t
	{
		position_t current_position;
		position_t next_position;
	};

	struct unit_t
	{
		int32_t guid;
		move_t move;
	};

	vector<unit_t> m_units;
};