////
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

#include "Controller.h"

Controller::Controller() {
}

Controller::~Controller() {
}

void Controller::initialize(int stage)
{
    cSimpleModule::initialize(stage);
    params =  dynamic_cast<SimulationParameters*> (getModuleByPath((string(PACKAGENAME)+".simParams").c_str()));
    processDependencies.resize(params->nbNodes);
    for(TotalDependencies& it : processDependencies)
        it = TotalDependencies(params->nbNodes);
    processBroadcastedMessages.resize(params->nbNodes);
}

void Controller::notifySendMessage(unsigned int idSender,unsigned  int seq)
{
    msg t;
    t.seq = seq;
    processDependencies[idSender][idSender]++;
    t.dependencies = processDependencies[idSender];
    t.psHasDelivered.assign(params->nbNodes, false);
    t.sendTime = simTime();
    t.deliveredAtTime.assign(params->nbNodes, 0);
    processBroadcastedMessages[idSender].push_back(t);
//    std::cout << "BROADCASTCONTROLLER: MSG ADDED from " << idSrc <<" seq " << t->seq << " || "; printclock(t->clock);
}

vector<msg>::iterator Controller::searchMessage(idMsg idM)
{
    if(processBroadcastedMessages.size() < idM.id)
        throw "Source process not contained in processBroadcastedMessages";

    for(vector<msg>::iterator it = processBroadcastedMessages[idM.id].begin(); it != processBroadcastedMessages[idM.id].end(); ++it)
    {
        if(it->seq == idM.seq)
            return it;
    }
    throw "Message not contained in vector of sent messages of process";
}

bool Controller::notifyDeliverMessage(idMsg idM, unsigned int idDest)
{
    vector<msg>::iterator m = searchMessage(idM);
    bool deliverInCausalOrder= canCausallyDeliverMessage(idM,idDest);

//    if(!deliverInCausalOrder)
//        printDeliveryError("Out of causal order delivery", idM, idDest, m.dependencies, processDependencies[idDest]);
    processDependencies[idDest][idM.id] = max(m->dependencies[idM.id],processDependencies[idDest][idM.id]); // increments local clock of idDest MUST BE MAX TO HANDLE THE CASE WHERE DELIVERED A MESSAGE OUT OF CAUSAL ORDER
    if(m->psHasDelivered[idDest])
    {
        printDeliveryError("Multiple delivery", idM, idDest, m->dependencies, processDependencies[idDest]);
        exit(0);
    }

    m->deliveredAtNbPs++;
    m->psHasDelivered[idDest] = true;
    m->deliveredAtTime[idDest] = simTime();
    if(m->deliveredAtNbPs == params->nbNodes) // delete le message car reçu par tous les mobiles ET stations
        deleteMessage(idM);
    return deliverInCausalOrder;
}

void Controller::printDeliveryError(string errorReason, idMsg idM, unsigned int destProcess, const TotalDependencies& messageDependencies, const TotalDependencies& processDependencies)
{
    cerr <<"BroadcastController " <<  errorReason << " idMsg " << idM.id << ","<< idM.seq << " destination " << destProcess<<endl;
    cerr << "Process clock: " ;
    processDependencies.printErr();
    cerr << "Message clock: ";
    messageDependencies.printErr();
}

bool Controller::canCausallyDeliverMessage(idMsg idM, unsigned int idDest)
{
    vector<msg>::iterator m = searchMessage(idM);
    return(processDependencies[idDest].SatisfiesDeliveryConditions(m->dependencies, idM.id));
}

void Controller::deleteMessage(idMsg idM)
{
//    std::cerr<< "BROADCASTCONTROLLER: DELETE MESSAGE (" << idM.id <<"," <<idM.seq<<")"<<endl;
    vector<msg>::iterator it;
    for(it = processBroadcastedMessages[idM.id].begin(); it != processBroadcastedMessages[idM.id].end(); ++it )
        if( it->seq == idM.seq)
            break;
    if(it == processBroadcastedMessages[idM.id].end())
        throw "Message not found";
    processBroadcastedMessages[idM.id].erase(it);
    return;
}

bool Controller::isDependency(idMsg idM, idMsg idDep)
{
    vector<msg>::iterator m = searchMessage(idM);
    return m->dependencies[idDep.id] >= idDep.seq;
}

