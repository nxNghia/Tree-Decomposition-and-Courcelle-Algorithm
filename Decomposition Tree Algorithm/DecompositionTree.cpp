#include <iostream>
#include <fstream>
#include <ctime>
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
	int edges = 0;
	int node1, node2;
	double duration;

	Graph* graph = new Graph();

	while (f >> node1 >> node2)
	{
		graph -> addVertice(node1, node2);
		graph -> addVertice(node2, node1);
		++edges;
	}

	cout << graph -> vertices.size() << " vertices" << endl;
	cout << edges << " edges" << endl;

	f.close();

	clock_t start = clock();
	graph -> createFillIn();
	graph -> showFillIn();
	graph -> createEliminationOrder();

	for (auto x : graph -> eliminationOrder)
		cout << x << ' ';
	cout << endl;


	dTree* tree = new dTree(&graph);
	tree -> createCompositionTree();
	cout << "Time: " << (clock() - start) / (double) CLOCKS_PER_SEC << 's' << endl;
}