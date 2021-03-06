#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <iostream>
#include <fstream>
#include <vector>
#include <memory>
#include <utility>
#include <algorithm>
#include <random>
#include <functional>
#include <stack>
#include <string>
#include <cstring>
#include <cmath>
#include <limits>

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
		Graph(){};
		Graph(int num_vertex, double p);
		Graph(std::vector<struct vertex> v, std::vector<struct edge> e);
		Graph(const Graph & g);
		Graph(std::string file);
		void display();
		std::vector<struct vertex> getVertices();
		std::vector<struct vertex> getVerticesDegree();
		std::vector<struct edge> getEdges();
		void removeVertices(std::vector<struct vertex> vertices);
		Graph removeVerticesCpy(std::vector<struct vertex> vertices);
		struct vertex getMaxDegreeVertex();
		std::vector<struct vertex> algo_couplage();
		std::vector<struct vertex> algo_glouton();
		std::vector<struct vertex> branch();
		std::vector<struct vertex> branch_bound();
		std::vector<struct vertex> branch_bound_v2();
		std::vector<struct vertex> branch_bound_v3();

	public: // operator
		Graph & operator=(const Graph & g);

	private:
		void init();
		std::vector<struct vertex> get_reachable_vertices(int v);

	private:
		std::vector<struct vertex> vertices;
		std::vector<struct edge> edges;
		std::unique_ptr<std::unique_ptr<bool[]>[]> adjacency;

	// séparation et évaluation
	public:
		std::vector<struct vertex> path;
		static std::vector<struct vertex> solution; // notre best value
		static std::stack<Graph> branch_stack;
		static std::stack<std::vector<struct vertex>> reachable_stack;
		static double borne_sup;
		static double borne_inf;
};


#endif
