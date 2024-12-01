import std;

void read_data(std::vector<int>& left, std::vector<int>& right);
int part_one(std::vector<int>& left, std::vector<int>& right);
int part_two(std::vector<int>& left, std::vector<int>& right);

int main()
{
	std::vector<int> left, right;
	read_data(left, right);
	std::println("{0}", part_one(left, right));
	std::println("{0}", part_two(left, right));

	return 0;
}

void read_data(std::vector<int>& left, std::vector<int>& right)
{
	std::ifstream input("./input.txt");
	int temp;
	while (!input.eof())
	{
		input >> temp;
		left.emplace_back(temp);
		input >> temp;
		right.emplace_back(temp);
	}
}

int part_one(std::vector<int>& left, std::vector<int>& right)
{
	int result{ 0 };
	std::sort(left.begin(), left.end());
	std::sort(right.begin(), right.end());
	for (int i{ 0 }; i != left.size(); ++i)
	{
		result += std::abs(left[i] - right[i]);
	}

	return result;
}

int part_two(std::vector<int>& left, std::vector<int>& right)
{
	int result{ 0 };
	std::map<int, int> similarity_map;
	for (int i{ 0 }; i != left.size(); ++i)
	{
		if (find(left.begin(), left.end(), right[i]) != left.end())
		{
			++similarity_map[right[i]];
		}
	}
	for (int i{ 0 }; i != left.size(); ++i)
	{
		if (similarity_map.find(left[i]) != similarity_map.end())
		{
			result += left[i] * similarity_map[left[i]];
		}
	}

	return result;
}