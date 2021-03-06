//
// This file is part of an OMNeT++/OMNEST simulation example.
//
// Copyright (C) 2004 Ahmet Sekercioglu
// Copyright (C) 2004-2008 Andras Varga
//
// This file is distributed WITHOUT ANY WARRANTY. See the file
// `license' for details on this and other legal matters.
//
#include <string.h>
#include <omnetpp.h>

using namespace  omnetpp;
class Txc4 : public cSimpleModule
{
private:
    int couter;
  protected:
    // The following redefined virtual function holds the algorithm.
    virtual void initialize();
    virtual void handleMessage(cMessage *msg);
};
// The module class needs to be registered with OMNeT++
Define_Module(Txc4);
void Txc4::initialize()
{
    // Initialize is called at the beginning of the simulation.
    // To bootstrap the tic-toc-tic-toc process, one of the modules needs
    // to send the first message. Let this be `tic'.
    // Am I Tic or Toc?
    couter=par("limit");
    WATCH(couter);
    if (par("sendMsgOnInit").boolValue())
    {
        // create and send first message on gate "out". "tictocMsg" is an
        // arbitrary string which will be the name of the message object.
        cMessage *msg = new cMessage("tictocMsg");
        send(msg, "out");
        EV << "Sending initial message\n";
    }
}
void Txc4::handleMessage(cMessage *msg)
{
    // The handleMessage() method is called whenever a message arrives
    // at the module. Here, we just send it to the other module, through
    // gate out'. Because both tic' and `toc' does the same, the message
    // will bounce between the two.
    couter--;
    if(couter>0){
        EV << "Received message `" << msg->getName()<<"couter:"<<couter << "', sending it out again\n";
         send(msg, "out");
    }else{
        delete msg;
    }

}
