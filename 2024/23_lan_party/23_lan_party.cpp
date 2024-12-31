#include "../../other/include_everything.h"
import std;

typedef std::set<std::string> adjacency;
typedef std::unordered_map<std::string, adjacency> Graph;
void run_program(std::string input, std::u8string comment);
void read_data(std::string& input_path, Graph& data);
int part_one(Graph& data);
std::string part_two(Graph& data);

int main()
{
	// 1.7066ms  : data in + parsing
	// 633.60µs  : part1
	// 234.63ms  : part2
	run_program(__TEST_PATH, u8"test input");
	run_program(__INPUT_PATH, u8"input day23");
	return 0;
}

void run_program(std::string input, std::u8string comment)
{
	Graph data;
	long long part_one_result{ 0 };
	std::string part_two_result{ 0 };
	TimerUtility input_timer, part1_timer, part2_timer;

	input_timer.startTimer();
	read_data(input, data);
	input_timer.getDuration(u8"input " + comment);

	part1_timer.startTimer();
	part_one_result = part_one(data);
	part1_timer.getDuration(u8"part1");

	part2_timer.startTimer();
	part_two_result = part_two(data);
	part2_timer.getDuration(u8"part2");

	std::println("{0}", part_one_result);
	std::println("{0}", part_two_result);
	std::println();
}

void read_data(std::string& input_path, Graph& data)
{
	std::ifstream input(input_path);
	std::string temp;
	while (std::getline(input, temp))
	{
		std::string left{ temp.begin(), temp.begin() + 2 }, right{ temp.begin() + 3, temp.end() };
		data[left].insert(right);
		data[right].insert(left);
	}

}

int part_one(Graph& data)
{
	int result{ 0 };
	adjacency visited;
	for (auto it{ data.begin() }; it != data.end(); ++it)
	{
		if ((*it).first[0] == 't')
		{
			for (auto it2{ (*it).second.begin() }; it2 != (*it).second.end(); ++it2)
				for (auto it3{ it2 }; it3 != (*it).second.end(); ++it3)
					if (data[(*it2)].find(*it3) != data[(*it2)].end())
					{
						std::vector<std::string> to_sort{ (*it).first, (*it2), (*it3) };
						std::sort(to_sort.begin(), to_sort.end());
						std::string temp_set{ to_sort[0] + to_sort[1] + to_sort[2] };
						if(visited.find(temp_set) == visited.end())
						{
							++result;
							visited.insert(temp_set);
						}
					}
		}
	}

	return result;
}

void bronKerbosch(const adjacency& R, adjacency& P, adjacency& X, const Graph& graph, std::vector<adjacency>& cliques) 
{
	if (P.empty() && X.empty()) 
	{
		cliques.push_back(R);
		return;
	}

	for (auto it = P.begin(); it != P.end();) 
	{
		std::string v = *it;
		adjacency newR = R;
		newR.insert(v);

		adjacency newP;
		adjacency newX;

		for (const auto& neighbor : graph.at(v)) 
		{
			if (P.find(neighbor) != P.end()) 
			{
				newP.insert(neighbor);
			}
			if (X.find(neighbor) != X.end()) 
			{
				newX.insert(neighbor);
			}
		}

		bronKerbosch(newR, newP, newX, graph, cliques);

		it = P.erase(it);
		X.insert(v);
	}
}

std::vector<adjacency> findAllCliques(const Graph& graph) 
{
	std::vector<adjacency> cliques;
	adjacency R;
	adjacency P;
	adjacency X;

	for (const auto& pair : graph) 
	{
		P.insert(pair.first);
	}

	bronKerbosch(R, P, X, graph, cliques);
	return cliques;
}

std::string part_two(Graph& data)
{
	std::vector<adjacency> cliques = findAllCliques(data);

	std::pair<int, adjacency> max_clique;
	max_clique.first = 0;
	for (const auto& clique : cliques) 
	{
		if (clique.size() > max_clique.first)
		{
			max_clique.second = clique;
			max_clique.first = clique.size();
		}
	}
	std::string str_result;
	for (auto it{ max_clique.second.begin() }; it != max_clique.second.end(); ++it)
	{
		str_result += *it + ',';
	}
	str_result.pop_back();


	return str_result;
}