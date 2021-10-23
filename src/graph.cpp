#include "graph.hpp"

std::vector<struct vertex> Graph::solution;
std::stack<Graph> Graph::branch_stack;
std::stack<std::vector<struct vertex>> Graph::reachable_stack;
double Graph::borne_sup;
double Graph::borne_inf;

Graph & Graph::operator=(const Graph & g)
{
	vertices = g.vertices;
	edges = g.edges;
	path = g.path;
	init();
	return *this;
}

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

Graph::Graph(const Graph & g) :
	vertices(g.vertices),
	edges(g.edges),
	path(g.path)
{
	init();
}

Graph::Graph(std::string file)
{
	std::function tokenize =
		[] (std::string const & str, const char delim, std::vector<std::string> & out)
		{
			size_t start;
			size_t end{0};
			while((start = str.find_first_not_of(delim, end)) != std::string::npos)
			{
				end = str.find(delim, start);
				out.push_back(str.substr(start, end - start));
			}
		};

	std::ifstream instance;
	instance.open(file);

	// récupération des données
	std::string line;
	int num_vertex{0};
	int num_edge{0};
	std::vector<struct vertex> lv;
	std::vector<struct edge> le;

	// nombre de sommets
	std::getline(instance, line);
	std::getline(instance, line);
	num_vertex = std::stoi(line);

	// sommets
	std::getline(instance, line);
	for(int i{0}; i < num_vertex; ++i)
	{
		std::getline(instance, line);
		lv.emplace_back(std::stoi(line));
	}

	// nombre d'arêtes
	std::getline(instance, line);
	std::getline(instance, line);
	num_edge = std::stoi(line);

	// arêtes
	std::getline(instance, line);
	for(int i{0}; i < num_edge; ++i)
	{
		std::getline(instance, line);
		std::vector<std::string> parts;
		tokenize(line, ' ', parts);
		std::string from = parts[0];
		std::string to = parts[1];
		le.emplace_back(std::stoi(from), std::stoi(to));
	}

	// création du graphe
	vertices = lv;
	edges = le;
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

Graph Graph::removeVerticesCpy(std::vector<struct vertex> rm)
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

	std::vector<struct vertex> new_vertices = vertices;
	std::vector<struct edge> new_edges = edges;

	for(int e{0}; e < new_edges.size(); ++e)
	{
		int v1{getVertexIndex(new_vertices, new_edges[e].from)};
		int v2{getVertexIndex(new_vertices, new_edges[e].to)};
		for(int i{0}; i < rm.size(); ++i)
		{
			int id{rm[i].id};
			if(new_vertices[v1].id == id || new_vertices[v2].id == id)
			{
				new_vertices[v1].degree--;
				new_vertices[v2].degree--;
				new_edges.erase(new_edges.begin() + e);
				e--;
				break;
			}
		}
	}

	for(int v{0}; v < rm.size(); ++v)
		for(int i{0}; i < new_vertices.size(); ++i)
			if(new_vertices[i].id == rm[v].id)
				new_vertices.erase(new_vertices.begin() + i);

	std::sort(new_vertices.begin(), new_vertices.end(),
			[] (struct vertex a, struct vertex b) -> bool{
				return (a.id < b.id) ? true : false;
			});

	return Graph(new_vertices, new_edges);
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
		*this = this->removeVerticesCpy({v});
		for(int e{0}; e < edges.size(); ++e)
		{
			if(v.id == edges[e].from || v.id == edges[e].to)
				edges.erase(edges.begin() + e);
		}
	}

	return couverture;
}

std::vector<struct vertex> Graph::get_reachable_vertices(int v)
{
	int rank{0};
	for(; rank < vertices.size(); ++rank)
	{
		if(vertices[rank].id == v)
			break;
	}

	std::vector<struct vertex> reachable;
	for(int i{0}; i < vertices.size(); ++i)
	{
		if(adjacency[rank][i] == 1)
			reachable.push_back(vertices[i].id);
	}
	return reachable;
}

std::vector<struct vertex> Graph::branch()
{
	// si on est sur une feuille, alors fin
	if(edges.empty())
		return vertices;

	// sinon : branchement
	branch_stack.push(*this);
	std::vector<struct vertex> explore{edges[0].from, edges[0].to};
	reachable_stack.push(explore);

	while(!branch_stack.empty())
	{
		// on récupère le sommet de la pile
		auto head = branch_stack.top();
		branch_stack.pop();
		std::vector<struct vertex> nodes = reachable_stack.top();
		reachable_stack.pop();

		// pour chaque sommet atteignable
		// créer le sous graphe correspondant
		// et le mettre dans la pile
		if(!nodes.empty())
		{
			for(auto v : nodes)
			{
				Graph g = head.removeVerticesCpy({v});
				for(auto hpath : head.path)
					g.path.push_back(hpath);
				g.path.push_back(v);
				explore = head.get_reachable_vertices(v.id);
				std::vector<struct vertex> explore_uncovered;
				for(auto e : explore)
				{
					std::vector<struct vertex> tmp = g.get_reachable_vertices(e.id);
					explore_uncovered.insert(explore_uncovered.end(), tmp.begin(), tmp.end());
				}
				explore = explore_uncovered;
				// autre partie non connexe
				if(explore.empty() && !g.getEdges().empty())
				{
					struct vertex u{g.getEdges()[0].from};
					struct vertex v{g.getEdges()[0].to};
					explore = {u,v};
				}
				//else
					//g.path.push_back(v);
				branch_stack.push(g);
				reachable_stack.push(explore);
			}
		}
		else
		{
			// on est sur une feuille
			if(solution.empty() || solution.size() > head.path.size())
				solution = head.path;
		}
	}

	// fin
	return solution;
}

std::vector<struct vertex> Graph::branch_bound()
{
	// si on est sur une feuille, alors fin
	if(edges.empty())
		return vertices;

	// sinon : branchement
	branch_stack.push(*this);
	std::vector<struct vertex> explore{edges[0].from, edges[0].to};
	reachable_stack.push(explore);

	// calcul d'un couplage et d'une borne inf
	std::function lower_limit = [] (Graph & g)
	{
		double n{g.getVertices().size()};
		double m{g.getEdges().size()};
		std::vector<struct vertex> couplage = g.algo_couplage();
		double b1{std::ceil(m/g.getMaxDegreeVertex().degree)};
		double b2{couplage.size() / 2.0};
		double b3{(2.0*n-1.0 - sqrt((2.0*n-1.0)*(2.0*n-1)-8.0*m))/2.0};
		return std::max(b1, std::max(b2, b3));
	};

	borne_inf = lower_limit(*this);
	borne_sup = algo_couplage().size();

	while(!branch_stack.empty())
	{
		// on récupère le sommet de la pile
		auto head = branch_stack.top();
		branch_stack.pop();
		std::vector<struct vertex> nodes = reachable_stack.top();
		reachable_stack.pop();

		// pour chaque sommet atteignable
		// créer le sous graphe correspondant
		// et le mettre dans la pile (si meilleure borne sup et borne inf)
		if(!nodes.empty())
		{
			Graph subgraph;
			std::vector<struct vertex> subGraphExplore;
			for(auto v : nodes)
			{
				Graph g = head.removeVerticesCpy({v});
				for(auto hpath : head.path)
					g.path.push_back(hpath);
				g.path.push_back(v);
				explore = head.get_reachable_vertices(v.id);
				std::vector<struct vertex> explore_uncovered;
				for(auto e : explore)
				{
					std::vector<struct vertex> tmp = g.get_reachable_vertices(e.id);
					explore_uncovered.insert(explore_uncovered.end(), tmp.begin(), tmp.end());
				}
				explore = explore_uncovered;
				// autre partie non connexe
				if(explore.empty() && !g.getEdges().empty())
				{
					struct vertex u{g.getEdges()[0].from};
					struct vertex v{g.getEdges()[0].to};
					explore = {u,v};
				}
				//else
					//g.path.push_back(v);
				// calcul d'une solution réalisable pour
				// le sous graphe g et d'une borne inf
				if(!g.getVertices().empty())
				{
					double binf{lower_limit(g)};
					double bsup{g.algo_couplage().size()};
					// élagage
					if(bsup <= borne_sup && binf <= borne_inf)
					{
						borne_sup = bsup;
						borne_inf = binf;
						subgraph = g;
						subGraphExplore = explore;
					}
				}
			}
			branch_stack.push(subgraph);
			reachable_stack.push(subGraphExplore);
		}
		else
		{
			// on est sur une feuille/des feuilles
			if(solution.empty() || solution.size() > head.path.size())
				solution = head.path;
		}
	}

	// fin
	return solution;
}

std::vector<struct vertex> Graph::branch_bound_v2()
{
	// si on est sur une feuille, alors fin
	if(edges.empty())
		return vertices;

	// sinon : branchement
	branch_stack.push(*this);
	std::vector<struct vertex> explore{edges[0].from, edges[0].to};
	reachable_stack.push(explore);

	// calcul d'un couplage et d'une borne inf
	std::function lower_limit = [] (Graph & g)
	{
		double n{g.getVertices().size()};
		double m{g.getEdges().size()};
		std::vector<struct vertex> couplage = g.algo_couplage();
		double b1{std::ceil(m/g.getMaxDegreeVertex().degree)};
		double b2{couplage.size() / 2.0};
		double b3{(2.0*n-1.0 - sqrt((2.0*n-1.0)*(2.0*n-1)-8.0*m))/2.0};
		return std::max(b1, std::max(b2, b3));
	};

	borne_inf = lower_limit(*this);
	borne_sup = algo_couplage().size();

	while(!branch_stack.empty())
	{
		// on récupère le sommet de la pile
		auto head = branch_stack.top();
		branch_stack.pop();
		std::vector<struct vertex> nodes = reachable_stack.top();
		reachable_stack.pop();

		// pour chaque sommet atteignable
		// créer le sous graphe correspondant
		// et le mettre dans la pile (si meilleure borne sup et borne inf)
		if(!nodes.empty() && !head.edges.empty())
		{
			int id_vertex_branch{-1};
			Graph subgraph;
			std::vector<struct vertex> subGraphExplore;
			for(auto v : nodes)
			{
				Graph g = head.removeVerticesCpy({v});
				explore = head.get_reachable_vertices(v.id);
				std::vector<struct vertex> explore_uncovered;
				for(auto e : explore)
				{
					std::vector<struct vertex> tmp = g.get_reachable_vertices(e.id);
					explore_uncovered.insert(explore_uncovered.end(), tmp.begin(), tmp.end());
				}
				explore = explore_uncovered;
				// autre partie non connexe
				if(explore.empty() && !g.getEdges().empty())
				{
					struct vertex u{g.getEdges()[0].from};
					struct vertex v{g.getEdges()[0].to};
					explore = {u,v};
				}
				// calcul d'une solution réalisable pour
				// le sous graphe g et d'une borne inf
				if(!g.getVertices().empty() && !g.edges.empty())
				{
					double binf{lower_limit(g)};
					double bsup{g.algo_couplage().size()};
					// élagage
					if(bsup <= borne_sup && binf <= borne_inf)
					{
						borne_sup = bsup;
						borne_inf = binf;
						subgraph = g;
						subGraphExplore = explore;
						id_vertex_branch = v.id;
					}
				}
			}
			// on supprime du graph tous les sommets
			// voisins des sommets sur lesquels
			// on a pas fait de branchement
			std::vector<struct vertex> neighbors;
			for(auto v : nodes)
			{
				if(v.id != id_vertex_branch)
				{
					std::vector<struct vertex> tmp = head.get_reachable_vertices(v.id);
					neighbors.insert(neighbors.end(), tmp.begin(), tmp.end());
				}
			}
			subgraph = head.removeVerticesCpy(neighbors);
			// mise à jour du chemin sur un noeud
			subgraph.path.insert(subgraph.path.end(), head.path.begin(), head.path.end());
			subgraph.path.insert(subgraph.path.end(), neighbors.begin(), neighbors.end());

			// redéfinition des voisins à explorer
			bool match{false};
			for(int i{0}; i < subGraphExplore.size(); ++i)
			{
				for(int j{0}; j < subgraph.vertices.size(); ++j)
				{
					if(subgraph.vertices[j].id == subGraphExplore[i].id)
					{
						match = true;
						break;
					}
				}
				if(!match)
					subGraphExplore.erase(subGraphExplore.begin() + i--);
				match = false;
			}

			// on empile
			branch_stack.push(subgraph);
			reachable_stack.push(subGraphExplore);
		}
		else
		{
			// on est sur une feuille/des feuilles
			if(solution.empty() || solution.size() > head.path.size())
				solution = head.path;
		}
	}

	// fin
	return solution;
}

std::vector<struct vertex> Graph::branch_bound_v3()
{
	// si on est sur une feuille, alors fin
	if(edges.empty())
		return vertices;

	// sinon : branchement
	branch_stack.push(*this);
	std::vector<struct vertex> explore{edges[0].from, edges[0].to};
	reachable_stack.push(explore);

	// calcul d'un couplage et d'une borne inf
	std::function lower_limit = [] (Graph & g)
	{
		double n{g.getVertices().size()};
		double m{g.getEdges().size()};
		std::vector<struct vertex> couplage = g.algo_couplage();
		double b1{std::ceil(m/g.getMaxDegreeVertex().degree)};
		double b2{couplage.size() / 2.0};
		double b3{(2.0*n-1.0 - sqrt((2.0*n-1.0)*(2.0*n-1)-8.0*m))/2.0};
		return std::max(b1, std::max(b2, b3));
	};

	borne_inf = lower_limit(*this);
	borne_sup = algo_couplage().size();

	while(!branch_stack.empty())
	{
		// on récupère le sommet de la pile
		auto head = branch_stack.top();
		branch_stack.pop();
		std::vector<struct vertex> nodes = reachable_stack.top();
		reachable_stack.pop();

		// pour chaque sommet atteignable
		// créer le sous graphe correspondant
		// et le mettre dans la pile (si meilleure borne sup et borne inf)
		if(!nodes.empty() && !head.edges.empty())
		{
			int id_vertex_branch{-1};
			Graph subgraph;
			std::vector<struct vertex> subGraphExplore;
			int degreeMIN{std::numeric_limits<int>::digits};
			int cpt{0};
			for(auto v : nodes)
			{
				if(v.degree < degreeMIN)
				{
					degreeMIN = v.degree;
					id_vertex_branch = cpt; 
				}
				cpt++;
			}
			{
				Graph g = head.removeVerticesCpy({nodes[id_vertex_branch]});
				explore = head.get_reachable_vertices(nodes[id_vertex_branch].id);
				std::vector<struct vertex> explore_uncovered;
				for(auto e : explore)
				{
					std::vector<struct vertex> tmp = g.get_reachable_vertices(e.id);
					explore_uncovered.insert(explore_uncovered.end(), tmp.begin(), tmp.end());
				}
				explore = explore_uncovered;
				// autre partie non connexe
				if(explore.empty() && !g.getEdges().empty())
				{
					struct vertex u{g.getEdges()[0].from};
					struct vertex v{g.getEdges()[0].to};
					explore = {u,v};
				}
				// calcul d'une solution réalisable pour
				// le sous graphe g et d'une borne inf
				if(!g.getVertices().empty() && !g.edges.empty())
				{
					double binf{lower_limit(g)};
					double bsup{g.algo_couplage().size()};
					// élagage
					if(bsup <= borne_sup && binf <= borne_inf)
					{
						borne_sup = bsup;
						borne_inf = binf;
						subgraph = g;
						subGraphExplore = explore;
					}
				}
			}
			// on supprime du graph tous les sommets
			// voisins des sommets sur lesquels
			// on a pas fait de branchement
			std::vector<struct vertex> neighbors;
			for(auto v : nodes)
			{
				if(v.id != id_vertex_branch)
				{
					std::vector<struct vertex> tmp = head.get_reachable_vertices(v.id);
					neighbors.insert(neighbors.end(), tmp.begin(), tmp.end());
				}
			}
			subgraph = head.removeVerticesCpy(neighbors);
			// mise à jour du chemin sur un noeud
			subgraph.path.insert(subgraph.path.end(), head.path.begin(), head.path.end());
			subgraph.path.insert(subgraph.path.end(), neighbors.begin(), neighbors.end());

			// redéfinition des voisins à explorer
			bool match{false};
			for(int i{0}; i < subGraphExplore.size(); ++i)
			{
				for(int j{0}; j < subgraph.vertices.size(); ++j)
				{
					if(subgraph.vertices[j].id == subGraphExplore[i].id)
					{
						match = true;
						break;
					}
				}
				if(!match)
					subGraphExplore.erase(subGraphExplore.begin() + i--);
				match = false;
			}

			// on empile
			branch_stack.push(subgraph);
			reachable_stack.push(subGraphExplore);
		}
		else
		{
			// on est sur une feuille/des feuilles
			if(solution.empty() || solution.size() > head.path.size())
				solution = head.path;
		}
	}

	// fin
	return solution;
}
