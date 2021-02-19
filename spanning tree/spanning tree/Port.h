#pragma once
// Port.h
#include <iostream>
using namespace std;


class Port {
	private:
		char name;			// identifier for this port
	public:
		char GetID();
		Port(char _name) {
			name = _name;
		}
};