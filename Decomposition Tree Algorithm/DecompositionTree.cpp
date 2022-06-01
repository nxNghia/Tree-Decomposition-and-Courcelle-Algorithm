#include <iostream>
#include <fstream>
#include <map>
#include <vector>
#include <queue>
#include <set>

using namespace std;

class Graph
{
	public:
		map<int, vector<int>> vertices;
		map<int, int> fillIn;

		void addVertice (int index, int parent)
		{
			if (parent != -1)
			{
				if (this -> vertices.count(parent) != 0)
				{
					this -> vertices[parent].push_back(index);
				}else{
					vector<int> childs;
					this -> vertices[parent] = childs;
					this -> vertices[parent].push_back(index);
				}
			}else{
				if (this -> vertices.count(index) == 0)
				{
					vector<int> childs;
					this -> vertices[index] = childs;
				}
			}
		}

		vector<int> getVertex (int index)
		{
			return (this -> vertices[index]);
		}

		void traversal (int index)
		{
			queue<int> _queue;
			set<int> marked;

			vector<int> _index = this -> getVertex(index);
			marked.insert(index);

			if (!_index.empty())
			{
				_queue.push(index);

				while (!_queue.empty())
				{
					int top = _queue.front();
					cout << top << endl;
					_index = this -> getVertex(top);
					for (auto x : _index)
					{
						if (marked.count(x) == 0)
						{
							_queue.push(x);
							marked.insert(x);
						}
					}
					_queue.pop();
				}
			}
		}

		vector<int> findCommon (vector<int> a, vector<int> b)
		{
			vector<int> result;

			while(!a.empty() && !b.empty())
			{
				if (a.back() == b.back())
				{
					result.push_back(a.back());
					a.pop_back();
					b.pop_back();
				} else {
					if (a.back() > b.back())
					{
						a.pop_back();
					}else{
						b.pop_back();
					}
				}
			}

			return result;
		}

		int findFillIn (vector<int> adjencies)
		{
			int edges = 0;

			for (int i = 0; i < adjencies.size(); i++)
			{
				vector<int> _adjencies_ = this -> getVertex(adjencies[i]);
				vector<int> common = this -> findCommon(adjencies, _adjencies_);

				edges += common.size();

			}

			return adjencies.size() * (adjencies.size() - 1) / 2 - edges / 2;
		}

		void createFillIn ()
		{
			for (int i = 0; i < this -> vertices.size(); ++i)
			{
				this -> fillIn[i] = findFillIn (this -> vertices[i]);
			}
		}

		void showFillIn ()
		{
			for (int i = 0; i < this -> fillIn.size(); ++i)
			{
				cout << (char)(i + 'A') << ' ' << this -> fillIn[i] << endl;
			}
		}
};

int main() {
	ifstream f("input.txt");
	int index;
	int num = 0;
	int row;

	Graph* graph = new Graph();

	while (f >> index)
	{
		if (num == 0)
		{
			row = index;

			graph -> addVertice(row, -1);

			f >> num;
		}else{
			--num;

			graph -> addVertice(index, row);
		}
	}

	f.close();

	graph -> createFillIn();
	cout << endl;
	graph -> showFillIn();

	// int a[5] = {0, 2, 4, 5, 6};
	// int b[4] = {0, 1, 3, 4};

	// int x = 0;
	// int count = 0;

	// for (int i = 0; i < 5; ++i)
	// {
	// 	cout << a[i] << ' ' << b[x] << endl;
	// 	if (a[i] > b[x])
	// 	{
	// 		++x;
	// 		--i;
	// 		if (x > 4)
	// 			break;
	// 	}else{
	// 		if (a[i] == b[x])
	// 		{
	// 			cout << " found " << endl;
	// 			++count;
	// 		}
	// 	}
	// }

	// cout << count << endl;

	// graph -> traversal(0);
}