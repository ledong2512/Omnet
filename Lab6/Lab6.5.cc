#include <omnetpp.h>
#include <queue>
//#include <limits>
#include <fstream>
#include<string>
#include "message_m.h"
#include "FatTreeGraph.h"
#include "FatTreeRoutingAlgorithm.h"
#include "stdio.h"
using namespace omnetpp;
using namespace std;
class Nodes: public cSimpleModule{
private:
    int EXB_SIZE;
    int type;
    int k;
    int BUFFER_COUNTER;
    int lastMessageId;
    int destination;
    int sumMsg;
    int *receivedMsgCount;
    int intervalCount = 0;
    double TIMEOUT;
    double TIME_INTERVAL;
    double TIME_GEN_MSG;
    double CHANNEL_DELAY;
    double CREDIT_DELAY;
    double TIME_OPERATION_OF_SWITCH;
    int arrayLength;

    FatTreeRoutingAlgorithm* ftra;
    FatTreeGraph fatTreeGraph;
    map<int, queue<cMessage *>> ENB, EXB;
    map<int, int> sizeOfNextENB; //lﾆｰu l蘯｡i s盻� buffer cﾃｲn tr盻創g t蘯｡i ENB t蘯｡i Node k蘯ｿ ti蘯ｿp
    map<int, bool> channelStatus; //bi蘯ｿn s盻ｭ d盻･ng ﾄ黛ｻ� check xem ﾄ柁ｰ盻拵g truy盻］ r蘯｣nh hay b蘯ｭn t蘯｡i t盻ｫng c盻貧g
    queue<int> SQ, EXB_SD; //Source queue vﾃ� exit buffer c盻ｧa sender
    queue<int> sendTo;

protected:
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
    virtual void finish() override;

    static int fin;
    //wrap function s盻ｭ d盻･ng cho t盻ｫng lo蘯｡i thi蘯ｿt b盻� sender, switch, receiver
    void senders(cMessage *msg);
    void switches(cMessage *msg);
    void receivers(cMessage *msg);

    //cﾃ｡c hﾃ�m s盻ｭ d盻･ng cho sender
    void generateMessage();
    void sendToExitBuffer_SD();
    void sendToSwitch();

    //cﾃ｡c hﾃ�m s盻ｭ d盻･ng cho switch
    void sendToExitBuffer_SW();
    bool checkENB();
    void sendToNextNode();
    //void sendSignalToNeighbor(int);
    void sendSignalToIncBuff(int);

    //cﾃ｡c hﾃ�m s盻ｭ d盻･ng cho receiver
    void sendSignalToIncBuff_RC();
};

Define_Module(Nodes);
int Nodes::fin=0;
void Nodes::initialize(){
    EXB_SIZE = par("EXB_SIZE").intValue();
    TIMEOUT = par("TIMEOUT").doubleValue();
    TIME_INTERVAL = par("TIME_INTERVAL").doubleValue();
    CREDIT_DELAY = par("CREDIT_DELAY").doubleValue();
    TIME_GEN_MSG = par("TIME_GEN_MSG").doubleValue();
    CHANNEL_DELAY = par("CHANNEL_DELAY").doubleValue();
    TIME_OPERATION_OF_SWITCH = par("TIME_OPERATION_OF_SWITCH").doubleValue();
    type = par("type").intValue();
    if (type == 2){
        k = par("k").intValue();
        fatTreeGraph = FatTreeGraph(k);
        ftra = new FatTreeRoutingAlgorithm(fatTreeGraph, true);
        ftra->PrintTables();
        //(ftra->path(10, 25)).PrintPath();
        //fatTreeGraph.printAddress();
        for (int i = 0; i < k; i++){
            cGate *g = gate("out", i);
            int index = g->getNextGate()->getOwnerModule()->getIndex();
            sendTo.push(index);
            sizeOfNextENB[index] = EXB_SIZE;
            channelStatus[index] = false;
        }
        scheduleAt(0 + TIME_OPERATION_OF_SWITCH, new cMessage("nextPeriod"));
        scheduleAt(0 + CHANNEL_DELAY, new cMessage("send"));
    }
    if (type == 1||type ==3){
        destination = par("des").intValue();
        BUFFER_COUNTER = EXB_SIZE;
        lastMessageId = -1;
        scheduleAt(0, new cMessage("generate"));
        scheduleAt(0 + CHANNEL_DELAY, new cMessage("send"));
    }
    if (type == 3||type ==1){
        sumMsg = 0;
        arrayLength = TIMEOUT / TIME_INTERVAL;
        receivedMsgCount = new int[arrayLength];
        memset(receivedMsgCount, 0, arrayLength * sizeof(int));
        scheduleAt(0, new cMessage("nextInterval"));
    }
}

void Nodes::handleMessage(cMessage *msg){
    if(type == 1||type ==3){
        //receivers(msg);
        senders(msg); // x盻ｭ lﾃｽ gﾃｳi tin ﾄ黛ｺｿn nﾃｺt g盻ｭi

    }else if(type == 2){
        switches(msg); //x盻ｭ lﾃｽ gﾃｳi tin ﾄ黛ｺｿn switch
    }
}

void Nodes::senders(cMessage *msg){
    if(simTime() >= TIMEOUT){
        delete msg;
        return;
    }

    //sinh gﾃｳi tin ﾄ黛ｻ杵h k盻ｳ
    if(!strcmp(msg->getName(), "generate")){
        generateMessage();
        if(EXB_SD.size() < EXB_SIZE)
            sendToExitBuffer_SD();
        scheduleAt(simTime() + TIME_GEN_MSG, msg);
    }

    //g盻ｭi gﾃｳi tin sang switch
    if(!strcmp(msg->getName(), "send")){
        if(BUFFER_COUNTER > 0 && EXB_SD.size() > 0){
            sendToSwitch();
            sendToExitBuffer_SD();
            --BUFFER_COUNTER;
        }
        scheduleAt(simTime() + CHANNEL_DELAY, msg);
    }

    //tﾄハg buffer c盻ｧa nﾃｺt k蘯ｿ ti蘯ｿp sau 1 kho蘯｣ng th盻拱 gian CREDIT_DELAY
    if(!strcmp(msg->getName(), "inc buffer")){
        scheduleAt(simTime() + CREDIT_DELAY, new cMessage("incBuff"));
        delete msg;
        return;
    }

    //tﾄハg buffer
    if(!strcmp(msg->getName(), "incBuff")){
        ++BUFFER_COUNTER;
        delete msg;
        return;
    }
    const char * eventName = msg->getName();
    if(!strcmp(eventName, "sender to receiver")){
            sendSignalToIncBuff_RC();
            sumMsg++;
            receivedMsgCount[intervalCount]++;
            delete msg;
            return;
        }
        if (!strcmp(msg->getName(), "nextInterval")) {
            intervalCount++;
            scheduleAt(simTime() + TIME_INTERVAL, msg);
        }
}

//Sinh gﾃｳi tin
void Nodes::generateMessage(){
    SQ.push(++lastMessageId);
}

//g盻ｭi gﾃｳi tin t盻嬖 exitbuffer
void Nodes::sendToExitBuffer_SD(){
    if( !SQ.empty()){
        int msgId = SQ.front();
        SQ.pop();
        EXB_SD.push(msgId);
    }
}

//g盻ｭi gﾃｳi tin t盻嬖 switch
void Nodes::sendToSwitch(){
    int sendMsgId = EXB_SD.front();
    EXB_SD.pop();
    sendMsg *sMsg = new sendMsg("sender to receiver");
    sMsg->setSource(getIndex());
    sMsg->setDestination(destination);
    sMsg->setPayload(sendMsgId);
    send(sMsg, "out", 0);
}

/**
 * nh蘯ｭn gﾃｳi tin mﾃ� cﾃ｡c node g盻ｭi ﾄ黛ｺｿn
 * lﾆｰu message vﾃ�o ENB tﾆｰﾆ｡ng 盻ｩng
 * g盻ｭi thﾃｴng bﾃ｡o ﾄ柁ｰ盻拵g truy盻］ r蘯｣nh cho node trﾆｰ盻嫩
 */
void Nodes::switches(cMessage *msg){
    if(simTime() >= TIMEOUT){
        delete msg;
        EV << "Time out" << endl;
        return;
    }
    const char * eventName = msg->getName();
    //chuy盻ハ gﾃｳi tin vﾃ�o ENB tﾆｰﾆ｡ng 盻ｩng khi switch nh蘯ｭn ﾄ柁ｰ盻｣c gﾃｳi tin
    if(!strcmp(eventName, "sender to receiver")){
        int index = msg->getSenderModule()->getIndex();
        queue<cMessage *> temp;
        if (ENB[index].empty()){
            temp.push(msg);
            ENB[index] = temp;
        } else {
            temp = ENB[index];
            temp.push(msg);
            ENB[index] = temp;
        }
    }
    //chu kì hoạt động switch
    if(!strcmp(eventName, "nextPeriod")){
        if(ENB.size() > 0){
            sendToExitBuffer_SW();
        }
        //
        ftra->shufflerPort(getIndex());
        scheduleAt(simTime() + TIME_OPERATION_OF_SWITCH, msg);
    }
    //tﾄハg buffer khi nh蘯ｭn ﾄ柁ｰ盻｣c tﾃｭn hi盻㎡ t盻ｫ nﾃｺt k盻� tﾆｰﾆ｡ng 盻ｩng sau kho蘯｣ng th盻拱 gian CREDIT_DELAY
    if (!strcmp(eventName, "inc buffer")){
        cMessage *sendMsg = new cMessage("incbuff");
        sendMsg->addPar("preNode");
        sendMsg->par("preNode").setLongValue(msg->getSenderModule()->getIndex());
        scheduleAt(simTime() + CREDIT_DELAY, sendMsg);
        delete msg;
        return;
    }
    //tﾄハg buffer
    if (!strcmp(eventName, "incbuff")){
        int index = msg->par("preNode").longValue();
        ++sizeOfNextENB[index];
        delete msg;
        return;
    }
    //g盻ｭi gﾃｳi tin t盻嬖 nﾃｺt k蘯ｿ ti蘯ｿp
    if(!strcmp(eventName, "send")){
        sendToNextNode();
        scheduleAt(simTime() + CHANNEL_DELAY, msg);
    }

}
//Tﾃｬm gﾃｳi tin mu盻創 chuy盻ハ sang exitbuffer
void Nodes::sendToExitBuffer_SW(){
    sendMsg *ttmsg;
    queue<cMessage *> temp;
    cMessage *mess;
    for(int i = 0; i < k; i++){
        cGate *g = gate("out", i);
        int index = g->getNextGate()->getOwnerModule()->getIndex();
        int id = numeric_limits<int>::max();
        int location = -1;
        for(map<int, queue<cMessage *>>::iterator it = ENB.begin(); it != ENB.end(); it++){
            temp = it->second;
            if(!temp.empty()){
                ttmsg = check_and_cast<sendMsg *>(temp.front());
                int payload = ttmsg->getPayload();
                if(ftra->next(ttmsg->getSource(), getIndex(), ttmsg->getDestination()) == index){
                    if (payload < id){
                        id = payload;
                        location = it->first;
                    }
                }
            }
        }
        if(location > -1){
           mess = ENB[location].front();
           if(EXB[index].size() < EXB_SIZE){
               EXB[index].push(mess);


               sendSignalToIncBuff(location);
               ENB[location].pop();
           }
        }
    }
}
//g盻ｭi tﾃｭn hi盻㎡ tﾄハg buffer cho nﾃｺt tﾆｰﾆ｡ng ﾆｰ盻嬾g khi cﾃｳ gﾃｳi tin ra kh盻淑 ENB 盻ｩng v盻嬖 nﾃｺt ﾄ妥ｳ
void Nodes::sendSignalToIncBuff(int nodeIndex){
    int index;
    for (int i = 0; i < k; i++){
        cGate *g = gate("out", i);
        index = g->getNextGate()->getOwnerModule()->getIndex();
        if(index == nodeIndex){
            send(new cMessage("inc buffer"), "out", i);
            break;
        }
    }
}
//g盻ｭi gﾃｳi tin sang nﾃｺt ti蘯ｿp theo
void Nodes::sendToNextNode(){
    for (int i = 0; i < k; i++){
        cGate *g = gate("out", i);
        int index = g->getNextGate()->getOwnerModule()->getIndex();
        if(sizeOfNextENB[index] > 0){
            if (!EXB[index].empty()){
                sizeOfNextENB[index]--;
                cMessage * mess = EXB[index].front()->dup();
                ftra->increaseTraficPorts(i, getIndex());
                sendMsg* ttmsg = check_and_cast<sendMsg *>(mess);
                ftra->increaseTraficTables(ttmsg->getSource(),ttmsg->getDestination(),getIndex());
                send(mess, "out", i);
                delete EXB[index].front();
                EXB[index].pop();
            }
        }
    }
}
//x盻ｭ lﾃｽ tin nh蘯ｯn ﾄ黛ｺｿn nﾃｺt ﾄ妥ｭch
void Nodes::receivers(cMessage *msg){
    if (simTime() >= TIMEOUT){
        delete msg;
        return;
    }
    const char * eventName = msg->getName();
    if(!strcmp(eventName, "sender to receiver")){
        sendSignalToIncBuff_RC();
        sumMsg++;
        receivedMsgCount[intervalCount]++;
        delete msg;
        return;
    }
    if (!strcmp(msg->getName(), "nextInterval")) {
        intervalCount++;
        scheduleAt(simTime() + TIME_INTERVAL, msg);
    }
}

void Nodes::sendSignalToIncBuff_RC(){
    send(new cMessage("inc buffer"), "out", 0);
}
//tﾃｭnh toﾃ｡n s盻� gﾃｳi tin nh蘯ｭn ﾄ柁ｰ盻｣c theo t盻ｫng interval lﾆｰu vﾃ�o file
void Nodes::finish(){
    fin++;
    if(type == 3||type == 1){
        fstream fileOutput, fileInput;
        int preArr[arrayLength];
        for (int i = 0; i < arrayLength; i++)
            preArr[i] = 0;
        fileInput.open(("D:/output.txt"), ios::in);
        if(fileInput){
            int i = 0;
            while (fileInput >> preArr[i])
                i++;
        }
        fileInput.close();
        fileOutput.open(("D:/output.txt"), ios::out);
        for (int i = 0; i < arrayLength; i++) {
            fileOutput << int(receivedMsgCount[i] + preArr[i]) << " ";
        }
        fileOutput << endl;
        fileOutput.close();
        cout << "Node " << getIndex() << " received : "  << sumMsg << " message." <<  endl;
        EV << "Node " << getIndex() << " received : "  << sumMsg << " message." <<  "\n";
        for (int i = 0; i < arrayLength; i++) {
                    EV << int(receivedMsgCount[i] + preArr[i]) << " ";
                }
        EV<<"\n";
    }
    //return;
    if(fin==36){
        //duong dan file chay gnuplot
        FILE *gnplt = popen("c:\\gnuplot\\bin\\gnuplot","w");

              // fprintf(gnplt,"plot sin(x) \n");
        EV<<"plot \"E:\\output.txt\"  matrix u ($1*100):($0*100/8) every 1::1 with lines ls 2  \n";
        fprintf(gnplt,"set xlabel \"Time(ns)\" \n");
        fprintf(gnplt,"set ylabel \"Throughtput(%)\" \n");
         fprintf(gnplt,"plot \"D:/output.txt\"  matrix u ($1*100):($0*100/16) every 1::1 with lines ls 2  \n");
         fflush(gnplt);


    }

}
