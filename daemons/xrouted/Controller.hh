/*
** Copyright 2017 Carnegie Mellon University
**
** Licensed under the Apache License, Version 2.0 (the "License");
** you may not use this file except in compliance with the License.
** You may obtain a copy of the License at
**
**    http://www.apache.org/licenses/LICENSE-2.0
**
** Unless required by applicable law or agreed to in writing, software
** distributed under the License is distributed on an "AS IS" BASIS,
** WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
** See the License for the specific language governing permissions and
** limitations under the License.
*/
#ifndef _controller_hh
#define _controller_hh

#include <syslog.h>
#include <map>
#include <vector>
#include <string>

#include "Settings.hh"
#include "RouteModule.hh"
#include "Topology.hh"


class Controller : public RouteModule {
public:
	Controller(const char *name) : RouteModule(name) {}
	~Controller() {}

protected:

	int handler();
	int init();
	int makeSockets();
	int saveControllerDAG();

	int sendHello();
	int sendInterDomainLSA();
	int sendRoutingTable(NodeStateEntry *nodeState, RouteTable routingTable);

	int processMsg(std::string msg, uint32_t iface);
	int processHostRegister(const Xroute::HostJoinMsg& msg);
	int processHello(const Xroute::HelloMsg& msg, uint32_t iface);
	int processLSA(const Xroute::LSAMsg& msg);
	int processInterdomainLSA(const Xroute::XrouteMsg& msg);
	void processRoutingTable(RouteTable routingTable);

	void purge();

	void extractNeighborADs(void);
	void populateNeighboringADBorderRouterEntries(string currHID, RouteTable &routingTable);
	void populateADEntries(RouteTable &routingTable, RouteTable ADRoutingTable);
	void populateRoutingTable(std::string srcHID, NetworkTable &networkTable, RouteTable &routingTable);
	void printRoutingTable(std::string srcHID, RouteTable &routingTable);
	void printADNetworkTable();

protected:
    Settings *_settings;

	uint32_t _flags;
	bool _dual_router;

	std::string _myAD;
	std::string _myHID;
	Node _my_fid;
	std::string _controller_sid;

	std::string _dual_router_AD; // AD (with dual router) -- default AD for 4ID traffic

	RouteTable    _ADrouteTable;    // dest AD -> route entry

    NeighborTable _neighborTable;   // neighbor HID -> neighbor entry
	NeighborTable _ADNeighborTable; // neighbor HID to neighbor entry for ADs

	NetworkTable  _networkTable;    // dest HID -> node state entry
    NetworkTable  _ADNetworkTable;  // dest AD to NodeState entry for ADs

	TimestampList _timeStamp;

	int32_t _calc_dijstra_ticks;

	// FIXME: improve these guys
    // next time the hello or lsa should be sent
	struct timeval h_freq, h_fire;
	struct timeval l_freq, l_fire;

	// last time we looked for stale entries
	time_t _last_purge;
	time_t _last_update_config;
//	time_t _last_update_latency;
};

#endif
