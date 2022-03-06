#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <set>

using namespace std;

int _k;
int _vertices;

class edge
{
	public:
		int u;
		int v;

		edge(int u, int v)
		{
			this -> u = u;
			this -> v = v;
		}

		void printEdge()
		{
			cout << '(' << this -> u << '-' << this -> v << ')' << endl;
		}

		bool contain(int k)
		{
			return this -> u == k || this -> v == k;
		}
};

bool smallVertexCover (vector<edge*> edges, set<int> checks, int k)
{
	// calculate edges
	int count = 0;
	for (auto edge : edges)
	{
		if (!checks.count(edge -> u) && !checks.count(edge -> v))
			++count;
	}

	if (count == 0 && checks.size() == _k)
	{
		for (auto x : checks)
			cout << x << ' ';
		cout << endl;
		return true;
	}

	if (count / 2 > k * (_vertices - checks.size()))
		return false;

	for (auto edge : edges)
	{
		if (!checks.count(edge -> u) && !checks.count(edge -> v))
		{
			checks.insert(edge -> u);

			if (smallVertexCover(edges, checks, k - 1))
			{
				return true;
			}
			else{
				checks.erase(edge -> u);
				checks.insert(edge -> v);
				bool res = smallVertexCover(edges, checks, k - 1);

				checks.erase(edge -> v);

				return res;
			}
		}
	}

	return false;
}

int main(int argc, const char** argv)
{
	ifstream f("input.txt");
	vector<edge*> edges;
	set<int> checks;
	int node;
	int num = 0;
	_k = atoi(argv[1]);
	int row;

	while (f >> node)
	{
		if (num == 0)
		{
			++_vertices;
			row = node;

			f >> num;
		}else{
			--num;
			edge* new_edge = new edge(row, node);
			edges.push_back(new_edge);
		}
	}

	f.close();

	string result = smallVertexCover(edges, checks, _k) ? "TRUE" : "FALSE";

	cout << result << endl;
}