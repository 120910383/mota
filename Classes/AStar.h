#pragma once

class AStar
{
public:
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

        node_t() {}
        node_t(int32_t _x, int32_t _y)
            : x(_x)
            , y(_y)
        {}

        bool operator == (const node_t& node) const { return x == node.x && y == node.y; }
    };

public:
	AStar() {}
	AStar(int32_t column, int32_t row);
	virtual ~AStar();

	void init(int32_t column, int32_t row);
	void set_start_and_end(const node_t& start_node, const node_t& end_node);
	void set_blocks(const std::vector<node_t>& vec_block);
	const std::vector<node_t>& get_path();
	node_t get_next_turn();
	static void translate_path_to_turns(const std::vector<node_t>& path, std::vector<node_t>& turns);

protected:
	node_t calculate_fgh(const node_t& node);
	int32_t get_distance(const node_t& rhs_node, const node_t& lhs_node);
	void get_min_from_open(std::vector<node_t>& min_vec);
	void do_neighbors(const node_t& node);
	void do_neighbor_node(const direction_t& direct, const node_t& node);

	bool find_node_open(int32_t x, int32_t y);
	bool find_node_close(int32_t x, int32_t y);
	bool find_node_barrier(int32_t x, int32_t y);
	bool find_node(const std::vector<node_t>& vec_node, int32_t x, int32_t y);

	void calculate_final_shortest_path();

protected:
	int32_t m_column;
	int32_t m_row;

	node_t m_start_node;
	node_t m_end_node;

    std::vector<node_t> m_open;
    std::vector<node_t> m_close;
    std::vector<node_t> m_barrier;
    std::vector<node_t> m_path;

	bool m_is_find;
};

/*
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

    std::vector<unit_t> m_units;
};
*/