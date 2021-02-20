#include "Network.h"
#include <algorithm>

//bool CompareBridge(Bridge a, Bridge b) { return a.GetID() < b.GetID(); }
//bool ComparePort(Port a, Port b) { return a.GetID() < b.GetID(); }

Network::Network(string config_filename) {
	ifstream config_file;
	string line;

	config_file.open(config_filename);
	while (!config_file.eof()) {
		getline(config_file, line);
		//TODO: parse the line and get a number, that is the bridge. The characters are the ports connected to the bridge
		Bridge * bridge = &AddBridge(line[0]-'0');
		for (int i = 1; i < line.length(); i++) {
			if (line[i] != ' ') {
				Port * port = &AddPort(line[i]);
				bridge->ConnectPort(port->GetID());
				port->ConnectBridge(bridge->GetID());
			}
		}
	}
	config_file.close();

	//sort(nodes.begin(), nodes.end(), CompareBridge);
	//sort(ports.begin(), ports.end());
	
	// DEBUG TO MAKE SURE EVERYTHING WORKED
	for (int i = 0; i < nodes.size(); i++) {
		cout << "Bridge " << nodes[i].GetID() << " has ports: ";
		vector<char> connections = nodes[i].GetConnections();
		for (int j = 0; j < connections.size(); j++) {
			Port port = GetPort(connections[j]);
			cout << port.GetID() << " ";
		}
		cout << endl << endl;
	}

	for (int i = 0; i < ports.size(); i++) {
		cout << "Port " << ports[i].GetID() << " is connected to: ";
		vector<int> bridges = ports[i].GetBridgeIDs();
		for (int j = 0; j < bridges.size(); j++) {
			Bridge bridge = GetBridge(bridges[j]);
			cout << bridge.GetID() << " ";
		}
		cout << endl;
	}

	// TODO: Consider sorting ports and nodes for easier access later on.

	// TODO: get the minimum spanning tree now that the network is connected

}

Bridge Network::GetBridge(int id) {
	for (int i = 0; i < nodes.size(); i++) {
		if (nodes[i].GetID() == id) {
			return nodes[i];
		}
	}
}
Port Network::GetPort(char name) {
	for (int i = 0; i < ports.size(); i++) {
		if (ports[i].GetID() == name) {
			return ports[i];
		}
	}
}

/// <summary>
/// Creates a bridge and adds it to the network.
/// If bridge already exists, return that bridge and do not add new bridge to network
/// </summary>
/// <param name="id"></param>
/// <returns></returns>
Bridge Network::AddBridge(int id) {
	Bridge bridge = Bridge(id);
	for (int i = 0; i < nodes.size(); i++) {
		if (nodes[i].GetID() == id) {
			bridge = nodes[i];
			return bridge;
		}
	}
	nodes.push_back(bridge);
	return bridge;
}

/// <summary>
/// Creates a port and adds it to the network.
/// If node already exists, return that port and do not add new port to network.
/// </summary>
/// <param name="name"></param>
/// <returns></returns>
Port Network::AddPort(char name) {
	Port port = Port(name);
	for (int i = 0; i < ports.size(); i++) {
		if (ports[i].GetID() == name) {
			port = ports[i];
			return port;
		}
	}
	ports.push_back(port);
	return port;
}