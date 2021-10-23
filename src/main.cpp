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
	std::cout << "4.1 : " << duration1.count() << " microseconds." << std::endl;

	start = std::chrono::high_resolution_clock::now();
	std::vector<struct vertex> res2 = g.branch_bound();
	stop = std::chrono::high_resolution_clock::now();
	auto duration2 = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
	std::cout << "4.2 : " << duration2.count() << " microseconds." << std::endl;
	
	start = std::chrono::high_resolution_clock::now();
	std::vector<struct vertex> res3 = g.branch_bound_v2();
	stop = std::chrono::high_resolution_clock::now();
	auto duration3 = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
	std::cout << "4.3 : " << duration3.count() << " microseconds." << std::endl;

	std::cout << "\nsolution 4.1 : (";
	for(int i{0}; i < res1.size(); ++i)
	{
		if(i == res1.size()-1)
			std::cout << res1[i].id << ")";
		else
			std::cout << res1[i].id << ", ";
	}
	std::cout << std::endl;

	std::cout << "solution 4.2 : (";
	for(int i{0}; i < res2.size(); ++i)
	{
		if(i == res2.size()-1)
			std::cout << res2[i].id << ")";
		else
			std::cout << res2[i].id << ", ";
	}
	std::cout << std::endl;
	
	std::cout << "solution 4.3 : (";
	for(int i{0}; i < res3.size(); ++i)
	{
		if(i == res3.size()-1)
			std::cout << res3[i].id << ")";
		else
			std::cout << res3[i].id << ", ";
	}
	std::cout << std::endl;

	std::cout << "\n4.2 acceleration = " << duration1.count() / duration2.count() << std::endl;
	std::cout << "4.3 acceleration =  " << duration1.count() / duration3.count() << std::endl;

	g.display();
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
