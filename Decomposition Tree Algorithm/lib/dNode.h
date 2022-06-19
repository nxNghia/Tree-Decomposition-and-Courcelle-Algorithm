#ifndef _DNODE_
#define _DNODE_

#include <vector>

class dNode
{
	public:
		int id;
		std::vector<dNode*> child;
		std::vector<int> block;

		dNode (dNode* p, int id, std::vector<int> other);
};

#endif