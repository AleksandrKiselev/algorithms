#pragma once
#include <vector>

namespace dijkstra
{

using Vertex = int;
using Distance = int;
using Path = std::vector<Vertex>;
using AdjacencyMatrix = std::vector<std::vector<Distance>>;

Path find_shortest_path(const AdjacencyMatrix& matrix, Vertex from, Vertex to)
{
	
}

}
