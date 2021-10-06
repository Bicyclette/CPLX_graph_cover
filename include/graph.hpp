#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <iostream>
#include <vector>
#include <memory>
#include <utility>

struct vertex
{
	int id;
};

struct edge
{
	struct vertex v1;
	struct vertex v2;
};

class Graph
{
	public:
		Graph(std::vector<struct vertex> v, std::vector<struct edge> e);
		void display();

	private:
		void init();

	private:
		std::vector<struct vertex> vertices;
		std::vector<struct edge> edges;
		std::unique_ptr<std::unique_ptr<bool[]>[]> adjacency;
};

#endif
