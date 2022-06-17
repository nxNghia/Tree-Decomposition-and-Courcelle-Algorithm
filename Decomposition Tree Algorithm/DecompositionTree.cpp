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
			for (auto x : this -> vertices)
			{
				cout << x.first << endl;
				this -> fillIn[x.first] = findFillIn (x.first, x.second);
			}

			// for (auto x : this -> vertices)
			// {
			// 	cout << x.first << ": ";
			// 	for (auto y : x.second)
			// 	{
			// 		cout << y << ' ';
			// 	}
			// 	cout << endl;
			// }
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

			for (int i = 0; i < this -> fillIn.size(); ++i)
			{
				if (marked.count(i) != 0)
					continue;
				else
					result = i;

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

			while (marked.size() != this -> vertices.size() && count != 10)
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
			for (auto x = other.rbegin(); x != other.rend(); ++x)
				this -> block.push_back(*x);
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

			if (parent == NULL)
			{
				this -> root = new dNode(parent, newNode, other);
			} else {
				dNode* newdNode = new dNode(parent, newNode, other);

				parent -> child.push_back(newdNode);
			}
		}

		void createDotFile ()
		{
			dNode* tree = this -> root;
			ofstream file;
			file.open("result.dot");

			file << "graph result {" << endl;

			queue<dNode*> q;

			q.push(tree);

			while (!q.empty())
			{
				string line;
				line += "	";
				line.append(1, (char)('A' + q.front() -> id));
				line += "[label=";
				if (!q.front() -> block.empty())
					line += "\"";
				for (auto x : q.front() -> block)
				{
					line.append(1, (char)('A' + x));
					line += " ";
				}

				line.pop_back();

				line += "\"]";

				file << line << ";" << endl;

				line.clear();

				for (auto x : q.front() -> child)
				{

					line += "	";
					line.append(1, (char)(q.front() -> id + 'A'));
					line += " -- ";
					line.append(1, (char)(x -> id + 'A'));
					file << line << ";" << endl;
					line.clear();
					q.push(x);
				}

				q.pop();
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

				existed.push_back(top);
				s.pop();
			}

			// this -> createDotFile();
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

	Graph* graph = new Graph();

	// while (f >> index)
	// {
	// 	if (num == 0)
	// 	{
	// 		row = index;

	// 		graph -> addVertice(row, -1);

	// 		f >> num;
	// 	}else{
	// 		--num;

	// 		graph -> addVertice(index, row);
	// 	}
	// }

	while (!f.eof())
	{
		int node1;
		int node2;
		f >> node1 >> node2;

		cout << node1 << ' ' << node2 << endl;
		graph -> addVertice(node1, node2);
		graph -> addVertice(node2, node1);

	}

	f.close();

	// cout << graph -> vertices.size() << endl;

	graph -> createFillIn();
	// graph -> createEliminationOrder();

	dTree* tree = new dTree(&graph);
	// tree -> createCompositionTree();
}