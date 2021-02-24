// Port.cpp

#include "Port.h"
#include <iostream>

char Port::GetID() {
	return name;
}

vector<int> Port::GetBridgeIDs() {
	return availableBridges;
}

void Port::ConnectBridge(int bridge) {
	availableBridges.push_back(bridge);
}

Configuration Port::GetBestConfiguration() {
	return bestConfig;
}

/// <summary>
/// When a bridge uses a port to send a message, we need to check if this is the best configuration for sending the message.
/// </summary>
void Port::SendMessage(Configuration message) {
	if (message.root < bestConfig.root) {
		bestConfig = message;
	}
	else if (message.root == bestConfig.root) {
		if (message.rootDist < bestConfig.rootDist) {
			bestConfig = message;
		}
		else if (message.rootDist == bestConfig.rootDist) {
			if (message.fromNode < bestConfig.fromNode) {
				bestConfig = message;
			}
		}
	}
	bestConfig.open = true;
}