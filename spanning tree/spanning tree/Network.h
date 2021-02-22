#pragma once
// Network.h

#include <string>
#include <fstream>
#include <iostream>
#include "Bridge.h"
using namespace std;

class Network {
private:
	vector<Bridge> nodes;
	vector<Port> ports;

public:
	Network(string config_filename);
	int GetBridgeIndex(int id);
	int GetPortIndex(char name);
	Bridge CreateBridge(int id);
	Port CreatePort(char name);
	void AddBridge(Bridge node);
	void AddPort(Port port);

};