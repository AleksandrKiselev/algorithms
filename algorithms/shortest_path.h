#pragma once
#include <gtest/gtest.h>
#include <unordered_map>
#include <numeric>
#include <vector>
#include <queue>
#include <set>

namespace dijkstra
{

using vertex_t = std::size_t;
using vertices_t = std::vector<vertex_t>;
using vertices_set_t = std::set<vertex_t>;

using distance_t = std::size_t;
const distance_t c_max_distance = std::numeric_limits<distance_t>::max();

using edge_t = std::pair<vertex_t, vertex_t>;

struct edge_hash
{
	std::size_t operator()(const edge_t& edge) const noexcept
	{
		const auto& [minv, maxv] = std::minmax(edge.first, edge.second);
		const auto h1 = std::hash<vertex_t>{}(minv);
		const auto h2 = std::hash<vertex_t>{}(maxv);
		return h1 ^ (h2 << 1);
	}
};

struct edge_equal
{
	std::size_t operator()(const edge_t& lhs, const edge_t& rhs) const noexcept
	{
		const auto& [minl, maxl] = std::minmax(lhs.first, lhs.second);
		const auto& [minr, maxr] = std::minmax(rhs.first, rhs.second);
		return minl == minr && maxl == maxr;
	}
};

using edges_t = std::unordered_map<edge_t, distance_t, edge_hash, edge_equal>;

class graph_t
{
public:
	void add_edge(const vertex_t& vertex1, const vertex_t& vertex2, const distance_t& distance)
	{
		vertices_.emplace(vertex1);
		vertices_.emplace(vertex2);
		if (!edges_.emplace(edge_t{ vertex1, vertex2 }, distance).second)
		{
			throw std::logic_error("edge already exists");
		}
	}

	bool has_edge(const vertex_t& vertex1, const vertex_t& vertex2) const
	{
		return edges_.find(edge_t{ vertex1, vertex2 }) != edges_.end();
	}

	std::size_t distance(const vertex_t& vertex1, const vertex_t& vertex2) const
	{
		const auto it = edges_.find(edge_t{ vertex1, vertex2 });
		if (it == edges_.end())
		{
			throw std::logic_error("edge doesn't exist");
		}
		return it->second;
	}

	vertices_t adjacent_vertices(const vertex_t& vertex1) const
	{
		vertices_t result;
		for (const auto& vertex2 : vertices_)
		{
			if (has_edge(vertex1, vertex2))
			{
				result.push_back(vertex2);
			}
		}
		return result;
	}

	std::size_t vertices_count() const noexcept { return vertices_.size(); }
	std::size_t edges_count() const noexcept { return edges_.size(); }

private:
	vertices_set_t vertices_;
	edges_t edges_;
};

std::pair<vertices_t, distance_t> find_shortest_path(const graph_t& graph, vertex_t from, vertex_t to)
{
	struct metadata_t
	{
		vertex_t vertex{ 0 };
		const metadata_t* from{ nullptr };
		distance_t distance{ c_max_distance };
	};

	using metadata_map_t = std::unordered_map<vertex_t, metadata_t>;
	using metadata_vec_t = std::vector<metadata_t*>;

	metadata_map_t metadata_map;
	auto get_md = [&metadata_map](const vertex_t& vertex) -> metadata_t*
	{
		auto* md = &metadata_map[vertex];
		md->vertex = vertex;
		return md;
	};

	auto comparator = [](const metadata_t* rhs, const metadata_t* lhs)
	{ return rhs->distance > lhs->distance; };
	using metadata_queue_t = std::priority_queue<metadata_t*, metadata_vec_t, decltype(comparator)>;
	metadata_queue_t min_distance_queue(comparator);

	auto add_reachable_vertices_to_queue = [&graph, &min_distance_queue, &get_md](const vertex_t& vertex)
	{
		const auto* md = get_md(vertex);
		const auto& adj_vertices = graph.adjacent_vertices(vertex);
		for (const auto& adj_vertex : adj_vertices)
		{
			const auto distance = graph.distance(vertex, adj_vertex);
			auto* adj_md = get_md(adj_vertex);

			if (adj_md->distance <= distance + md->distance)
				continue;

			adj_md->distance = distance + md->distance;
			adj_md->from = md;
			min_distance_queue.emplace(adj_md);
		}
	};

	get_md(from)->distance = 0;
	add_reachable_vertices_to_queue(from);

	while (!min_distance_queue.empty())
	{
		auto* md = min_distance_queue.top();
		min_distance_queue.pop();
		if (md->vertex == from) break;
		add_reachable_vertices_to_queue(md->vertex);
	}

	vertices_t path;
	distance_t distance = 0;
	const metadata_t* path_part_md = get_md(to);
	if (path_part_md->from)
	{
		distance = path_part_md->distance;
		while (path_part_md)
		{
			path.push_back(path_part_md->vertex);
			path_part_md = path_part_md->from;
		}
		std::reverse(path.begin(), path.end());
	}

	return { path, distance };
}

TEST(FindShortestPath, EmptyGraph)
{
	const auto expected = std::make_pair(vertices_t{}, distance_t{ 0 });
	const auto shortest = find_shortest_path(graph_t{}, 0, 0);
	ASSERT_EQ(shortest, expected);
}

TEST(FindShortestPath, OneEdgeGraph)
{
	graph_t graph;
	graph.add_edge(0, 1, 1);

	const auto expected = std::make_pair(vertices_t{ 0, 1 }, distance_t{ 1 });
	const auto shortest = find_shortest_path(graph, 0, 1);
	ASSERT_EQ(shortest, expected);
}

TEST(FindShortestPath, SixEdgesGraph)
{
	graph_t graph;
	graph.add_edge(0, 1, 2);
	graph.add_edge(0, 3, 9);
	graph.add_edge(1, 4, 3);
	graph.add_edge(2, 3, 1);
	graph.add_edge(2, 4, 1);
	graph.add_edge(3, 5, 1);
	graph.add_edge(4, 5, 9);

	const auto expected = std::make_pair(vertices_t{ 0, 1, 4, 2, 3, 5 }, distance_t{ 8 });
	const auto shortest = find_shortest_path(graph, 0, 5);
	ASSERT_EQ(shortest, expected);
}

} // namespace dijkstra
