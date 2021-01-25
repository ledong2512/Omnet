#include "FatTreeRoutingAlgorithm.h"
#include <omnetpp.h>
#include<stdio.h>
#include <time.h>
#include <stdlib.h>
using namespace omnetpp;
using namespace std;
FatTreeRoutingAlgorithm::FatTreeRoutingAlgorithm(FatTreeGraph G, bool precomputed) {
    this->G = G;
    buildTables();
    if (precomputed){
        vector<int> hosts = G.hosts();
        for (int i = 0; i < hosts.size() - 1; i++){
            for (int j = i + 1; j < hosts.size(); j++){
                int source = hosts[i];
                int destination = hosts[j];
                path(source, destination);
            }
        }
    }
}

void FatTreeRoutingAlgorithm::buildTables(){
    // TODO - build prefix - suffix routing table
    int k = G.getK();
    int numEachPod = k * k / 4 + k;
//    for(int i=0;i<40;i++){
//        flowTraficPorts[i][2]=0;
//        flowTraficPorts[i][3]=0;
//    }

    //edge switches
    for (int p = 0; p < k; p++){
        int offset = numEachPod * p;;
        for (int e = 0; e < k / 2; e++){
            int edgeSwitch = offset + k * k / 4 + e;
            //create suffix table
            map<int, int> suffixTable;
            for(int suffix = 2; suffix <= k/2 + 1; suffix++){
                int agg = offset + k * k / 4 + (0+ suffix -2) % (k / 2) + (k / 2);
                suffixTable.insert(pair<int, int>(suffix, agg));
            }
            suffixTables.insert(pair<int,map<int, int>>(edgeSwitch, suffixTable));
        }

    }

    //agg switches
    for (int p = 0; p < k; p++){
        int offset = numEachPod * p;
        for (int a = 0; a < k / 2; a++){
            int aggSwitch = offset + k * k / 4 + k / 2 + a;

            //create suffix table
            map<int, int> suffixTable;
            for (int suffix = 2; suffix <= k / 2 + 1; suffix++){
               int core = a * k / 2 + (suffix + a - 2) % (k /  2) + numEachPod * k;
               suffixTable.insert(pair<int, int>(suffix, core));
            }
            //inject to the behavior
            suffixTables.insert(pair<int, map<int, int>>(aggSwitch, suffixTable));

            //create prefix table
            map<int, int> prefixTable;
            for (int e = 0; e < k / 2; e++){
                int edgeSwitch = offset + k * k / 4 + e;
                int temp = (10 << 16) | (p << 8) | e;
                prefixTable.insert(pair<int, int>(temp, edgeSwitch));
            }
            prefixTables.insert(pair<int, map<int, int>>(aggSwitch, prefixTable));
        }
    }

    //core switches
    for (int c = 0; c < k * k / 4; c++){
        int core = k * k * k / 4 + k * k + c;

        // build core prefix
        map<int, int> corePrefixTable;
        for (int p = 0; p < k; p++){
            int offset = numEachPod * p;
            int agg = (c / (k / 2)) + k / 2 + k * k / 4 + offset;
            int temp = (10 << 8) | p;
            corePrefixTable.insert(pair<int, int>(temp, agg));
        }
        corePrefixTables.insert(pair<int, map<int, int>>(core, corePrefixTable));
    }
}

void FatTreeRoutingAlgorithm::PrintTables()
{
    // print tables
    EV << "suffixTables" << "\n";
    for (map<int, map<int, int>>::iterator it0 = suffixTables.begin(); it0 != suffixTables.end(); ++it0) {
        EV << "suffixTable: " << G.getAddress(it0->first).toString() << ": " << "\n";
        for (map<int, int>::iterator it1 = it0->second.begin(); it1 != it0->second.end(); ++it1) {
            EV << "Suffix : " << "0.0.0."<< it1->first << "/8 - " << " Output Port: " << it1->first << " - Address Out: " << G.getAddress(it1->second).toString() << "\n";
        }
    }
    EV << "\n";;

    EV << "prefixTables" << "\n";
    for (map<int, map<int, int>>::iterator it0 = prefixTables.begin(); it0 != prefixTables.end(); ++it0) {
        EV << "prefixTable: " << G.getAddress(it0->first).toString() << ": " << "\n";
        for (map<int, int>::iterator it1 = it0->second.begin(); it1 != it0->second.end(); ++it1) {
            int ip = it1->first;
            int out = it1->second;
            int ip_0 = (ip >> 16);
            int ip_1 = ((ip << 16) >> 24);
            int ip_2 = ((ip << 24) >> 24);
            EV << "Prefix : " << ip_0 << "." << ip_1 << "." << ip_2 << ".0/24 - " << " Output Port: " << ip_2 <<" - Address Out: "<< G.getAddress(out).toString() << "\n";
        }
        EV << "\n";
    }

    EV << "corePrefixTables" << "\n";
    for (map<int, map<int, int>>::iterator it0 = corePrefixTables.begin(); it0 != corePrefixTables.end(); ++it0) {
        EV << "corePrefixTable: " << G.getAddress(it0->first).toString() << ": " << "\n";
        for (map<int, int>::iterator it1 = it0->second.begin(); it1 != it0->second.end(); ++it1) {
            int ip = it1->first;
            int out = it1->second;
            int ip_0 = (ip >> 8);
            int ip_1 = ((ip << 24) >> 24);
            EV << ip_0 << "." << ip_1 << ".0.0/16 - " << " Output Port: " << ip_1 << " - Address Out: " << G.getAddress(out).toString() << "\n";
        }
        EV << "\n";
    }
    EV << "\n";;
}

int FatTreeRoutingAlgorithm::next(int source, int current, int destination) {
    if (G.isHostVertex(current)) {
        return G.adj[current][0];
    }
    else if (std::find(G.adj[current].begin(), G.adj[current].end(), destination) != G.adj[current].end()) {
        return destination;
    }
    else {
        int type = G.switchType(current);
        if (type == FatTreeGraph::CORE) {

            Address address = G.getAddress(destination);
            int prefix = (address._1 << 8) | address._2;
            map<int, int> corePrefixTable = corePrefixTables[current];
            return corePrefixTable[prefix];
        }
        else if (type == FatTreeGraph::AGG) {

            Address address = G.getAddress(destination);

            int prefix = (address._1 << 16) | (address._2 << 8) | address._3;
            int suffix = address._4;

            map<int, int> prefixTable = prefixTables[current];
            map<int, int> suffixTable = suffixTables[current];
            map< pair<int,int>,int> flowTable=flowTables[current];
            if (prefixTable.count(prefix)) {
                return prefixTable[prefix];
            }
            else {
//                srand((int)time(0));
//                suffix=2+rand()%2;
               //return suffixTable[suffix];
                if(flowTable.find(make_pair(source,destination))==flowTable.end()){
                                flowTables[current][make_pair(source,destination)]=suffixTable[suffix];
                                    return suffixTable[suffix];
                                            }
                            else
                                    return flowTables[current][make_pair(source,destination)];
            }
        }
        else { // Edge switch

            Address address = G.getAddress(destination);
            int suffix = address._4;

            map<int, int> suffixTable = suffixTables[current];
            map< pair<int,int>,int> flowTable=flowTables[current];
//            srand((int)time(0));
//            suffix=2+rand()%2;
            //return suffixTable[suffix];
            if(flowTable.find(make_pair(source,destination))==flowTable.end()){
                flowTables[current][make_pair(source,destination)]=suffixTable[suffix];
                    return suffixTable[suffix];
                            }
            else
                    return flowTables[current][make_pair(source,destination)];
        }
    }
}

RoutingPath FatTreeRoutingAlgorithm::path(int source, int destination) {

    if (precomputedPaths.count(pair<int, int>(source, destination))) {
        return precomputedPaths[pair<int, int>(source, destination)];
    }
    else {
        RoutingPath rp;
        int current = source;
        while (current != destination) {
            if (current != source) {
                rp.path.push_back(current);
            }
            current = next(source, current, destination);
        }
        precomputedPaths.insert(pair<pair<int, int>, RoutingPath>(pair<int, int>(source, destination), rp));
        return rp;
    }
}

void FatTreeRoutingAlgorithm::increaseTraficPorts(int port,int current){// tăng bộ đếm số lần gói tin đi qua cổng (chỉ cần quan tâm cổng 2 và 3)
    int k = G.getK();
    if(port>=k/2&&port<k)
    //if(port==2||port==3)
    flowTraficPorts[current][port]++;
}
void FatTreeRoutingAlgorithm::increaseTraficTables(int source,int destination,int current){// tăng bộ đếm số lần 1 cặp nguồn đích được chuyển đi
    //flowTraficTables[current][make_pair(source,destination)]++;

    if (G.isHostVertex(current)) {
           return ;
       }
       else if (std::find(G.adj[current].begin(), G.adj[current].end(), destination) != G.adj[current].end()) {
           return ;
       }
       else {
           int type = G.switchType(current);
           if (type == FatTreeGraph::CORE) {
               return ;
           }
           else if (type == FatTreeGraph::AGG) {
               Address address = G.getAddress(destination);

               int prefix = (address._1 << 16) | (address._2 << 8) | address._3;
               int suffix = address._4;

               map<int, int> prefixTable = prefixTables[current];
               map<int, int> suffixTable = suffixTables[current];
               map< pair<int,int>,int> flowTable=flowTables[current];
               if (prefixTable.count(prefix)) {
                   return ;
               }
               else {
                   flowTraficTables[current][make_pair(source,destination)]++;
               }
           }
           else { // Edge switch

               flowTraficTables[current][make_pair(source,destination)]++;
           }
       }



}
void FatTreeRoutingAlgorithm::shufflerPort(int current){// tính toán lại để chuyển hướng các luồng giá trị ra
    int minn=9999999,maxx=0;
    int idxMin=0,idxMax=0;
    int k = G.getK();
    map<int,int> flowTraficPort=flowTraficPorts[current];
    for(int i=k/2;i<k;i++){
        if(flowTraficPort[i]>maxx){
            idxMax=i;
            maxx=flowTraficPort[i];
        }
        if(flowTraficPort[i]<minn){
            idxMin=i;
            minn=flowTraficPort[i];
             }

    }
//    maxx=flowTraficPort[2];idxMax=2;
//    minn=flowTraficPort[3];idxMin=3;
//    if(flowTraficPort[2]<flowTraficPort[3]){
//        maxx=flowTraficPort[3];idxMax=3;
//        minn=flowTraficPort[2];idxMin=2;
//    }
    int d=maxx-minn;

    pair<int,int> tmp,array[10];
    int count=0;
    bool check=false;
    map<pair<int,int>,int> flowTraficTable=flowTraficTables[current];
    map<pair<int,int>,int> flowTable=flowTables[current];
    for(map<pair<int,int>,int>::iterator it = flowTraficTable.begin(); it != flowTraficTable.end(); it++){
        if(it->second<d&&flowTables[current][it->first]==suffixTables[current][idxMax]){
            tmp=it->first;
            check=true;

          // break;
        }
    }
    if(check){

        EV<<"Node: "<<tmp.first<<" "<<tmp.second<<" "<<idxMax<<"\n";
        flowTables[current][tmp]=suffixTables[current][idxMin];


    }
    flowTraficPorts[current][3]=0;flowTraficPorts[current][2]=0;
              flowTraficTables[current].clear();
}
void FatTreeRoutingAlgorithm::printPath(int current){
    if(flowTables.find(current)==flowTables.end()) return;
    map<pair<int,int>,int> flowTable=flowTables[current];
    for(map<pair<int,int>,int>::iterator it =flowTable.begin(); it !=flowTable.end(); it++){
        EV<<"Node: "<<it->first.first<<" "<<it->first.second<<" "<<it->second<<"\n";
        }
}
void FatTreeRoutingAlgorithm::makePath(int source,int destination,int current){
    if (G.isHostVertex(current)) {
               return ;
           }
           else if (std::find(G.adj[current].begin(), G.adj[current].end(), destination) != G.adj[current].end()) {
               return ;
           }
           else {
               int type = G.switchType(current);
               if (type == FatTreeGraph::CORE) {
                   return ;
               }
               else if (type == FatTreeGraph::AGG) {
                   Address address = G.getAddress(destination);

                   int prefix = (address._1 << 16) | (address._2 << 8) | address._3;
                   int suffix = address._4;

                   map<int, int> prefixTable = prefixTables[current];
                   map<int, int> suffixTable = suffixTables[current];
                   map< pair<int,int>,int> flowTable=flowTables[current];
                   if (prefixTable.count(prefix)) {
                       return ;
                   }
                   else {
                       int minn=9999999,maxx=0;
                          int idxMin=0,idxMax=0;
                          map<int,int> flowTraficPort=flowTraficPorts[current];
                          maxx=flowTraficPort[2];idxMax=2;
                          minn=flowTraficPort[3];idxMin=3;
                          if(flowTraficPort[2]<flowTraficPort[3]){
                              maxx=flowTraficPort[3];idxMax=3;
                              minn=flowTraficPort[2];idxMin=2;
                          }
                           suffix=idxMin;

                           if(flowTable.find(make_pair(source,destination))==flowTable.end())
                                      flowTables[current][make_pair(source,destination)]=suffixTable[suffix];
                   }
               }
               else { // Edge switch
                   Address address = G.getAddress(destination);
                               int suffix = address._4;

                               map<int, int> suffixTable = suffixTables[current];
                               map< pair<int,int>,int> flowTable=flowTables[current];
                               int minn=9999999,maxx=0;
                                  int idxMin=0,idxMax=0;
                                  map<int,int> flowTraficPort=flowTraficPorts[current];
                                  maxx=flowTraficPort[2];idxMax=2;
                                  minn=flowTraficPort[3];idxMin=3;
                                  if(flowTraficPort[2]<flowTraficPort[3]){
                                      maxx=flowTraficPort[3];idxMax=3;
                                      minn=flowTraficPort[2];idxMin=2;
                                  }
                                             suffix=idxMin;

                                         if(flowTable.find(make_pair(source,destination))==flowTable.end())
                                                        flowTables[current][make_pair(source,destination)]=suffixTable[suffix];
               }
           }
}
