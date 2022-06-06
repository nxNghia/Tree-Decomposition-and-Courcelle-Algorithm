#include <iostream>
#include <fstream>
#include <map>
#include <vector>
#include <queue>
#include <set>
#include <stack>
#include <algorithm>

using namespace std;

vector<int> findCommon (vector<int> a, vector<int> b)
{
	vector<int> result;

	sort(a.begin(), a.end());
	sort(b.begin(), b.end());

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

int getMin (vector<int> a)
{
	int min = a[0];

	for (auto x : a)
	{
		if (x < min)
		{
			min = x;
		}
	}

	return min;
}

class Graph
{
	public:
		map<int, vector<int>> vertices;
		map<int, int> fillIn;
		vector<vector<int>> k;
		vector<int> eliminationOrder;
		vector<int> revert;

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

		int findFillIn (int index, vector<int> adjencies)
		{
			int edges = 0;

			for (int i = 0; i < adjencies.size(); i++)
			{
				vector<int> _adjencies_ = this -> getVertex(adjencies[i]);
				vector<int> common = findCommon(adjencies, _adjencies_);

				edges += common.size();

				// cout << (char)(adjencies[i] + 'A') << ' ' << (char)(index + 'A') << ' ' << adjencies.size << endl;

				this -> k[adjencies[i]][index] = adjencies.size() - common.size() - 1;
			}

			return adjencies.size() * (adjencies.size() - 1) / 2 - edges / 2;
		}

		void createFillIn ()
		{
			for (int i = 0; i < this -> vertices.size(); ++i)
			{
				vector<int> t (this -> vertices.size(), -1);
				this -> k.push_back(t);
			}
			for (int i = 0; i < this -> vertices.size(); ++i)
			{
				this -> fillIn[i] = findFillIn (i, this -> vertices[i]);
			}
		}

		void showFillIn ()
		{
			for (int i = 0; i < this -> fillIn.size(); ++i)
			{
				cout << (char)(i + 'A') << ' ' << this -> fillIn[i] << endl;
			}
		}

		void showEliminationOrder ()
		{
			for (auto x : this -> eliminationOrder)
			{
				cout << (char)(x + 'A') << ' ';
			}
			cout << endl;
		}

		int getMin (set<int> marked)
		{
			int result = 0;

			for (int i = 0; i < this -> fillIn.size(); ++i)
			{
				if (this -> fillIn[i] == 0 && marked.count(i) == 0)
					return i;
				if (this -> fillIn[i] <= this -> fillIn[result])
				{
					if (marked.count(i) == 0)
						result = i;
				}
			}

			return result;
		}

		void createEliminationOrder ()
		{
			this -> revert.assign(this -> vertices.size(), 0);
			set<int> marked;
			int count = 0;

			while (marked.size() != this -> vertices.size())
			{
				int minIndex = this -> getMin(marked);

				for (int i = 0; i < this -> vertices.size(); ++i)
				{
					if (this -> k[minIndex][i] != -1 && marked.count(minIndex) == 0)
					{
						this -> fillIn[i] -= this -> k[minIndex][i];

						for (int j = 0; j < this -> vertices.size(); ++j)
						{
							if (this -> k[j][i] != -1 && this -> k[j][minIndex] == -1 && marked.count(j) == 0)
							{
								this -> k[j][i] -= 1;
							}
						}
					}
				}

				this -> revert[minIndex] = count;
				++count;
				this -> eliminationOrder.push_back(minIndex);
				marked.insert(minIndex);
			}
		}
};

class dNode {
	public:
		int id;
		vector<dNode*> child;
		vector<int> block;

		dNode (dNode* p, int id, vector<int> other)
		{
			this -> id = id;
			this -> block.push_back(id);
			for (auto x : other)
				this -> block.push_back(x);
		}
};

class dTree {
	public:
		Graph** graph;
		dNode* root;

		dTree (Graph** graph)
		{
			this -> graph = graph;
		}

		void addVertice (int p, int newNode, vector<int> other)
		{
			dNode* parent = this -> getNode(this -> root, p);

			// cout << (char)(newNode + 'A') << ' ';
			// for (auto x : other)
			// 	cout << (char)(x + 'A') << ' ';
			// cout << endl;

			if (parent == NULL)
			{
				this -> root = new dNode(parent, newNode, other);
			} else {
				dNode* newdNode = new dNode(parent, newNode, other);

				parent -> child.push_back(newdNode);
			}
		}

		dNode* getNode (dNode* root, int index)
		{
			if (root == NULL)
				return NULL;
			if (root -> id == index)
				return root;
			else{
				if (root -> child.size() != 0)
				{
					for (auto x : root -> child)
					{
						dNode* result = this -> getNode(x, index);
						if (result != NULL)
						{
							return result;
						}
					}
				}
			}

			return NULL;
		}

		void createCompositionTree ()
		{
			vector<int> existed;
			stack<int> s;
			Graph _graph_ = **(this -> graph);

			for (auto x : _graph_.eliminationOrder)
				s.push(x);

			vector<int> temp;
			this -> addVertice(-1, s.top(), temp);
			existed.push_back(s.top());
			s.pop();


			int count = 0;

			// cout << "-------------------------------" << endl;

			while (!s.empty() && count != 10)
			{
				int top = s.top();

				// cout << "top: " << (char)(top + 'A') << endl;

				vector<int> adjencies1 = _graph_.vertices[top];

				vector<int> common = findCommon(adjencies1, existed);

				int minVertice = common[0];

				// for (auto x : adjencies1)
				// 	cout << (char)(x + 'A') << ' ';
				// cout << endl;

				// for (auto x : existed)
				// 	cout << (char)(x + 'A') << ' ';
				// cout << endl;

				// cout << "common: ";
				// for (auto x : common)
				// 	cout << (char)(x + 'A') << ' ';
				// cout << endl;

				// cout << (char)(minVertice + 'A') << '-' << (char)(top + 'A') << endl;

				this -> addVertice(minVertice, top, common);
				// cout << "'-------------------------------------------" << endl;

				existed.push_back(top);
				s.pop();
				++count;
			}
		}

		void traversal ()
		{
			queue<dNode*> q;

			q.push(this -> root);

			while (!q.empty())
			{
				for (auto x : q.front() -> block)
				{
					cout << (char)(x + 'A') << ' ';
				}
				cout << endl;

				for (auto x : q.front() -> child)
				{
					q.push(x);
				}

				q.pop();
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

	graph -> createEliminationOrder();

	dTree* tree = new dTree(&graph);

	// graph -> showEliminationOrder();
	tree -> createCompositionTree();

	tree -> traversal();

	// graph -> showFillIn();



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