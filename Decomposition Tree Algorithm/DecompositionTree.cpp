#include <iostream>
#include <fstream>
#include <map>
#include <vector>
#include <queue>
#include <set>
#include <stack>
#include <algorithm>
#include <string>

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
		map<int, vector<int>> k;
		vector<int> eliminationOrder;
		map<int, int> revert;

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

			for (auto x : adjencies)
			{
				vector<int> _adjencies_ = this -> getVertex(x);
				vector<int> common = findCommon(adjencies, _adjencies_);

				edges += common.size();
				this -> k[x][index] = adjencies.size() - common.size() - 1;
			}

			return adjencies.size() * (adjencies.size() - 1) / 2 - edges / 2;
		}

		void createFillIn ()
		{
			map<int, vector<int>>::iterator it;
			it = this -> vertices.end();
			--it;

			for (auto x : this -> vertices)
			{
				vector<int> t (it -> first + 1, -1);
				this -> k[x.first] = t;
			}

			for (auto x : this -> vertices)
			{
				this -> fillIn[x.first] = findFillIn(x.first, x.second);
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
			int result = -1;

			for (auto x : this -> fillIn)
			{
				if (marked.count(x.first) != 0)
					continue;
				else
					result = x.first;

				if (x.second == 0 && marked.count(x.first) == 0)
					return x.first;

				if (x.second <= this -> fillIn[result])
				{
					if (marked.count(x.first) == 0)
						result = x.first;
				}
			}

			return result;
		}

		void createEliminationOrder ()
		{
			for (auto x : this -> vertices)
				this -> revert[x.first] = 0;

			set<int> marked;
			int count = 0;

			while (marked.size() != this -> vertices.size())
			{
				int minIndex = this -> getMin(marked);

				for (auto x : this -> vertices)
				{
					if (this -> k[minIndex][x.first] != -1 && marked.count(minIndex) == 0)
					{
						this -> fillIn[x.first] -= this -> k[minIndex][x.first];

						for (auto y : this -> vertices)
						{
							if (this -> k[y.first][x.first] != -1 && this -> k[y.first][minIndex] == -1 && marked.count(y.first) == 0)
								this -> k[y.first][x.first] -= 1;
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
			for (auto x = other.rbegin(); x != other.rend(); ++x)
				this -> block.push_back(*x);
		}
};

class dTree {
	public:
		Graph** graph;
		dNode* root;
		vector<dNode*> roots;

		dTree (Graph** graph)
		{
			this -> graph = graph;
		}

		void addVertice (int p, int newNode, vector<int> other)
		{
			dNode* parent = NULL;

			for (auto x : this -> roots)
			{
				parent = this -> getNode(x, p);
				if (parent != NULL)
					break;
			}

			if (parent == NULL)
			{
				dNode* newRoot = new dNode(parent, newNode, other);
				this -> root = newRoot;
				this -> roots.push_back(newRoot);
			} else {
				dNode* newdNode = new dNode(parent, newNode, other);

				parent -> child.push_back(newdNode);
			}
		}

		void createDotFile ()
		{
			ofstream file;
			file.open("result.dot");

			file << "graph result {" << endl;


			for (auto x : this -> roots)
			{
				dNode* tree = x;
				queue<dNode*> q;
				q.push(tree);

				while (!q.empty())
				{
					string line;
					line += "	";
					line += to_string(q.front() -> id);
					line += "[label=";
					if (!q.front() -> block.empty())
						line += "\"";
					for (auto x : q.front() -> block)
					{
						line += to_string(x);
						line += " ";
					}

					line.pop_back();

					line += "\"]";

					file << line << ";" << endl;

					line.clear();

					for (auto x : q.front() -> child)
					{

						line += "	";
						line += to_string(q.front() -> id);
						line += " -- ";
						line += to_string(x -> id);
						file << line << ";" << endl;
						line.clear();
						q.push(x);
					}

					q.pop();
				}				
			}


			file << '}' << endl;
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
			int minWidth = 1;

			for (auto x : _graph_.eliminationOrder)
				s.push(x);

			vector<int> temp;
			this -> addVertice(-1, s.top(), temp);
			existed.push_back(s.top());
			s.pop();

			while (!s.empty())
			{
				int top = s.top();

				vector<int> adjencies1 = _graph_.vertices[top];

				vector<int> common = findCommon(adjencies1, existed);

				if (common.size() == 0)
				{
					this -> addVertice(-1, top, common);
				} else {
					int minVertice = common[0];
					int minValue = _graph_.revert[minVertice];

					for (auto x : common)
					{
						if (minValue > _graph_.revert[x])
						{
							minValue = _graph_.revert[x];
							minVertice = x;
						}
					}

					this -> addVertice(minVertice, top, common);
				}

				if (common.size() > minWidth)
				{
					for (auto x : common)
						cout << x << ' ';
					cout << endl;
					cout << common.size() << endl;
					cout << "-----------------------" << endl;
					minWidth = common.size();
				}

				existed.push_back(top);
				s.pop();
			}

			cout << minWidth << endl;

			this -> createDotFile();
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
	ifstream f("input3.txt");
	int index;
	int num = 0;
	int row;
	int node1, node2;

	Graph* graph = new Graph();

	while (!f.eof())
	{
		if (f.eof())
			break;

		f >> node1 >> node2;
		graph -> addVertice(node1, node2);
		graph -> addVertice(node2, node1);

	}

	f.close();

	graph -> createFillIn();
	graph -> createEliminationOrder();

	dTree* tree = new dTree(&graph);
	tree -> createCompositionTree();
}