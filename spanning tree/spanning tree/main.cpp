// main.cpp
// PURPOSE: central file for running the spanning tree algorithm
// AUTHOR: Michael Probst

#include <iostream>
#include "Network.h"
#include <string>
using namespace std;

int main(int argc, char* argv[]) {
	Network net = Network(argv[1]);
	// get argument count, add those characters to the sequence array
	int numMessages = argc - 2;
	char* sequence = new char[numMessages];

	for (int i = 0; i < numMessages; i++) {
		char c = argv[i+2][0];
		sequence[i] = c;
	}

	net.SendMessages(sequence, numMessages);
	delete[] sequence;
}