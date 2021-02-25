# CS571 pg1 - Spanning Tree
## By: Michael Probst

## Overview
This repository contains a solution to the assignment pg1 for the class CS571 - Computer Networks at the University of Kentucky instructed by Dr. Zongming Fei. 

The purpose of this project is to simulate the spanning tree algorithm for extended LANs. This is done by constructing a network of bridges and ports and sending messages along those bridges to configure the network and determine what the root of the network is.

## Compiling/Running
There is a makefile that eases compilation and running. To compile, enter: `make` then run the program with the command: `./spanningtree <LANconfigfile.txt> <sequence of bridge ids separated by whitespace>`
ex: `./spanningtree lanconfig.txt 1 3 4 6 2 3 1 5` and `./spanningtree lanconfig.txt 1 4`
A simple example can be run with the command: `make run`
The folder can be cleaned up with the command: `make clean`

### Files
#### Port.h Port.cpp: 
Definition and implementation for a port in the network. 
Contains the Configuration struct that contains information required for the nodes to send messages. It records a believed root of the network, the distance to that root, the node from which the configuration Stores its id, bridges it is connected to, and the best configuration it has seen.
Ports contain name to identify the port, the bridges it can access, and the best configuration it has seen.
#### Bridge.h Bridge.cpp:
Definition and implementation for a bridge/node in the network.
Bridges contain an id to identify the bridge, the best configuration it has seen, the ports it is connected to as well as the best configuration it believes each port has based on messages the bridge has sent.
#### Network.h Network.cpp:
Definition and implementation for the network in which we are simulation.
This object performs all actions that need to occur utilizing the Bridge and Port objects. The network stores vectors of all the bridges and ports in the network.
#### main.cpp:
The central file for initializing the network and prompting it to send messages across bridges indicated by the user's input.
#### makfile:
A standard makefile to make compilation easier. Includes functionality to compile, run, and clean.
#### lanconfig.txt
A text file that contains the configuration of a network. If other networks are to be constructed, the configuration file MUST be formatted in the following manner:
`<bridge id> <connected port1 id> <connected port2 id> ... <connected portN id>
.
.
.`

A bridge's configuration MUST be separated by a new line, and each id for a port or bridge must be separated by whitespace.

### Limitations
The program is a bit inefficient due to a lack of knowledge of properly using pointers, so all access to the bridges and ports are done by first gathering the index of the bridge or port by searching it, then modifying the bridge/port by directly accessing the bridge/port vector with the found index.
Also, it is known that in the first example test provided by the handout that Bridge 3 will incorrectly identify its best configuration acquired via port A. This causes port A to be seen as open rather than closed in this test.  
