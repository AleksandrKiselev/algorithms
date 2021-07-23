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
	ASSERT_EQ(find_shortest_path(AdjacencyMatrix{}, 0, 0), Path{});
}

TEST(FindShortestPath, OnePointMatrix)
{
	ASSERT_EQ(find_shortest_path(AdjacencyMatrix{{ 0 }}, 0, 0), Path{ 0 });
}

}
