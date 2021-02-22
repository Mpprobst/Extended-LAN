// Bridge.cppp

#include "Bridge.h"
using namespace std;

int Bridge::GetID() {
	return id;
}

Configuration Bridge::GetConfiguration() {
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

void Bridge::ReceiveMessage(Configuration message) {
	if (message.rootDist < bestConfig.rootDist) {
		bestConfig = message;
	}
	else if (message.rootDist == bestConfig.rootDist) {
		if (message.fromNode < bestConfig.fromNode) {
			bestConfig = message;
		}
	}
}


