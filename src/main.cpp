#include "graph.hpp"
#include <chrono>

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
	
	auto start = std::chrono::high_resolution_clock::now();
	std::vector<struct vertex> res1 = g.branch();
	auto stop = std::chrono::high_resolution_clock::now();
	auto duration1 = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
	std::cout << "branch method took : " << duration1.count() << " microseconds." << std::endl;

	start = std::chrono::high_resolution_clock::now();
	std::vector<struct vertex> res2 = g.branch_bound();
	stop = std::chrono::high_resolution_clock::now();
	auto duration2 = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
	std::cout << "branch & bound method took : " << duration2.count() << " microseconds." << std::endl;

	std::cout << "solution branch : ";
	for(auto v : res1)
		std::cout << v.id << ", ";
	std::cout << std::endl;
	std::cout << "solution branch & bound : ";
	for(auto v : res2)
		std::cout << v.id << ", ";
	std::cout << std::endl;

	std::cout << "Branch & bound method is " << duration1.count() / duration2.count() << " times faster on the current graph." << std::endl;

	return 0;
}
