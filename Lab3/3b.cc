/*
 * 3b.cc
 *
 *  Created on: 17 thg 11, 2020
 *      Author: 123
 */
#include <stdio.h>
#include <string.h>
#include <omnetpp.h>
using namespace omnetpp;
class WirelessNode : public cSimpleModule{
public:
    static float matrix[100][100];
    static int start;
    static int list[100][2];
    static int visited[100];
    virtual void initialize();
    virtual void handleMessage(cMessage *msg);
    void calculateMatrix();
    int getClosestPossible(int index);
};
Define_Module(WirelessNode);
int WirelessNode::start=0;
int WirelessNode::list[100][2]={0,0};
float WirelessNode::matrix[100][100]={0};
int WirelessNode::visited[100]={0,0,0};
void WirelessNode::initialize(){
    visited[start]=0;
    start++;
    int x=intuniform(0, 800);
    int y=intuniform(0, 800);
    getDisplayString().setTagArg("p", 0, x);
    getDisplayString().setTagArg("p", 1, y);
    list[getIndex()][0]=x;
    list[getIndex()][1]=y;
    int ix=getIndex();
    EV<<ix<<" "<<visited[ix]<<"\n";



    if(start==100){
        calculateMatrix();
        float maxx=0.0f;
        for(int i=0;i<100;i++){
                for(int j=0;j<100;j++){
                    if(matrix[i][j]>maxx) maxx=matrix[i][j];
                }
        }

        EV<<"MAX: "<<maxx;
        // chuan hoa
        for(int i=0;i<100;i++){
               for(int j=0;j<100;j++){
                   matrix[i][j]/=maxx;
               }
        }

        WATCH(matrix);
        //send first
         cModule *target=getParentModule()->getSubmodule("node", getIndex());
         sendDirect(new cMessage(""), target,"radioIn");
    }
    }
void WirelessNode::handleMessage(cMessage *msg){
    visited[getIndex()]=1;
    int n=getClosestPossible(getIndex());
    if(n==101) return;
    cModule *target=getParentModule()->getSubmodule("node", n);
    sendDirect(msg, target,"radioIn");

}
void WirelessNode::calculateMatrix(){

    for(int i=0;i<100;i++){
        for(int j=0;j<100;j++){
            matrix[i][j]=sqrt((list[i][0]-list[j][0])*(list[i][0]-list[j][0])+(list[i][1]-list[j][1])*(list[i][1]-list[j][1]));

        }
    }
}
int WirelessNode::getClosestPossible(int index){
    float minn=2.0f;
    int cp=101;
    for(int i=0;i<100;i++){

        if(i==index) continue;

        if(visited[i]==0&&(matrix[index][i]<minn)){
            minn=matrix[index][i];
            cp=i;
        }
    }
    EV<<"Next hop:"<<cp<<" Distance: "<<minn<<"\n";
    return cp;
}
