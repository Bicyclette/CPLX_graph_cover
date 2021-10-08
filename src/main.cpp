#include "graph.hpp"

int main(int argc, char * argv[])
{
	std::unique_ptr<Graph> g = createGraph(5, 0.5);
	g->display();

	return 0;
}
