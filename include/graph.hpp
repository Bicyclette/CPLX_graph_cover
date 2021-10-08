#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <iostream>
#include <vector>
#include <memory>
#include <utility>
#include <algorithm>
#include <random>

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
		Graph(std::vector<struct vertex> v, std::vector<struct edge> e);
		void display();
		std::vector<struct vertex> getVertices();
		std::vector<struct edge> getEdges();

	private:
		void init();

	private:
		std::vector<struct vertex> vertices;
		std::vector<struct edge> edges;
		std::unique_ptr<std::unique_ptr<bool[]>[]> adjacency;
};

// suppression d'un ensemble de sommets du graphe
// et de toutes les arêtes liées à ce sommet
std::unique_ptr<Graph> removeVertices(Graph & g, std::vector<struct vertex> vertices);

// affiche le degré de chaque sommet du graphe
std::vector<struct vertex> getVerticesDegree(Graph & g);
std::vector<struct vertex> getVerticesDegree(std::unique_ptr<Graph> & g);

// renvoie le sommet de degré maximum
struct vertex getMaxDegreeVertex(Graph & g);
struct vertex getMaxDegreeVertex(std::unique_ptr<Graph> & g);

// generation d'instance avec paramètre p : un flottant entre 0 et 1 tous deux exclus
std::unique_ptr<Graph> createGraph(int num_vertex, double p);

#endif
