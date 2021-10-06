#include "graph.hpp"

Graph::Graph(std::vector<struct vertex> v, std::vector<struct edge> e) :
	vertices(v),
	edges(e)
{
	init();
}

void Graph::init()
{
	adjacency = std::make_unique<std::unique_ptr<bool[]>[]>(vertices.size());
	for(int i{0}; i < vertices.size(); ++i)
		adjacency[i] = std::make_unique<bool[]>(vertices.size());

	for(int e{0}; e < edges.size(); ++e)
	{
		int i = edges[e].v1.id;
		int j = edges[e].v2.id;
		adjacency[i][j] = true;
		adjacency[j][i] = true;
	}
}

void Graph::display()
{
	int N{vertices.size()};
	std::unique_ptr<char[]> separator{std::make_unique<char[]>(2*N+2)};
	for(int i{0}; i < 2*N+1; ++i)
		separator[i] = '-';

	std::cout << "========== ADJACENCY MATRIX ==========" << std::endl;
	std::cout << " ";
	for(int i{1}; i <= N; ++i)
		std::cout << "\033[1;31m " << i << "\033[0m";
	std::cout << std::endl << "\033[1;31m " << separator.get() << "\033[0m" << std::endl;
	for(int i{0}; i < N; ++i)
	{
		std::cout << "\033[1;31m" << i+1 << "\033[0m";
		for(int j{0}; j < N; ++j)
		{
			std::cout << "|" << adjacency[i][j];
		}
		std::cout << "|" << std::endl << "\033[1;31m-\033[0m" << separator.get() << std::endl;
	}
}
