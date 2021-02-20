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

	Configuration() {
		root = 0;
		rootDist = 0;
		fromPort = 0;
		fromNode = 0;
	}
	Configuration(int _root, int dist, int port, int node) {
		root = _root;
		rootDist = dist;
		fromPort = port;
		fromNode = node;
	}
};

class Bridge {
	private:
		int id;							// identifier for this Bridge
		Configuration bestConfig;		// Best configuation for the node
		vector<Port> connectedPorts;	// Array of ports connected to the bridge
		// Configuration[] portConfigs	// Best configuration for each of the bridge's ports

	public:
		int GetID();
		void SetID(int newID);
		vector<Port> GetConnections();
		void ConnectPort(Port port);

		Bridge(int _id) {
			id = _id;
			bestConfig = Configuration(id, 0, 0, id);
		}
};