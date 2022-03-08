#include <iostream>
#include <vector>
#include <fstream>
#include <map>

using namespace std;

class Tree
{
	public:
		map<int, vector<int>> vertices;
		map<int, int> vertices_weight;
		map<int, map<int, bool>> trace;
		vector<int> max_in;
		vector<int> max_out;
		vector<int> independentSet;
		bool in;

		void addVertice (int index, int parent, int weight)
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
					// this -> vertices_weight[parent] = weight;
					this -> vertices_weight.insert(pair<int, int> (index, weight));
					map<int, bool> _trace;
					this -> trace.insert(pair<int, map<int, bool>> (index, _trace));
				}
				max_in.push_back(0);
				max_out.push_back(0);
			}
		}

		void LRN (int root)
		{
			if (this -> vertices[root].size() == 0)
			{
				cout << root << endl;
			}else{
				for (auto child : this -> vertices[root])
				{
					LRN (child);
				}
				cout << root << endl;
			}
		}

		int maxWeightIndependentSet (int root)
		{
			map<int, bool> trace1;
			map<int, bool> trace2;

			if (this -> vertices[root].size() == 0)
			{
				this -> max_out[root] = 0;
				this -> max_in[root] = this -> vertices_weight[root];
			}else{
				for (auto child : this -> vertices[root])
				{
					maxWeightIndependentSet(child);

					trace1.insert(pair<int, bool> (child, (bool)((this -> max_in[child]) > (this -> max_out[child]))));
					trace2.insert(pair<int, bool> (child, true));

					this -> max_out[root] += max(this -> max_out[child], this -> max_in[child]);
					this -> max_in[root] += this -> max_out[child];
				}

				this -> max_in[root] += this -> vertices_weight[root];
			}

			this -> in = this -> max_in[root] > this -> max_out[root];

			this -> trace[root].swap(trace1);

			return max(this -> max_out[root], this -> max_in[root]);
		}

		void getIndependentSet(int root, bool select)
		{
			if (select)
				this -> independentSet.push_back(root);

			for (map<int, bool>::iterator it = this -> trace[root].begin(); it != this -> trace[root].end(); ++it)
			{
				getIndependentSet(it -> first, it -> second && !select);
			}
		}
};

int main()
{
	ifstream f("input.txt");
	int index;
	int num = 0;
	int row;

	Tree* tree = new Tree();

	while (f >> index)
	{
		if (num == 0)
		{
			row = index;

			f >> index;

			tree -> addVertice(row, -1, index);

			f >> num;
		}else{
			--num;
			
			tree -> addVertice(index, row, 0);
		}
	}

	f.close();

	cout << tree -> maxWeightIndependentSet(0) << endl;

	tree -> getIndependentSet(0, tree -> in);

	for (auto x : tree -> independentSet)
		cout << x << ' ';
	cout << endl;
}