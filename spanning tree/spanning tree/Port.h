#pragma once
// Port.h
#include <vector>
#include <iostream>
using namespace std;

class Port {
	private:
		char name;			// identifier for this port
		vector<int> availableBridges;
	public:
		char GetID();
		vector<int> GetBridgeIDs();
		Port(char _name) {
			name = _name;
		}
		void ConnectBridge(int bridge);
};