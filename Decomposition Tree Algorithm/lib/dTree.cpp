#include "dTree.h"
#include "graph.h"
#include "utils.h"
#include <vector>
#include <queue>
#include <fstream>
#include <iostream>
#include <stack>
#include <string>

dTree::dTree (Graph** graph)
{
	this -> graph = graph;
	this -> minWidth = 0;
}

void dTree::addVertice (int p, int newNode, std::vector<int> other)
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
		this -> roots.push_back(newRoot);
	} else {
		dNode* newdNode = new dNode(parent, newNode, other);

		parent -> child.push_back(newdNode);
	}
}

void dTree::createDotFile ()
{
	std::ofstream file;
	file.open("result.dot");

	file << "graph result {" << std::endl;

	for (auto x : this -> roots)
	{
		dNode* tree = x;
		std::queue<dNode*> q;
		q.push(tree);

		while (!q.empty())
		{
			std::string line;
			line += "	";
			line += std::to_string(q.front() -> id);
			line += "[label=";
			if (!q.front() -> block.empty())
				line += "\"";
			for (auto x : q.front() -> block)
			{
				line += std::to_string(x);
				line += " ";
			}

			line.pop_back();
			
			if (q.front() -> block.size() - 1 == this -> minWidth)
			{
				line += "\", color=red]";
			}
			else
				line += "\"]";

			file << line << ";" << std::endl;

			line.clear();

			for (auto x : q.front() -> child)
			{

				line += "	";
				line += std::to_string(q.front() -> id);
				line += " -- ";
				line += std::to_string(x -> id);
				file << line << ";" << std::endl;
				line.clear();
				q.push(x);
			}

			q.pop();
		}				
	}


	file << '}' << std::endl;
}

dNode* dTree::getNode (dNode* root, int index)
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

void dTree::createCompositionTree ()
{
	std::vector<int> existed;
	std::stack<int> s;
	Graph _graph_ = **(this -> graph);

	for (auto x : _graph_.eliminationOrder)
		s.push(x);

	std::vector<int> temp;
	this -> addVertice(-1, s.top(), temp);
	existed.push_back(s.top());
	s.pop();

	// for (auto x : _graph_.revert)
	// 	std::cout << x.first << ": " << x.second << std::endl;

	while (!s.empty())
	{
		int top = s.top();

		std::vector<int> adjencies1 = _graph_.vertices[top];

		std::vector<int> common = findCommon(adjencies1, existed);

		if (common.size() == 0)
		{
			this -> addVertice(-1, top, common);
		} else {
			int minVertice = common[0];
			int minValue = _graph_.revert[minVertice];

			// std::cout << minVertice << '-' << minValue << std::endl;

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

		if (common.size() > this -> minWidth)
		{
			this -> minWidth = common.size();
			this -> minId = top;
		}

		existed.push_back(top);
		s.pop();
	}

	std::cout << "Tree width: " << this -> minWidth << std::endl;

	this -> createDotFile();
}