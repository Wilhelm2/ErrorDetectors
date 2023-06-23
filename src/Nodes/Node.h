//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
// 

#ifndef NODE_H_
#define NODE_H_

#include <omnetpp.h>
#include <algorithm>
#include <iostream>
#include <string>
#include <regex>
#include <functional>
#include "../Messages/AppMsg_m.h"
#include "../Messages/depReq_m.h"
#include "../Messages/depRsp_m.h"
#include "../Messages/Init_m.h"
#include "../Controller.h"
#include "../structures.h"
#include "../simulationParameters.h"
#include "../Clocks/ProbabilisticClock.h"
#include "../Dependencies/Dependencies.h"
#include "../Dependencies/TotalDependencies.h"
#include "../Detectors/ErrorDetector.h"
#include "../Detectors/HashErrorDetectorClockDifference.h"
#include "../Detectors/HashErrorDetectorInterval.h"
#include "../Detectors/MostefaouiErrorDetector.h"

using namespace omnetpp;
using namespace std;


typedef struct s_statsNode {
    unsigned int nbFalseDeliveredMessages = 0;
    unsigned int nbSentDependencies = 0;
    unsigned int nbDeliveredMessages = 0;
    unsigned int controlDataSize = 0;
}statsNode;

class Stats;
static unsigned int idCountNodeDep=0;

class Node : public cSimpleModule
{
public:
  virtual void initialize() override;
  virtual void handleMessage(cMessage *msg) override;
  virtual void processMessage(cMessage* msg) = 0;
  void sendAppMsg();
  AppMsg* createAppMsg();
  void clearDelivered();
  virtual bool testDeliverMessage(dep message) = 0;
  virtual void iterativeDelivery() = 0;
  bool deliverMsg(dep message);
  void RecvAppMsg(AppMsg*m);
  dep createDep(idMsg id, simtime_t recvtime, size_t hash, vector<idMsg> dependencies, ProbabilisticClock clock);


  cMessage* broadcastTimer = new cMessage();
  unsigned int seq = 0;
  unsigned int id = idCountNodeDep++;
  ProbabilisticClock clock;
  TotalDependencies deliveredMessagesTracker ; // tracks dependencies of process
  vector<dep> pendingMsg;
  vector<dep> delivered;

  simtime_t baseTimeBroadcast ;
  std::default_random_engine generatorSendDistribution;
  static std::normal_distribution<double> sendDistribution;

  cGate* outGate;

  ErrorDetectorClockDifference detector;
  statsNode stats;
  SimulationParameters* params;
  Controller* control;
};

#endif /* NODE_H_ */
