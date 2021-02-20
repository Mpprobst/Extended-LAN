// Port.cpp

#include "Port.h"

char Port::GetID() {
	return name;
}

vector<int> Port::GetBridgeIDs() {
	return availableBridges;
}

void Port::ConnectBridge(int bridge) {
	availableBridges.push_back(bridge);
}