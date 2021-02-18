#pragma once
// Bridge.h
#include "Port.h"

using namespace std;

// Struct containing informaton of configuration message
struct Configuration {
	int root;			// The believed root
	int rootDist;		// Distance to believed root
	int fromPort;		// ID of Which port did the config come from
	int fromNode;		// ID of Which node (bridge) did the config come from
};

class Bridge {
	private:
		int id;							// identifier for this Bridge
		Configuration bestConfig;		// Best configuation for the node
		Port connectedPorts[];			// Array of ports connected to the bridge
		// Configuration[] portConfigs	// Best configuration for each of the bridge's ports

	public:
		int GetID();
		void SetID(int newID);
};