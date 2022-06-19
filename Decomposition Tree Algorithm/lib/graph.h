#ifndef _GRAPH_
#define _GRAPH_

#include <vector>
#include <map>
#include <set>

class Graph
{
	public:
		std::map<int, std::vector<int>> vertices;
		std::map<int, int> fillIn;
		std::map<int, std::vector<int>> k;
		std::map<int, int> revert;
		std::vector<int> eliminationOrder;

		void addVertice (int index, int parent);
		std::vector<int> getVertex (int index);
		void traversal (int index);
		int findFillIn (int idnex, std::vector<int> adjencies);
		void createFillIn ();
		void showFillIn ();
		void showEliminationOrder ();
		int getMin (std::set<int> marked);
		void createEliminationOrder ();
};

#endif