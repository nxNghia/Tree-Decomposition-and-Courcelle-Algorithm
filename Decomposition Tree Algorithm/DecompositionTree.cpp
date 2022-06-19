#include <iostream>
#include <fstream>
#include "./lib/graph.h"
#include "./lib/dTree.h"

using namespace std;

int main(int argc, const char** argv) {
	if (argc != 2)
	{
		std::cout << "Insufficient parameters" << std::endl;
		return 0;
	}

	ifstream f(argv[1]);

	if (!f.good())
	{
		std::cout << "File does not exist" << std::endl;
		return 0;
	}

	int index;
	int num = 0;
	int row;
	int node1, node2;

	Graph* graph = new Graph();

	while (f >> node1 >> node2)
	{
		graph -> addVertice(node1, node2);
		graph -> addVertice(node2, node1);
	}

	f.close();

	graph -> createFillIn();
	graph -> createEliminationOrder();

	dTree* tree = new dTree(&graph);
	tree -> createCompositionTree();
}