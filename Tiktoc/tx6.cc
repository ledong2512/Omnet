//
// This file is part of an OMNeT++/OMNEST simulation example.
//
// Copyright (C) 2006 Ahmet Sekercioglu
// Copyright (C) 2006-2008 Andras Varga
//
// This file is distributed WITHOUT ANY WARRANTY. See the file
// `license' for details on this and other legal matters.
//
#include <string.h>
#include <omnetpp.h>

using namespace  omnetpp;
class Txc6 : public cSimpleModule
{
public:
    Txc6();
    ~Txc6();
private:
    cMessage *Tmsg;
    cMessage *event;
    int couter;
  protected:
    // The following redefined virtual function holds the algorithm.
    virtual void initialize();
    virtual void handleMessage(cMessage *msg);
};
Txc6::Txc6(){
    event=Tmsg=NULL;
}
Txc6::~Txc6(){
    cancelAndDelete(event);
    delete Tmsg;
}
// The module class needs to be registered with OMNeT++
Define_Module(Txc6);
void Txc6::initialize()
{
    // Initialize is called at the beginning of the simulation.
    // To bootstrap the tic-toc-tic-toc process, one of the modules needs
    // to send the first message. Let this be `tic'.
    // Am I Tic or Toc?
    event = new cMessage("event");


       Tmsg = NULL;
    if (strcmp("tic", getName()) == 0)
        {
            // We don't start right away, but instead send an message to ourselves
            // (a "self-message") -- we'll do the first sending when it arrives
            // back to us, at t=5.0s simulated time.
            EV << "Scheduling first send to t=5.0s\n";
            Tmsg = new cMessage("tictocMsg");
            scheduleAt(5.0, event);
        }
}
void Txc6::handleMessage(cMessage *msg)
{
    // The handleMessage() method is called whenever a message arrives
    // at the module. Here, we just send it to the other module, through
    // gate out'. Because both tic' and `toc' does the same, the message
    // will bounce between the two.

    if (msg==event)
        {

            EV << "Wait period is over, sending back message\n";
            send(Tmsg, "out");
            Tmsg = NULL;
        }
        else
        {

            Tmsg = msg;
            scheduleAt(simTime()+1.0, event);
        }

}
