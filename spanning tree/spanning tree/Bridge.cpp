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
	if (message.root < bestConfig.root) {
		message.rootDist += 1;
		bestConfig = message;
	}
	else if (message.root == bestConfig.root) {
		if (message.rootDist < bestConfig.rootDist) {
			message.rootDist += 1;
			bestConfig = message;
		}
	}
	bestConfig.open = true;
}

void Bridge::SendMessage(Configuration message) {
	int configIndex = GetConfigIndex(message.fromNode, message.fromPort);
	if (configIndex == -1) {
		message.open = true;		// new messages should always be seen as open
		portConfigs.push_back(message);
		return;
	}

	// if we are sending a message and the port's best configuration is this bridge, then we open the connection. Otherwise, close
	if (portConfigs[configIndex].root < message.root) {

	}
}


