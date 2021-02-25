#include "Bridge.h"

int Bridge::GetID() {
	return id;
}

void Bridge::SetID(int newID) {
	id = newID;
}

vector<char> Bridge::GetConnections() {
	return connectedPorts;
}

void Bridge::ConnectPort(char port) {
	connectedPorts.push_back(port);
}

Configuration Bridge::GetBestConfiguration() {
	return bestConfig;
}

void Bridge::ModifyPortConfig(int portIndex, Configuration message) {
	portConfigs[portIndex] = message;
}

int Bridge::GetConfigIndex(char portID) {
	int connectionIndex = -1;
	for (int i = 0; i < portConfigs.size(); i++) {
		if (portConfigs[i].fromPort == portID) {
			connectionIndex = i;
			break;
		}
	}
	return connectionIndex;
}

Configuration Bridge::GetPortConfig(char portID) {
	int connectionIndex = GetConfigIndex(portID);
	if (connectionIndex == -1) {
		return Configuration(-1, -1, '?', -1, true);	// if config is invalid, return some nonsense
	}
	return portConfigs[connectionIndex];
}

/// <summary>
/// Reads a message and if it is better than current configuration, replace it.
/// </summary>
/// <param name="message"></param>
/// <returns></returns>
void Bridge::ReceiveMessage(Configuration message) {
	// If message has not yet been sent from a node via some port, add it to the portConfigs
	if (message.root < bestConfig.root) {
		message.rootDist += 1;
		bestConfig = message;
	}
	else if (message.root == bestConfig.root) {
		if (message.rootDist < bestConfig.rootDist) {
			message.rootDist += 1;
			bestConfig = message;
		}
		else if (message.rootDist == bestConfig.rootDist) {
			if (message.fromNode < bestConfig.fromNode) {
				bestConfig = message;
			}
		}
	}
	UpdatePortConfigs();
}

/// <summary>
/// When a message is sent from a port, we should update each of the port configurations.
/// If there is not yet a configuration for a port, it is added to the connectedPorts vector.
/// </summary>
void Bridge::UpdatePortConfigs() {
	for (int i = 0; i < connectedPorts.size(); i++) {
		Configuration config = GetPortConfig(connectedPorts[i]);
		bool isNew = (config.root == -1);
		config.root = bestConfig.root;
		config.rootDist = bestConfig.rootDist;
		config.fromNode = id;
		config.fromPort = connectedPorts[i];
		config.open = true;
		if (isNew) {
			portConfigs.push_back(config);
		}
		else {
			portConfigs[GetConfigIndex(connectedPorts[i])] = config;
		}
	}
}  