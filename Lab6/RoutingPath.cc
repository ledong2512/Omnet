/*
 * RoutingPath.cc
 *
 *  Created on: Apr 25, 2020
 *      Author: hongt
 */

#include "RoutingPath.h"
#include <omnetpp.h>
using namespace omnetpp;
using namespace std;
RoutingPath::RoutingPath(){

}

void RoutingPath::PrintPath(){
    EV<<"Path: ";
    for(auto v:path){

        EV<<v<<" ";

    }
    EV<<"\n";
}
