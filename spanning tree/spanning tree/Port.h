#pragma once
// Port.h
#include <vector>
using namespace std;

// Struct containing informaton of configuration message
struct Configuration {
	int root;			// The believed root
	int rootDist;		// Distance to believed root
	char fromPort;		// ID of Which port did the config come from
	int fromNode;		// ID of Which node (bridge) did the config come from
	bool open;			// if the connection between fromPort and fromNode is open or closed

	Configuration() {
		root = 100;
		rootDist = 100;
		fromPort = ' ';
		fromNode = 100;
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

class Port {
	private:
		char name;			// identifier for this port
		vector<int> availableBridges;
		Configuration bestConfig;
	public:
		char GetID();
		vector<int> GetBridgeIDs();
		Configuration GetBestConfiguration();
		void SendMessage(Configuration message);
		Port(char _name) {
			name = _name;
			bestConfig = Configuration(100, 100, name, 100, true);		// initialized with 100 becasue we dont know what bridges could send messages yet
		}
		void ConnectBridge(int bridge);
		
};