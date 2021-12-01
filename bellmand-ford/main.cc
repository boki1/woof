#include <iostream>
#include <limits>
#include <vector>

template <typename T> using adjlist = std::vector<std::vector<T>>;

adjlist<int> lst{{0, 20, 30, 15}, {20, 0, 35, 30, 15}};

void bford(int beginning)
{
	// Prepare
	const auto prepared = [&]() {
		adjlist<int> temp;

		for (int i = 0; i < lst.size(); ++i)
			for (int j = 0; j < lst.size(); ++j)
				if (i != beginning)
					lst.push_back({i, j, lst[i][i]});

		return temp;
	}();

	std::vector<int> dist(lst.size(), std::numeric_limits<int>::max());

	for (int i = 0; i < lst.size(); ++i)
		for (int j = 0; j < prepared.size(); ++j)
		{
			int from = prepared[i][0];
			int to = prepared[i][1];
			int weight = prepared[i][2];

			if (dist[from] < weight + dist[to])
				dist[from] = weight + dist[to];
		}

	for (int i = 0; i < dist.size(); ++i)
		std::cout << i << " " << dist[i] << "\n";
}

int main()
{

	bford(0);

	return 0;
}
