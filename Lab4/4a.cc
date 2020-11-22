#include <stdio.h>
#include <string.h>
#include <omnetpp.h>
using namespace omnetpp;
class Switch : public cSimpleModule
{
    virtual void initialize();
    virtual void handleMessage(cMessage *msg);
};
Define_Module(Switch);
void Switch::initialize()
{
}
void Switch::handleMessage(cMessage *msg)
{
}


class Host : public cSimpleModule
{
    virtual void initialize();
    virtual void handleMessage(cMessage *msg);
};
Define_Module(Host);
void Host::initialize()
{
}
void Host::handleMessage(cMessage *msg)
{
}
