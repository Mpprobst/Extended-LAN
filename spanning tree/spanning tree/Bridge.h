#pragma once
// Bridge.h
#include "Port.h"
using namespace std;

// Struct containing informaton of configuration message
struct Configuration {
	int root;			// The believed root
	int rootDist;		// Distance to believed root
	char fromPort;		// ID of Which port did the config come from
	int fromNode;		// ID of Which node (bridge) did the config come from
	bool open;			// if the connection between fromPort and fromNode is open or closed

	Configuration() {
		root = 0;
		rootDist = 0;
		fromPort = ' ';
		fromNode = 0;
		open = true;
	}
	Configuration(int _root, int dist, char port, int node, bool isOpen) {
		root = _root;
		rootDist = dist;
		fromPort = port;
		fromNode = node;
		open = isOpen;
	}
};

class Bridge {
	private:
		int id;							// identifier for this Bridge
		Configuration bestConfig;		// Best configuation for the node
		vector<char> connectedPorts;	// Array of ports connected to the bridge
		vector<Configuration> portConfigs;	// Configuration for each of the bridge's ports

	public:
		int GetID();
		Configuration GetBestConfiguration();
		Configuration GetPortConfig(int connectionIndex);
		int GetConfigIndex(int bridgeID, char portID);
		void SetID(int newID);
		vector<char> GetConnections();
		void ConnectPort(char port);
		void ReceiveMessage(Configuration message);

		Bridge(int _id) {
			id = _id;
			bestConfig = Configuration(id, 10, ' ', id, true);		// dist set to 10 initially so config messages will actually choose a new root
		}
};