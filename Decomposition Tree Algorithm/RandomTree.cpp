#include <iostream>
#include <fstream>
#include <time.h>
#include <stdlib.h>
#include <cstring>

using namespace std;

const string outputFile = "input.txt";
const string outputDotFile = "input.dot";

int main(int argc, const char* argv[])
{
	if (argc != 3)
	{
		cout << "Insufficient parameters" << endl;
		return 0;
	}

	if (stod(argv[2]) > 1 || stod(argv[2]) <= 0)
	{
		cout << "Parameter 2 has to be between 0 to 1" << endl;
		return 0;
	}

	ofstream file;
	ofstream file2;

	file.open(outputDotFile);
	file2.open(outputFile);

	cout << "Create a graph with " << argv[1] << " vertices and propability of " << stod(argv[2]) * 100 << "% having edge between 2 vertices" << endl;

	file << "graph input {" << endl;

	int size = stoi(argv[1]);
	srand(time(NULL));

	for (int i = 0; i < size; i++)
	{
		for (int j = i + 1; j < size; ++j)
		{
			if (i != j)
			{
				int randomNumber = rand() % (int)(1 / stod(argv[2]));
				if (randomNumber == 0)
				{
					file << "	" << i << " -- " << j << endl;
					file2 << i << ' ' << j << endl;
				}
			}
		}
	}

	file << "}" << endl;

	file.close();

	cout << "Data is stored in " << outputFile << endl;
}