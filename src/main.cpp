#include "graph.hpp"

int main(int argc, char * argv[])
{
	/*
	Graph g(4, 0.5);
	g.display();
	std::vector<struct vertex> degree = g.getVerticesDegree();
	for(auto v : degree)
		std::cout << "vertex " << v.id << ", degree = " << v.degree << std::endl;

	std::vector<struct vertex> couverture1 = g.algo_couplage();
	std::vector<struct vertex> couverture2 = g.algo_glouton();
	std::cout << "COUVERTURE couplage :" << std::endl;
	for(auto v : couverture1)
		std::cout << "vertex " << v.id << std::endl;
	std::cout << "COUVERTURE glouton :" << std::endl;
	for(auto v : couverture2)
		std::cout << "vertex " << v.id << std::endl;
*/
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

	return 0;
}
