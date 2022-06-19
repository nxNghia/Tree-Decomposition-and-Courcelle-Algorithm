#include <iostream>
#include <fstream>
#include <time.h>
#include <stdlib.h>
#include <cstring>

#define PROP 10

using namespace std;

const string outputFile = "input3.txt";
const string outputDotFile = "input3.dot";

int convertToInt (const char* str)
{
	int result = 0;
	for (int i = 0; i < strlen(str); ++i)
	{
		result *= 10;
		result += str[i] - '0';
	}

	return result;
}

int main(int argc, const char* argv[])
{
	if (argc != 2)
	{
		cout << "Need number of vertices" << endl;
		return 0;
	}

	ofstream file;
	ofstream file2;

	file.open(outputDotFile);
	file2.open(outputFile);

	file << "graph input {" << endl;

	int size = convertToInt(argv[1]);
	srand(time(NULL));

	for (int i = 0; i < size; i++)
	{
		for (int j = i + 1; j < size; ++j)
		{
			if (i != j)
			{
				int randomNumber = rand() % PROP;
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