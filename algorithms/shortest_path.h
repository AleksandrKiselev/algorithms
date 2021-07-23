#pragma once
#include <gtest/gtest.h>
#include <vector>

namespace dijkstra
{

using Vertex = int;
using Distance = int;
using Path = std::vector<Vertex>;
using AdjacencyMatrix = std::vector<std::vector<Distance>>;

Path find_shortest_path(const AdjacencyMatrix& matrix, Vertex from, Vertex to)
{
	Path shortest_path;
	if (!matrix.size())
		return shortest_path;

	if (from == to)
		return { from };

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

}

}
