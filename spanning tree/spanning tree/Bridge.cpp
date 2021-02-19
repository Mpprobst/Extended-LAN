// Bridge.cppp

#include "Bridge.h"
using namespace std;

int Bridge::GetID() {
	return id;
}

void Bridge::SetID(int newID) {
	id = newID;
}

vector<Port> Bridge::GetConnections() {
	return connectedPorts;
}


void Bridge::ConnectPort(Port port) {
	connectedPorts.push_back(port);
}


