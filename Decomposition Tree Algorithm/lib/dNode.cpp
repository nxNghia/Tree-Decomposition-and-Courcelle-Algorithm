#include "dNode.h"
#include <vector>

dNode::dNode (dNode* p, int id, std::vector<int> other)
{
	this -> id = id;
	this -> block.push_back(id);
	for (auto x = other.rbegin(); x != other.rend(); ++x)
		this -> block.push_back(*x);
}