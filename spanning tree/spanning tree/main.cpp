// main.cpp
// PURPOSE: central file for running the spanning tree algorithm
// AUTHOR: Michael Probst

#include <iostream>
#include <string>
#include "Network.h"
using namespace std;

int main(int argc, char* argv[]) {
	// TODO: ensure the parameters are valid
	if (argc < 3) {
		cout << "Too few arguments. Please enter the command './spanningtree <LANfilename.txt> <sequence of integers separated by spaces>' \n";
		return 0;
	}

	string filename = argv[1];
	ifstream f(filename.c_str());
	if (!f.good()) {
		cout << "ERROR: " << filename << " does not exist.\n";
		return 0;
	}

	if (filename.substr(filename.find_last_of(".") + 1) != "txt") {
		cout << "ERROR: " << filename << " is not a valid configuration file. Please use a .txt file" << endl;
		return 0;
	}

	Network net = Network(filename);
	int numMessages = argc - 2;
	int* sequence = new int[numMessages];

	for (int i = 0; i < numMessages; i++) {
		int c = argv[i+2][0] - '0';
		sequence[i] = c;
	}

	net.SendMessages(sequence, numMessages);
	delete[] sequence;
	return 0;
}