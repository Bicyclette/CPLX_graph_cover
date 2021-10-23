#include "graph.hpp"
#include <chrono>

void arg_error()
{
	std::cerr << "\033[1;91m";
	std::cerr << "Error : you must provide 2 parameters for text file creation mode,\n"
	"or 3 parameters the for random graph generation.\n" << std::endl;
	std::cerr << "\033[0m";
	std::cerr << "\033[4;92m";
	std::cerr << "Use one of these two following forms :" << std::endl;
	std::cerr << "\033[0m" << std::endl;
	std::cerr << "Usage : ./graph_cover -f <instance.txt>" << std::endl;
	std::cerr << "Usage : ./graph_cover -n <vertex count> <probability>" << std::endl;
	std::cerr << std::endl;
	std::exit(-1);
}

void exec(Graph & g)
{
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
	
	start = std::chrono::high_resolution_clock::now();
	std::vector<struct vertex> res3 = g.branch_bound_v2();
	stop = std::chrono::high_resolution_clock::now();
	auto duration3 = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
	std::cout << "branch & bound v2 method took : " << duration3.count() << " microseconds." << std::endl;

	std::cout << "\nsolution branch : ";
	for(auto v : res1)
		std::cout << v.id << ", ";
	std::cout << std::endl;

	std::cout << "solution branch & bound : ";
	for(auto v : res2)
		std::cout << v.id << ", ";
	std::cout << std::endl;
	std::cout << "solution branch & bound v2 : ";
	for(auto v : res3)
		std::cout << v.id << ", ";
	std::cout << std::endl;

	std::cout << "\nBranch & bound method is " << duration1.count() / duration2.count() << " times faster on the current graph." << std::endl;
	std::cout << "Branch & bound v2 method is " << duration1.count() / duration3.count() << " times faster on the current graph." << std::endl;
}

int main(int argc, char * argv[])
{
	// Parse command line arguments
	if(argc < 3 || argc > 4)
		arg_error();
	else
	{
		if(strncmp(argv[1], "-f", 2) == 0) // instance file
		{
			if(argc != 3)
				arg_error();
			std::string instance{argv[2]};
			Graph g(instance);
			exec(g);
		}
		else if(strncmp(argv[1], "-n", 2) == 0) // random graph of n vertices
		{
			if(argc != 4)
				arg_error();
			int vertices{std::stoi(argv[2])};
			double probability{std::stod(argv[3])};
			Graph g(vertices, probability);
			exec(g);
		}
		else
			arg_error();
	}

	return 0;
}
