// main.cpp
// PURPOSE: central file for running the spanning tree algorithm
// AUTHOR: Michael Probst

#include <iostream>
#include "Network.h"
using namespace std;

int main(int argc, char* argv[]) {
	// TODO: ensure the parameters are valid

	Network net = Network(argv[1]);
	int numMessages = argc - 2;
	int* sequence = new int[numMessages];

	for (int i = 0; i < numMessages; i++) {
		int c = argv[i+2][0] - '0';
		sequence[i] = c;
	}

	net.SendMessages(sequence, numMessages);
	delete[] sequence;
}