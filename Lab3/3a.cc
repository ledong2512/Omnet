/*
 * 3a.cc
 *
 *  Created on: 5 thg 11, 2020
 *      Author: 123
 */




#include <stdio.h>
#include <string.h>
#include <omnetpp.h>
#include "3a_m.h"
using namespace omnetpp;
class Node : public cSimpleModule
{
  private:
    int sendPort=0;
  protected:

    virtual void initialize();
    virtual void handleMessage(cMessage *msg);
};

Define_Module(Node);
void Node::initialize()
{
    // Initialize is called at the beginning of the simulation.
    // To bootstrap the tic-toc-tic-toc process, one of the modules needs
    // to send the first message. Let this be `tic'.

    // Am I Tic or Toc?
    if (strcmp("node5", getName()) == 0)
    {
        // create and send first message on gate "out". "tictocMsg" is an
        // arbitrary string which will be the name of the message object.
        Lab3a *msg = new Lab3a("tictocMsg");
        send(msg, "out",0);
    }
}
void Node::handleMessage(cMessage *msg)
{
    // The handleMessage() method is called whenever a message arrives
    // at the module. Here, we just send it to the other module, through
    // gate `out'. Because both `tic' and `toc' does the same, the message
    // will bounce between the two.

    Lab3a *tmp=check_and_cast<Lab3a*>(msg);
    tmp->setHopCount(tmp->getHopCount()+1);
    if(gateSize("out")<1){
        EV<<"HopCount: "<<tmp->getHopCount();
        return;
    }
    else if(gateSize("out")>1){
        send(msg, "out",sendPort);
        sendPort++;
    }else
    {
    send(msg, "out",0);

    }
}
