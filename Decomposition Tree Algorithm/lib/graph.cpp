#include "graph.h"
#include "utils.h"
#include <iostream>
#include <vector>
#include <map>
#include <queue>
#include <set>
#include <algorithm>

void Graph::addVertice (int index, int parent)
{
	if (parent != -1)
	{
		if (this -> vertices.count(parent) != 0)
		{
			this -> vertices[parent].push_back(index);
		}else{
			std::vector<int> childs;
			this -> vertices[parent] = childs;
			this -> vertices[parent].push_back(index);
		}
	}else{
		if (this -> vertices.count(index) == 0)
		{
			std::vector<int> childs;
			this -> vertices[index] = childs;
		}
	}
}

std::vector<int> Graph::getVertex (int index)
{
	return this -> vertices[index];
}

void Graph::traversal (int index)
{
	std::queue<int> _queue;
	std::set<int> marked;
	std::vector<int> _index = this -> getVertex(index);

	marked.insert(index);

	if (!_index.empty())
	{
		_queue.push(index);

		while (!_queue.empty())
		{
			int top = _queue.front();
			std::cout << top << std::endl;
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

int Graph::findFillIn (int index, std::vector<int> adjencies)
{
	int edges = 0;

	for (auto x : adjencies)
	{
		std::vector<int> _adjencies_ = this -> getVertex(x);
		std::vector<int> common = findCommon(adjencies, _adjencies_);

		edges += common.size();
		this -> k[x][index] = adjencies.size() - common.size() - 1;
	}

	return adjencies.size() * (adjencies.size() - 1) / 2 - edges / 2;
}

void Graph::createFillIn ()
{
	std::map<int, std::vector<int>>::iterator it;
	it = this -> vertices.end();
	--it;

	for (auto x : this -> vertices)
	{
		std::vector<int> t (it -> first + 1, -1);
		this -> k[x.first] = t;
	}

	for (auto x : this -> vertices)
	{
		this -> fillIn[x.first] = findFillIn(x.first, x.second);
	}
}

void Graph::showFillIn ()
{
	for (int i = 0; i < this -> fillIn.size(); ++i)
	{
		std::cout << (char)(i + 'A') << ' ' << this -> fillIn[i] << std::endl;
	}
}

void Graph::showEliminationOrder ()
{
	for (auto x : this -> eliminationOrder)
	{
		std::cout << (char)(x + 'A') << ' ';
	}
	std::cout << std::endl;
}

int Graph::getMin (std::set<int> marked)
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

void Graph::createEliminationOrder ()
{
	for (auto x : this -> vertices)
		this -> revert[x.first] = 0;

	std::set<int> marked;
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
