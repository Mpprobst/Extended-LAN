#include "Network.h"
#include <algorithm>
#include <queue>

Network::Network(string config_filename) {
	ifstream config_file;
	string line;

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
/// Send message from one node to all it's connected nodes via the connecting port.
/// If a node does not send or receive a message, then it will believe it is still
/// the root. Therefore, it will think it's ports best configurations will be for 
/// the node.
/// </summary>
/// <param name="startNode"></param>
void Network::SendMessage(int startNode)
{
	int sender = startNode;
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
			//cout << nodes[sender].GetID() << " sends to " << nodes[bridge].GetID() << " via " << message.fromPort << ": <" << message.root << ", " << message.rootDist << ">";
			nodes[bridge].ReceiveMessage(message);
			nodes[sender].UpdatePortConfigs();
			cout << endl;
		}
	}
}

/// <summary>
/// Debug to ensure the network was configured correctly
/// </summary>
void Network::PrintNetwork() {
	for (int i = 0; i < nodes.size(); i++) {
		Configuration config = nodes[i].GetBestConfiguration();
		cout << "Bridge " << nodes[i].GetID() << ": best configuration <" << config.root << ", " << config.rootDist << "> from " << config.fromNode << " via " << config.fromPort << endl;
		vector<char> connections = nodes[i].GetConnections();
		for (int j = 0; j < connections.size(); j++) {
			// If the best configuration of the port is better than what the node believes, then display that
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
	/*
	cout << endl << "CHECK PORT CONNECTIONS" << endl;
	for (int i = 0; i < ports.size(); i++) {
		cout << "Port " << ports[i].GetID() << " is connected to: ";
		vector<int> bridges = ports[i].GetBridgeIDs();
		for (int j = 0; j < bridges.size(); j++) {
			int bridge = GetBridgeIndex(bridges[j]);
			Bridge _bridge = nodes[bridge];
			cout << _bridge.GetID() << " ";
		}
		Configuration config = ports[i].GetBestConfiguration();
		cout << "\tconfig: <" << config.root << ", " << config.rootDist << ", " << config.fromNode << ", " << config.fromPort << ">";
		if (config.open) {
			cout << " OPEN" << endl;
		}
		else {
			cout << " CLOSED" << endl;
		}
	}*/
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

/// <summary>
/// Takes a sequence of bridges to send messages from. 
/// Connections between bridges and nodes that are not used should be closed 
/// </summary>
/// <param name="sequence"></param>
void Network::SendMessages(int sequence[], int numMessages) {
	for (int i = 0; i < numMessages; i++) {
		int bridge = GetBridgeIndex(sequence[i]);
		SendMessage(bridge);
	}
	PrintNetwork();
}