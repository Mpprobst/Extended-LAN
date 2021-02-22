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
	int root = 0;
	for (int i = 0; i < nodes.size(); i++) {
		if (nodes[i].GetID() < nodes[root].GetID()) {
			root = GetBridgeIndex(nodes[i].GetID());
		}
	}
	
	// Identify root as root
	nodes[root].ReceiveMessage(Configuration(nodes[root].GetID(), 0, ' ', nodes[root].GetID(), true));		// declare the root
	vector<int> closedNodes;				// vector containing IDs of bridges that have sent config messages on their ports
	stack<int> openNodes;					// vector containint IDs of bridges yet to send messages
	openNodes.push(nodes[root].GetID());

	int sender = root;
	// BFS approach for forwarding messages
	cout << "CONFIGURING NETWORK" << endl;
	while (openNodes.size() > 0 && closedNodes.size() < nodes.size()) { 
		sender = GetBridgeIndex(openNodes.top());
		openNodes.pop();
		bool visited = false;
		for (int i = 0; i < closedNodes.size(); i++) {
			if (closedNodes[i] == nodes[sender].GetID()) {
				visited = true;
			}
		}

		if (visited) {
			continue;
		}

		Configuration message = nodes[sender].GetBestConfiguration();
		message.rootDist += 1;
		vector<char> links = nodes[sender].GetConnections();
		for (int i = 0; i < links.size(); i++) {
			int port = GetPortIndex(links[i]);
			vector<int> connections = ports[port].GetBridgeIDs();
			for (int j = 0; j < connections.size(); j++) {
				//cout << "port connected to " << connections[j] << " sending from " << ports[port].GetID() << endl;
				// Dont forward a message back to where it came from
				//if (connections[j] == message.fromNode || connections[j] == nodes[sender].GetID()) {
					//cout << "do not forward " << nodes[sender].GetID() << " to " << connections[j] << endl;
				//	continue;
				//}
				message.fromNode = nodes[sender].GetID();
				int bridge = GetBridgeIndex(connections[j]);
				openNodes.push(connections[j]);
				message.fromPort = ports[port].GetID();
				//cout << "sender: " << nodes[sender].GetID() << ". receiver: " << nodes[bridge].GetID() << " thinks root is: " << message.root << " " << message.rootDist << " away" << endl;
				nodes[bridge].ReceiveMessage(message);
			}
		}
		closedNodes.push_back(nodes[sender].GetID());
	}

	// DEBUG TO MAKE SURE EVERYTHING WORKED
	cout << "CHECK BRIDGE CONNECTIONS" << endl;
	for (int i = 0; i < nodes.size(); i++) {
		cout << "Bridge " << nodes[i].GetID() << " has ports:\n";
		vector<char> connections = nodes[i].GetConnections();
		for (int j = 0; j < connections.size(); j++) {
			int port = GetPortIndex(connections[j]);
			cout << "\t"<< ports[port].GetID();
			// print if port is open or closed by looking at the bridges port configs
			Bridge bridge = nodes[i];
			for (int k = 0; k < nodes.size(); k++) {
				Configuration portConfig = bridge.GetPortConfig(bridge.GetConfigIndex(nodes[k].GetID(), ports[port].GetID()));
				if (portConfig.fromNode != -1) {
					cout << "<" << portConfig.root << ", " << portConfig.rootDist << ", " << portConfig.fromNode << ", " << portConfig.fromPort << "> ";

					if (portConfig.open) {
						cout << "OPEN\n";
					}
					else {
						cout << "CLOSE\n";
					}
				}
				else {
					cout << endl;
				}
			}
		}
		Configuration config = nodes[i].GetBestConfiguration();
		cout << " config: <" << config.root << ", " << config.rootDist << ", " << config.fromNode << ", " << config.fromPort << ">" << endl;
	}

	cout << endl << "CHECK PORT CONNECTIONS" << endl;
	for (int i = 0; i < ports.size(); i++) {
		cout << "Port " << ports[i].GetID() << " is connected to: ";
		vector<int> bridges = ports[i].GetBridgeIDs();
		for (int j = 0; j < bridges.size(); j++) {
			int bridge = GetBridgeIndex(bridges[j]);
			Bridge _bridge = nodes[bridge];
			cout << _bridge.GetID() << " ";
		}
		cout << endl;
	}

}

/// <summary>
/// Find a bridge by its id. If that bridge doesn't exist, its id will be -1/
/// </summary>
/// <param name="id"></param>
/// <returns></returns>
int Network::GetBridgeIndex(int id) {
	for (int i = 0; i < nodes.size(); i++) {
		if (nodes[i].GetID() == id) {
			return i;
		}
	}
	return -1;
}

/// <summary>
/// Finds a port in the network by its name. If that port does not exist, a port with id = ? is returned
/// </summary>
/// <param name="name"></param>
/// <returns></returns>
int Network::GetPortIndex(char name) {
	for (int i = 0; i < ports.size(); i++) {
		if (ports[i].GetID() == name) {
			return i;
		}
	}
	return -1;
}

Bridge Network::CreateBridge(int id) {
	int index = GetBridgeIndex(id);
	if (index == -1) {
		return Bridge(id);
	}
	else {
		return nodes[index];
	}
}

Port Network::CreatePort(char name) {
	int index = GetPortIndex(name);
	if (index == -1) {
		return Port(name);
	}
	else {
		return ports[index];
	}
}

/// <summary>
/// Creates a bridge and adds it to the network.
/// If bridge already exists, return that bridge and do not add new bridge to network
/// </summary>
/// <param name="id"></param>
/// <returns></returns>
void Network::AddBridge(Bridge node) {
	int index = GetBridgeIndex(node.GetID());
	// If new bridge was not found, add it to network
	if (index == -1) {
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
	int index = GetPortIndex(port.GetID());
	// If new port was not found, add it to network
	if (index == -1) {
		ports.push_back(port);
	}
}