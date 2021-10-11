#include "graph.hpp"

Graph::Graph(std::vector<struct vertex> v, std::vector<struct edge> e) :
	vertices(v),
	edges(e)
{
	std::sort(vertices.begin(), vertices.end(),
			[] (struct vertex a, struct vertex b) -> bool{
				return (a.id < b.id) ? true : false;
			});
	init();
}

void Graph::init()
{
	std::function getVertexIndex = [] (std::vector<vertex> & v, int id) -> int {
		int index{-1};
		for(int i{0}; i < v.size(); ++i)
		{
			if(v[i].id == id)
			{
				index = i;
				break;
			}
		}
		return index;
	};

	adjacency = std::make_unique<std::unique_ptr<bool[]>[]>(vertices.size());
	for(int i{0}; i < vertices.size(); ++i)
		adjacency[i] = std::make_unique<bool[]>(vertices.size());

	for(int e{0}; e < edges.size(); ++e)
	{
		int i{getVertexIndex(vertices, edges[e].from)};
		int j{getVertexIndex(vertices, edges[e].to)};
		if(i == -1 || j == -1)
		{
			std::cerr << "Erreur: arête référençant des sommets inexistants." << std::endl;
			std::exit(-1);
		}
		adjacency[i][j] = true;
		adjacency[j][i] = true;
	}

	// compute vertex degree
	for(int i{0}; i < vertices.size(); ++i)
	{
		for(int j{0}; j < edges.size(); ++j)
		{
			int fromId{getVertexIndex(vertices, edges[j].from)};
			int toId{getVertexIndex(vertices, edges[j].to)};
			if(vertices[i].id == fromId || vertices[i].id == toId)
				vertices[i].degree++;
		}
	}
}

void Graph::display()
{
	long unsigned int N{vertices.size()};
	std::unique_ptr<char[]> separator{std::make_unique<char[]>(2*N+2)};
	for(int i{0}; i < 2*N+1; ++i)
		separator[i] = '-';

	std::cout << "========== ADJACENCY MATRIX ==========" << std::endl;
	std::cout << " ";
	for(int i{0}; i < N; ++i)
		std::cout << "\033[1;31m " << vertices[i].id << "\033[0m";
	std::cout << std::endl << "\033[1;31m " << separator.get() << "\033[0m" << std::endl;
	for(int i{0}; i < N; ++i)
	{
		std::cout << "\033[1;31m" << vertices[i].id << "\033[0m";
		for(int j{0}; j < N; ++j)
		{
			std::cout << "|" << adjacency[i][j];
		}
		std::cout << "|" << std::endl << "\033[1;31m-\033[0m" << separator.get() << std::endl;
	}
}

std::vector<struct vertex> Graph::getVertices()
{
	return vertices;
}

std::vector<struct edge> Graph::getEdges()
{
	return edges;
}

std::unique_ptr<Graph> removeVertices(Graph & g, std::vector<struct vertex> vertices)
{
	std::function getVertexIndex = [] (std::vector<vertex> & v, int id) -> int {
		int index{-1};
		for(int i{0}; i < v.size(); ++i)
		{
			if(v[i].id == id)
			{
				index = i;
				break;
			}
		}
		return index;
	};

	std::vector<struct vertex> graph_vertices{g.getVertices()};
	std::vector<struct edge> graph_edges{g.getEdges()};

	std::vector<struct vertex> new_graph_vertices;
	std::vector<struct edge> new_graph_edges;

	bool rm{false};
	for(int e{0}; e < graph_edges.size(); ++e)
	{
		int v1{getVertexIndex(graph_vertices, graph_edges[e].from)};
		int v2{getVertexIndex(graph_vertices, graph_edges[e].to)};
		for(int i{0}; i < vertices.size(); ++i)
		{
			int id{vertices[i].id};
			if(v1 == id || v2 == id)
			{
				rm = true;
				break;
			}
		}
		if(!rm)
		{
			struct vertex new_v1 = vertex(v1);
			struct vertex new_v2 = vertex(v2);
			struct edge new_edge = edge(v1, v2);
			
			bool findv1{false};
			bool findv2{false};
			for(int j{0}; j < new_graph_vertices.size(); ++j)
			{
				if(new_graph_vertices[j].id == new_v1.id)
					findv1 = true;
				else if(new_graph_vertices[j].id == new_v2.id)
					findv2 = true;
			}

			new_graph_edges.push_back(new_edge);
			if(!findv1){new_graph_vertices.push_back(new_v1);}
			if(!findv2){new_graph_vertices.push_back(new_v2);}
		}
		else
			rm = false;
	}

	std::sort(new_graph_vertices.begin(), new_graph_vertices.end(),
			[] (struct vertex a, struct vertex b) -> bool{
				return (a.id < b.id) ? true : false;
			});

	std::unique_ptr<Graph> new_g{std::make_unique<Graph>(new_graph_vertices, new_graph_edges)};
	return new_g;
}

std::vector<struct vertex> getVerticesDegree(Graph & g)
{
	return g.getVertices();
}

std::vector<struct vertex> getVerticesDegree(std::unique_ptr<Graph> & g)
{
	return g->getVertices();
}


struct vertex getMaxDegreeVertex(Graph & g)
{
	std::vector<struct vertex> vertices{g.getVertices()};
	int index{0};
	for(int i{1}; i < vertices.size(); ++i)
	{
		if(vertices[i].degree > vertices[index].degree)
			index = i;
	}
	return vertices[index];
}

struct vertex getMaxDegreeVertex(std::unique_ptr<Graph> & g)
{
	std::vector<struct vertex> vertices{g->getVertices()};
	int index{0};
	for(int i{1}; i < vertices.size(); ++i)
	{
		if(vertices[i].degree > vertices[index].degree)
			index = i;
	}
	return vertices[index];
}

std::unique_ptr<Graph> createGraph(int num_vertex, double p)
{
	std::function getVertexIndex = [] (std::vector<vertex> & v, int id) -> int {
		int index{-1};
		for(int i{0}; i < v.size(); ++i)
		{
			if(v[i].id == id)
			{
				index = i;
				break;
			}
		}
		return index;
	};

	std::random_device rd;
	std::mt19937 mt(rd());
	std::uniform_real_distribution<double> dist(0.0, 1.0);

	std::vector<struct vertex> v;
	std::vector<struct edge> e;

	for(int i{0}; i < num_vertex; ++i)
	{
		for(int j{i+1}; j < num_vertex; ++j)
		{
			if(getVertexIndex(v, i) == -1)
				v.emplace_back(i);
			if(getVertexIndex(v, j) == -1)
				v.emplace_back(j);
			if(dist(mt) <= p)
				e.emplace_back(i, j);
		}
	}

	std::unique_ptr<Graph> g{std::make_unique<Graph>(v, e)};
	return g;
}
