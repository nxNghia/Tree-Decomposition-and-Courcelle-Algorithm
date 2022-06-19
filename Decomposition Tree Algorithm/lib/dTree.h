#ifndef _DTREE_
#define _DTREE_

#include "dNode.h"
#include "graph.h"
#include <vector>

class dTree
{
	public:
		Graph** graph;
		std::vector<dNode*> roots;
		int minWidth;
		int minId;

		dTree (Graph** graph);
		void addVertice (int p, int newNode, std::vector<int> other);
		void createDotFile ();
		dNode* getNode (dNode* root, int index);
		void createCompositionTree ();
};

#endif