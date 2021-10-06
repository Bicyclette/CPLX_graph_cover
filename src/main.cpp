#include "graph.hpp"

int main(int argc, char * argv[])
{
	std::vector<struct vertex> vertices{
		{0},
		{1},
		{2},
		{3},
		{4}
	};

	std::vector<struct edge> edges{
		edge{vertices[0], vertices[1]},
		edge{vertices[1], vertices[4]},
		edge{vertices[3], vertices[4]},
		edge{vertices[0], vertices[3]},
		edge{vertices[0], vertices[2]},
		edge{vertices[1], vertices[2]},
		edge{vertices[2], vertices[3]},
		edge{vertices[2], vertices[4]}
	};

	Graph g(vertices, edges);
	g.display();

	return 0;
}
