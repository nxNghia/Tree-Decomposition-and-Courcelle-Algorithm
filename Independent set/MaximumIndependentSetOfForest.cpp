#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

class Edge
{
	public:
		int u;
		int v;

		Edge(int u, int v)
		{
			this -> u = u;
			this -> v = v;
		}

		bool contain(int k)
		{
			return this -> u == k || this -> v == k;
		}
};

vector<int> IndependentSet(vector<vector<int>> forest, vector<int> degree, vector<Edge*> edges)
{
	vector<int> result;
	vector<bool> checks (forest.size(), false);

	while (true)
	{
		// Calculate edges

		if (edges.size() == 0)
			break;

		// Find leaf

		int leaf = -1;

		for (auto edge : edges)
		{
			if (degree[edge -> u] == 0)
			{
				leaf = edge -> u;
				break;
			}

			if (degree[edge -> v] == 0)
			{
				leaf = edge -> v;
				break;
			}
		}

		// Delete u and v

		int u = 0;

		for (int i = 0; i < edges.size(); ++i)
		{
			if (edges[i] -> contain(leaf))
			{
				checks[edges[i] -> u] = true;
				checks[edges[i] -> v] = true;

				u = edges[i] -> v == leaf ? edges[i] -> u : edges[i] -> v;

				result.push_back(leaf);

				edges.erase(edges.begin() + i);
				break;
			}
		}

		// Update vertice degree
		// Delete incident edge

		for (int i = edges.size() - 1; i >= 0; --i)
		{
			if (edges[i] -> contain(u))
			{
				edges.erase(edges.begin() + i);
			}
		}

		for (auto adjency : forest[u])
		{
			--degree[adjency];
		}
	}

	for (int i = 0; i < forest.size(); ++i)
	{
		if (!checks[i])
		{
			result.push_back(i);
		}
	}

	return result;
}

vector<int> nodeDegree(vector<vector<int>> forest)
{
	vector<int> degree;
	vector<bool> checks (forest.size(), false);
	int index = 0;

	for (int i = 0; i < forest.size(); ++i)
	{
		degree.push_back(0);
	}

	for (auto node : forest)
	{
		for (auto child : node)
		{
			if (!checks[child])
			{
				++degree[index];
			}
		}
		checks[index] = true;
		++index;
	}

	return degree;
}

int main(int argc, const char** argv)
{
	ifstream f("input.txt");
	vector<Edge*> edges;
	vector<vector<int>> forest;
	int node;
	int num = 0;
	int row;

	while (f >> node)
	{
		if (num == 0)
		{
			row = node;

			vector<int> vertice;

			forest.push_back(vertice);

			f >> num;
		}else{
			--num;
			Edge* new_edge = new Edge(row, node);
			edges.push_back(new_edge);

			forest.back().push_back(node);
		}
	}

	f.close();

	vector<int> degree = nodeDegree(forest);
	vector<int> independentSet = IndependentSet(forest, degree, edges);

	for (auto vertice : independentSet)
	{
		cout << vertice << ' ';
	}
	cout << endl;
}