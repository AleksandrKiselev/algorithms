#pragma once
#include <gtest/gtest.h>
#include <vector>
#include <queue>

namespace dijkstra
{

using Vertex = size_t;
using Distance = size_t;
using Path = std::vector<Vertex>;
using AdjacencyMatrix = std::vector<std::vector<Distance>>;

const auto cUnreachable = std::numeric_limits<Distance>::max();

Path find_shortest_path(const AdjacencyMatrix& matrix, Vertex start, Vertex finish)
{
	if (!matrix.size())
		return {};

	if (start == finish)
		return { start };

	struct VertexData
	{
		Vertex vertex_{ 0 };
		const VertexData* from_{ nullptr };
		Distance distance_{ cUnreachable };
	};

	std::map<Vertex, VertexData> vertices_data;
	auto create_vertex_data = [&vertices_data](const Vertex& vertex, const VertexData* from, const Distance& distance) -> VertexData*
	{
		VertexData* data = &vertices_data[vertex];
		data->vertex_ = vertex;
		data->from_ = from;
		data->distance_ = distance;
		return data;
	};

	auto get_vertex_data = [&vertices_data](const Vertex& vertex) ->VertexData*
	{ 
		VertexData* data = &vertices_data[vertex];
		data->vertex_ = vertex;
		return data;
	};

	auto comp = [](const VertexData* rhs, const VertexData* lhs)
	{ return rhs->distance_ > lhs->distance_; };
	std::priority_queue<
		VertexData*, 
		std::vector<VertexData*>, 
		decltype(comp)> vertices_queue(comp);

	auto add_reachable_vertices_to_queue = [&](const VertexData* data)
	{
		const Vertex& vertex = data->vertex_;
		for (size_t i = 0; i < matrix.size(); ++i)
		{
			if (i == vertex)
				continue;

			const Distance distance = matrix[i][vertex];
			if (distance == cUnreachable)
				continue;

			auto* added_data = get_vertex_data(i);
			if (added_data->distance_ <= distance + data->distance_)
				continue;

			added_data->distance_ = distance + data->distance_;
			added_data->from_ = data;
			vertices_queue.emplace(added_data);
		}
	};

	add_reachable_vertices_to_queue(create_vertex_data(start, nullptr, 0));

	while (!vertices_queue.empty())
	{
		auto* data = vertices_queue.top();
		vertices_queue.pop();
		if (data->vertex_ == finish) 
			break;

		add_reachable_vertices_to_queue(data);
	}

	Path shortest_path;

	const VertexData* curr_data = get_vertex_data(finish);
	if (curr_data->from_)
	{
		while (curr_data)
		{
			shortest_path.push_back(curr_data->vertex_);
			curr_data = curr_data->from_;
		}

		std::reverse(shortest_path.begin(), shortest_path.end());
	}
	
	return shortest_path;
}

TEST(FindShortestPath, EmptyMatrix)
{
	const AdjacencyMatrix empty_matrix;
	const Path empty_path;
	const Path shortest_path = find_shortest_path(empty_matrix, 0, 0);
	ASSERT_EQ(shortest_path, empty_path);
}

TEST(FindShortestPath, OnePointMatrix)
{
	const AdjacencyMatrix one_point_matrix{ { 0 } };
	const Path one_point_path{ 0 };
	const Path shortest_path = find_shortest_path(one_point_matrix, 0, 0);
	ASSERT_EQ(shortest_path, one_point_path);
}

TEST(FindShortestPath, SimpleMatrix)
{
	const AdjacencyMatrix two_points_matrix{ { 0, 1 }, { 1, 0 } };
	const Path expected_path{ 0, 1 };
	const Path shortest_path = find_shortest_path(two_points_matrix, 0, 1);
	ASSERT_EQ(shortest_path, expected_path);
}

TEST(FindShortestPath, SmallMatrix)
{
	const auto u = cUnreachable;
	const AdjacencyMatrix matrix
	{ 
		{ 0, 2, u, 9, u, u },
		{ 2, 0, u, u, 3, u },
		{ u, u, 0, 1, 1, u },
		{ 9, u, 1, 0, u, 1 },
		{ u, 3, 1, u, 0, 9 },
		{ u, u, u, 1, 9, 0 },
	};
	const Path expected_path{ 0, 1, 4, 2, 3, 5 };
	const Path shortest_path = find_shortest_path(matrix, 0, 5);
	ASSERT_EQ(shortest_path, expected_path);
}

}
