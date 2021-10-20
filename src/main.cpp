#include "graph.hpp"

int main(int argc, char * argv[])
{
	/*
	std::vector<struct vertex> lv{0, 1, 2, 3};
	std::vector<struct edge> le;
	le.emplace_back(0,1);
	le.emplace_back(1,2);
	le.emplace_back(2,3);
	Graph g(lv, le);
	g.display();
	std::vector<struct vertex> res = g.branch();
	std::cout << "solution finale : ";
	for(auto v : res)
		std::cout << v.id << ", ";
	std::cout << std::endl;
	*/
	Graph g("../instances/exempleinstance.txt");
	g.display();

	return 0;
}
