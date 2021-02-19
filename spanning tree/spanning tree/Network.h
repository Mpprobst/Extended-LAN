#pragma once
// Network.h

#include <vector>
#include <string>
#include <fstream>
#include "Bridge.h"
using namespace std;

class Network {
private:
	vector<Bridge> nodes;
	vector<Port> ports;

public:
	Network(string config_filename);
	Bridge AddBridge(int id);
	Port AddPort(char name);
};