#include "Network.h"
#include <algorithm>
#include <queue>

Network::Network(string config_filename) {
	ifstream config_file;
	string line;

	// Use text file to construct network
	config_file.open(config_filename);
	while (!config_file.eof()) {
		getline(config_file, line);
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

	// initialize best port configurations on each bridge
	for (int i = 0; i < nodes.size(); i++) {
		nodes[i].UpdatePortConfigs();
	}
}

/// <summary>
/// Given the id of a bridge, create a new bridge. If it exists, return existing bridge.
/// </summary>
/// <param name="id"></param> id of new or existing bridge.
/// <returns></returns>
Bridge Network::CreateBridge(int id) {
	int index = GetBridgeIndex(id);
	if (index == -1) {
		return Bridge(id);
	}
	else {
		return nodes[index];
	}
}

/// <summary>
/// Given the name of a port, create a new port. If it exists, return existing port.
/// </summary>
/// <param name="name"></param> name of new or existing port.
/// <returns></returns>
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
/// Adds a bridge to the network.
/// If bridge already exists, do not add it to network.
/// </summary>
/// <param name="node"></param> bridge object to add
/// <returns></returns>
void Network::AddBridge(Bridge node) {
	int index = GetBridgeIndex(node.GetID());
	// given bridge did not exist in network, add it.
	if (index == -1) {
		nodes.push_back(node);
	}
}

/// <summary>
/// Adds a port to the network.
/// If port already exists, do not add it to network.
/// </summary>
/// <param name="port"></param> port object to add.
/// <returns></returns>
void Network::AddPort(Port port) {
	int index = GetPortIndex(port.GetID());
	// given port did not exist in network, add it.
	if (index == -1) {
		ports.push_back(port);
	}
}

/// <summary>
/// Uses the bridge's id to return its index in the nodes array.
/// </summary>
/// <param name="id"></param> id value of desired bridge
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
/// Uses the port's id to return its index in the ports array.
/// </summary>
/// <param name="name"></param> name of desired port.
/// <returns></returns>
int Network::GetPortIndex(char name) {
	for (int i = 0; i < ports.size(); i++) {
		if (ports[i].GetID() == name) {
			return i;
		}
	}
	return -1;
}


/// <summary>
/// Send message from one node to all it's connected nodes via the connecting port.
/// If a node does not send or receive a message, then it will believe it is still
/// the root. Therefore, it will think it's ports best configurations will be for 
/// the node. 
/// </summary>
/// <param name="sender"></param> the index of the node sending the message
void Network::SendMessage(int sender) {
	Configuration message = nodes[sender].GetBestConfiguration();
	vector<char> links = nodes[sender].GetConnections();
	message.fromNode = nodes[sender].GetID();
	for (int i = 0; i < links.size(); i++) {
		int port = GetPortIndex(links[i]);
		message.fromPort = ports[port].GetID();
		vector<int> connections = ports[port].GetBridgeIDs();
		ports[port].SendMessage(message);
		for (int j = 0; j < connections.size(); j++) {
			int bridge = GetBridgeIndex(connections[j]);
			if (bridge == sender) {
				continue;
			}
			//cout << nodes[sender].GetID() << " sends to " << nodes[bridge].GetID() << " via " << message.fromPort << ": <" << message.root << ", " << message.rootDist << ">\n";
			nodes[bridge].ReceiveMessage(message);
			nodes[sender].UpdatePortConfigs();
		}
	}
}

/// <summary>
/// Prints each node in the network and its best configuration.
/// Also, prints the ports connected to each port and their
/// respective best configurations.
/// </summary>
void Network::PrintNetwork() {
	for (int i = 0; i < nodes.size(); i++) {
		Configuration config = nodes[i].GetBestConfiguration();
		cout << "Bridge " << nodes[i].GetID() << ": best configuration <" << config.root << ", " << config.rootDist << "> from " << config.fromNode << " via " << config.fromPort << endl;
		vector<char> connections = nodes[i].GetConnections();
		for (int j = 0; j < connections.size(); j++) {
			// If the best configuration of the port is better than what the node believes, then display that configuration
			int port = GetPortIndex(connections[j]);
			Configuration portConfig = nodes[i].GetPortConfig(ports[port].GetID());
			Configuration bestConfig = ports[port].GetBestConfiguration();
			bool open = false;
			
			if (bestConfig.root < portConfig.root) {
				portConfig = bestConfig;
			}
			else if (bestConfig.root == portConfig.root) {
				if (bestConfig.rootDist < portConfig.rootDist) {
					portConfig = bestConfig;
				}
				else if (bestConfig.rootDist == portConfig.rootDist) {
					if (bestConfig.fromNode < portConfig.fromNode) {
						portConfig = bestConfig;
					}
				}
			}

			// If the port sent the best configuration to the current node, open the port.
			// If the node's best configuration came from itself, open the port
			if (portConfig.fromPort == config.fromPort || portConfig.fromNode == nodes[i].GetID()) {
				open = true;
			}
			portConfig.open = open;

			cout << "\tport " << ports[port].GetID() << ": <" << portConfig.root << ", " << portConfig.rootDist << ", " << portConfig.fromNode << "> ";
			if (portConfig.open) {
				cout << "open ";
			}
			else {
				cout << "closed ";
			}
			cout << endl;
		}
	}
}

/// <summary>
/// Sends messages from designated bridges, which in turn configures the network.
/// </summary>
/// <param name="sequence"></param> ids of bridges to send messages from.
/// <param name="numMessages"></param> total number of messages to send.
void Network::SendMessages(int sequence[], int numMessages) {
	for (int i = 0; i < numMessages; i++) {
		int bridge = GetBridgeIndex(sequence[i]);
		if (bridge != -1) {
			SendMessage(bridge);
		}
		else {
			cout << "ERROR: bridge " << sequence[i] << " does not exist in the network\n";
		}
	}
	PrintNetwork();
}