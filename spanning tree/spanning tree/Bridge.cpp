// Bridge.cppp

#include "Bridge.h"
using namespace std;

int Bridge::GetID() {
	return id;
}

Configuration Bridge::GetBestConfiguration() {
	return bestConfig;
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

Configuration Bridge::GetPortConfig(int connectionIndex) {
	if (connectionIndex == -1) {
		return Configuration(-1, -1, '?', -1, false);	// if config is invalid, return some nonsense
	}
	return portConfigs[connectionIndex];
}

int Bridge::GetConfigIndex(int bridgeID, char portID) {
	for (int i = 0; i < portConfigs.size(); i++) {
		if (portConfigs[i].fromNode == bridgeID && portConfigs[i].fromPort == portID) {
			return i;
		}
	}
	return -1;
}

/// <summary>
/// Reads a message and if it is better than current configuration, replace it. Return true if new config was taken
/// </summary>
/// <param name="message"></param>
/// <returns></returns>
void Bridge::ReceiveMessage(Configuration message) {
	// If message has not yet been sent from a node via some port, add it to the portConfigs
	int configIndex = GetConfigIndex(message.fromNode, message.fromPort);
	// <1, 2, 4, I>
	bool updated = false;
	if (message.rootDist < bestConfig.rootDist) {
		bestConfig = message;
		updated = true;
	}
	else if (message.rootDist == bestConfig.rootDist) {
		if (message.fromNode < bestConfig.fromNode) {
			bestConfig = message;
			updated = true;
		}
	}
	bestConfig.open = true;
	if (configIndex == -1) {
		message.open = updated;
		portConfigs.push_back(message);
	}
	else if (!updated) {
		portConfigs[configIndex].open = false;
	}


}


