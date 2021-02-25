#pragma once

#include "Port.h"

class Bridge {
	private:
		int id;								// identifier for this Bridge
		Configuration bestConfig;			// Best configuation for the node
		vector<char> connectedPorts;		// Array of ports connected to the bridge
		vector<Configuration> portConfigs;	// All configurations for each of the bridge's ports. If the config != port's best config, then close it

	public:
		Bridge(int _id) {
			id = _id;
			bestConfig = Configuration(id, 0, '?', id, true);
		}

		int GetID();
		void SetID(int newID);
		vector<char> GetConnections();
		void ConnectPort(char port);
		Configuration GetBestConfiguration();
		void ModifyPortConfig(int portIndex, Configuration message);
		int GetConfigIndex(char portID);
		Configuration GetPortConfig(char portID);

		void ReceiveMessage(Configuration message);
		void UpdatePortConfigs();
};