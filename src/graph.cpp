#include "graph.hpp"

Graph::Graph(int num_vertex, double p)
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

	for(int i{0}; i < num_vertex; ++i)
	{
		for(int j{i+1}; j < num_vertex; ++j)
		{
			if(getVertexIndex(vertices, i) == -1)
				vertices.emplace_back(i);
			if(getVertexIndex(vertices, j) == -1)
				vertices.emplace_back(j);
			if(dist(mt) <= p)
				edges.emplace_back(i, j);
		}
	}

	init();
}

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

std::vector<struct vertex> Graph::getVerticesDegree()
{
	return vertices;
}

std::vector<struct edge> Graph::getEdges()
{
	return edges;
}

void Graph::removeVertices(std::vector<struct vertex> rm)
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

	for(int e{0}; e < edges.size(); ++e)
	{
		int v1{getVertexIndex(vertices, edges[e].from)};
		int v2{getVertexIndex(vertices, edges[e].to)};
		for(int i{0}; i < rm.size(); ++i)
		{
			int id{rm[i].id};
			if(vertices[v1].id == id || vertices[v2].id == id)
			{
				vertices[v1].degree--;
				vertices[v2].degree--;
				edges.erase(edges.begin() + e);
				break;
			}
		}
	}

	for(int v{0}; v < rm.size(); ++v)
		for(int i{0}; i < vertices.size(); ++i)
			if(vertices[i].id == rm[v].id)
				vertices.erase(vertices.begin() + i);

	std::sort(vertices.begin(), vertices.end(),
			[] (struct vertex a, struct vertex b) -> bool{
				return (a.id < b.id) ? true : false;
			});
}

struct vertex Graph::getMaxDegreeVertex()
{
	int index{0};
	for(int i{1}; i < vertices.size(); ++i)
	{
		if(vertices[i].degree > vertices[index].degree)
			index = i;
	}
	return vertices[index];
}

std::vector<struct vertex> Graph::algo_couplage()
{
	std::vector<struct vertex> couverture;
	for(auto e : edges)
	{
		bool inside{false};
		for(int i{0}; i < couverture.size(); ++i)
		{
			struct vertex v{couverture[i]};
			if(v.id == e.from || v.id == e.to)
			{
				inside = true;
				break;
			}
		}
		if(!inside)
		{
			couverture.push_back(e.from);
			couverture.push_back(e.to);
		}
	}
	return couverture;
}

std::vector<struct vertex> Graph::algo_glouton()
{
	std::vector<struct vertex> couverture;
	while(!edges.empty())
	{
		struct vertex v{getMaxDegreeVertex()};
		if(v.degree == -1 && v.id == -1)
			break;
		couverture.push_back(v);
		removeVertices({v});
		for(int e{0}; e < edges.size(); ++e)
		{
			if(v.id == edges[e].from || v.id == edges[e].to)
				edges.erase(edges.begin() + e);
		}
	}

	return couverture;
}
