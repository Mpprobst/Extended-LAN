#include "Network.h"
#include <algorithm>
#include <stack>

Network::Network(string config_filename) {
	ifstream config_file;
	string line;

	config_file.open(config_filename);
	while (!config_file.eof()) {
		getline(config_file, line);
		//TODO: parse the line and get a number, that is the bridge. The characters are the ports connected to the bridge
		Bridge bridge = CreateBridge(line[0]-'0');
		for (int i = 1; i < line.length(); i++) {
			if (line[i] != ' ') {
				Port port = CreatePort(line[i]);
				bridge.ConnectPort(port.GetID());
				AddPort(port);
			}
		}
		AddBridge(bridge);
	}
	
	// add bridge refs to ports
	for (int i = 0; i < ports.size(); i++) {
		for (int j = 0; j < nodes.size(); j++) {
			vector<char> connections = nodes[j].GetConnections();
			for (int k = 0; k < connections.size(); k++) {
				if (connections[k] == ports[i].GetID()) {
					ports[i].ConnectBridge(nodes[j].GetID());
				}
			}
		}
	}

	config_file.close();
	
	// TODO: Consider sorting ports and nodes for easier access later on.
	
	// Choose bridge with smallest ID as root
	Bridge root = nodes[0];
	for (int i = 0; i < nodes.size(); i++) {
		if (nodes[i].GetID() < root.GetID()) {
			root = nodes[i];
		}
	}
	
	// Identify root as root
	root.ReceiveMessage(Configuration(root.GetID(), 0, ' ', root.GetID()));		// declare the root
	vector<int> closedNodes;				// vector containing IDs of bridges that have sent config messages on their ports
	stack<int> openNodes;					// vector containint IDs of bridges yet to send messages
	openNodes.push(root.GetID());

	Bridge sender = root;
	// BFS approach for forwarding messages
	cout << "CONFIGURING NETWORK" << endl;
	while (openNodes.size() > 0 && closedNodes.size() < nodes.size()) { 
		sender = GetBridge(openNodes.top());
		openNodes.pop();
		bool visited = false;
		for (int i = 0; i < closedNodes.size(); i++) {
			if (closedNodes[i] == sender.GetID()) {
				visited = true;
			}
		}

		if (visited) {
			continue;
		}

		Configuration message = sender.GetConfiguration();
		message.rootDist += 1;
		message.fromNode = sender.GetID();
		vector<char> ports = sender.GetConnections();
		for (int i = 0; i < ports.size(); i++) {
			Port port = GetPort(ports[i]);
			vector<int> connections = port.GetBridgeIDs();
			for (int j = 0; j < connections.size(); j++) {
				// Dont forward a message back to where it came from
				if (connections[j] == message.fromNode) {
					continue;
				}
				Bridge bridge = GetBridge(connections[j]);
				openNodes.push(connections[j]);
				message.fromPort = connections[j];
				bridge.ReceiveMessage(message);
				cout << "sender: " << sender.GetID() << ". receiver: " << bridge.GetID() << " thinks root is: " << message.root << " " << message.rootDist << " away" << endl;

			}
		}
		closedNodes.push_back(sender.GetID());
	}

	// DEBUG TO MAKE SURE EVERYTHING WORKED
	cout << "CHECK BRIDGE CONNECTIONS" << endl;
	for (int i = 0; i < nodes.size(); i++) {
		cout << "Bridge " << nodes[i].GetID() << " has ports: ";
		vector<char> connections = nodes[i].GetConnections();
		for (int j = 0; j < connections.size(); j++) {
			Port port = GetPort(connections[j]);
			cout << port.GetID() << " ";
		}
		Configuration config = nodes[i].GetConfiguration();
		cout << " is " << config.rootDist << " away from root " << config.root << " as told by port " << config.fromPort << " from bridge " << config.fromNode << endl;
	}

	cout << endl << "CHECK PORT CONNECTIONS" << endl;
	for (int i = 0; i < ports.size(); i++) {
		cout << "Port " << ports[i].GetID() << " is connected to: ";
		vector<int> bridges = ports[i].GetBridgeIDs();
		for (int j = 0; j < bridges.size(); j++) {
			Bridge bridge = GetBridge(bridges[j]);
			cout << bridge.GetID() << " ";
		}
		cout << endl;
	}

}

/// <summary>
/// Find a bridge by its id. If that bridge doesn't exist, its id will be -1/
/// </summary>
/// <param name="id"></param>
/// <returns></returns>
Bridge Network::GetBridge(int id) {
	for (int i = 0; i < nodes.size(); i++) {
		if (nodes[i].GetID() == id) {
			return nodes[i];
		}
	}
	return Bridge(-1);
}

/// <summary>
/// Finds a port in the network by its name. If that port does not exist, a port with id = ? is returned
/// </summary>
/// <param name="name"></param>
/// <returns></returns>
Port Network::GetPort(char name) {
	for (int i = 0; i < ports.size(); i++) {
		if (ports[i].GetID() == name) {
			return ports[i];
		}
	}
	return Port('?');
}

Bridge Network::CreateBridge(int id) {
	Bridge bridge = GetBridge(id);
	if (bridge.GetID() == -1) {
		bridge = Bridge(id);
	}
	return bridge;
}

Port Network::CreatePort(char name) {
	Port port = GetPort(name);
	if (port.GetID() == '?') {
		port = Port(name);
	}
	return port;
}

/// <summary>
/// Creates a bridge and adds it to the network.
/// If bridge already exists, return that bridge and do not add new bridge to network
/// </summary>
/// <param name="id"></param>
/// <returns></returns>
void Network::AddBridge(Bridge node) {
	Bridge bridge = GetBridge(node.GetID());
	if (bridge.GetID() == -1) {
		nodes.push_back(node);
	}
}

/// <summary>
/// Creates a port and adds it to the network.
/// If node already exists, return that port and do not add new port to network.
/// </summary>
/// <param name="name"></param>
/// <returns></returns>
void Network::AddPort(Port port) {
	Port p = GetPort(port.GetID());
	if (p.GetID() == '?') {
		ports.push_back(port);
	}
}