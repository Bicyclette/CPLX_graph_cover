#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <iostream>
#include <vector>
#include <memory>
#include <utility>
#include <algorithm>
#include <random>
#include <functional>
#include <stack>

struct vertex
{
	vertex(int label) : id(label), degree(0) {}
	int id;
	int degree;
};

struct edge
{
	edge(int a, int b) : from(a), to(b) {}
	int from;
	int to;
};

class Graph
{
	public:
		Graph(int num_vertex, double p);
		Graph(std::vector<struct vertex> v, std::vector<struct edge> e);
		Graph(const Graph & g);
		void display();
		std::vector<struct vertex> getVertices();
		std::vector<struct vertex> getVerticesDegree();
		std::vector<struct edge> getEdges();
		void removeVertices(std::vector<struct vertex> vertices);
		Graph removeVerticesCpy(std::vector<struct vertex> vertices);
		struct vertex getMaxDegreeVertex();
		std::vector<struct vertex> algo_couplage();
		std::vector<struct vertex> algo_glouton();
		std::vector<struct vertex> init_branch();
		void branch(const std::vector<struct vertex> & g_explore);

	private:
		void init();
		std::vector<struct vertex> get_reachable_vertices(int v);

	private:
		std::vector<struct vertex> vertices;
		std::vector<struct edge> edges;
		std::unique_ptr<std::unique_ptr<bool[]>[]> adjacency;

	// séparation et évaluation
	private:
		std::vector<struct vertex> explore;
	public:
		static std::vector<struct vertex> solution; // notre best value
		static std::vector<struct vertex> path;
		static std::stack<std::reference_wrapper<Graph>> branch_stack;
};


#endif
