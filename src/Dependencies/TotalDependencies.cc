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

#include "TotalDependencies.h"

TotalDependencies::TotalDependencies() {
    // TODO Auto-generated constructor stub

}

TotalDependencies::TotalDependencies(const vector<unsigned int>& dependencies)
{
    this->dependencies = dependencies;
}

TotalDependencies::TotalDependencies(unsigned int size)
{
    dependencies.resize(size,0);
}

TotalDependencies::~TotalDependencies() {
    // TODO Auto-generated destructor stub
}

unsigned int& TotalDependencies::operator[](unsigned int i)
{
    return dependencies[i];
}

unsigned int TotalDependencies::operator[](unsigned int i) const
{
    return dependencies[i];
}

bool TotalDependencies::operator==(const TotalDependencies& compareDep)
{
    for(unsigned int i=0; i<dependencies.size(); i++)
        if(dependencies[i] != compareDep.getDependencies()[i])
            return false;
    return true;
}

bool TotalDependencies::operator<(const TotalDependencies& compareDep)
{
    for(unsigned int i=0; i<dependencies.size(); i++)
        if(dependencies[i] < compareDep[i])
            return true;
    return false;
}

vector<unsigned int> TotalDependencies::getDependencies() const
{
    return dependencies;
}

bool TotalDependencies::includesDependencies(const TotalDependencies& dependenciesToCheck)
{
    for(unsigned int i=0; i<dependencies.size(); i++)
        if(dependencies[i] < dependenciesToCheck[i])
            return false;
    return true;
}

bool TotalDependencies::includesDependencies(const vector<idMsg>& dependenciesToCheck)
{
    for(const idMsg& id : dependenciesToCheck)
        if(dependencies[id.id] < id.seq)
            return false;
    return true;
}

void TotalDependencies::print()
{
    for(unsigned int elt : dependencies)
        cout << elt << "\t";
    cout<< endl;
}

void TotalDependencies::printErr() const
{
    for(unsigned int elt : dependencies)
        cerr << elt << "\t";
    cerr<< endl;
}

bool TotalDependencies::SatisfiesDeliveryConditions(const TotalDependencies& MessageDependencies, unsigned int idMessageSender)
{
    for(unsigned int i=0; i < dependencies.size(); i++)
    {
        if(MessageDependencies[i]==0)// No causal dependency + necessary because MessageDependencies[i] -1 provokes buffer underflow
            continue;
        if(i == idMessageSender)
        {
            if( dependencies[i] < (MessageDependencies[i]-1) )
                return false;
        }
        else
        {
            if( dependencies[i] < MessageDependencies[i] )
                return false;
        }
    }
    return true;
}

void TotalDependencies::printComparisionWith(const TotalDependencies& dep)
{
    for(unsigned int i=0; i<dependencies.size(); i++)
    {
        if(dependencies[i] != dep[i])
            cerr << "entry " << i << " message dependency " << dependencies[i] << " and value of compared set of dependencies " << dep[i]<<endl;
    }
}

unsigned int TotalDependencies::size() const
{
    return dependencies.size();
}
