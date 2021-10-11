#include "graph.hpp"

int main(int argc, char * argv[])
{
	std::unique_ptr<Graph> g = createGraph(5, 0.5);
	g->display();
	std::vector<struct vertex> degree = getVerticesDegree(g);
	for(auto v : degree)
		std::cout << "vertex " << v.id << ", degree = " << v.degree << std::endl;

	return 0;
}
