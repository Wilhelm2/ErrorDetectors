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

#include "StatsRecovery.h"

StatsRecovery::StatsRecovery() {
    // TODO Auto-generated constructor stub

}

StatsRecovery::~StatsRecovery() {
    // TODO Auto-generated destructor stub
}


void StatsRecovery::WriteTotalNbRecoveries()
{
    unsigned int totalNbRecoveries = 0;
    unsigned int totalNbDeliveries = 0;
    for(NodeRecovery* node: nodes )
    {
        totalNbRecoveries += node->statsRecovery.nbRecoveries;
        totalNbDeliveries += node->stats.nbDeliveredMessages;
        nbRecoveriesFile << node->statsRecovery.nbRecoveries << " ";
    }
    nbRecoveriesFile<<endl;
    nbRecoveriesFile<< totalNbRecoveries <<endl;
    nbRecoveriesFile << ((float)totalNbRecoveries) / totalNbDeliveries << endl;
    cerr << "Number of recoveries " << totalNbRecoveries <<endl;
    cerr << "Rate of recoveries " << ((float)totalNbRecoveries) / totalNbDeliveries << endl;
}


void StatsRecovery::writeNbAvoidedRecoveries()
{
    unsigned int nbAvoidedRecoveries = 0;
    unsigned int totalNbDeliveries = 0;
    for(NodeRecovery* n : nodes)
    {
        nbAvoidedRecoveries += n->statsRecovery.nbAvoidedRecoveries;
        totalNbDeliveries += n->stats.nbDeliveredMessages;
    }
    nbAvoidedRecoveriesFile << nbAvoidedRecoveries << " " << ((float)nbAvoidedRecoveries)/totalNbDeliveries<<endl;
}

void StatsRecovery::printBuffersStates()
{
    Stats::printBuffersStates();

    cerr<< "Number of messages to recover for each process :"<<endl;
    for(NodeRecovery* n : nodes)
        cerr<<n->messagesToRecover.size()<< " ";
    cerr<<endl;
}

void StatsRecovery::handleMessage(cMessage *msg)
{
    WriteTotalNbRecoveries();
    writeNbAvoidedRecoveries();
    Stats::handleMessage(msg);
}


