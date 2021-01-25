/*
 * FatTreeRoutingAlgorithm.h
 *
 *  Created on: Apr 25, 2020
 *      Author: hongt
 */

#ifndef FATTREEROUTINGALGORITHM_H_
#define FATTREEROUTINGALGORITHM_H_

#include <map>
#include <utility>
#include <list>
#include <algorithm>
#include <iostream>
#include <utility>
#include <random>
#include <chrono>
#include <string>

#include "RoutingPath.h"
#include "FatTreeGraph.h"

#include "RoutingPath.h"

using namespace std;

class FatTreeRoutingAlgorithm
{
private:
    FatTreeGraph G;
    map<pair<int, int>, RoutingPath> precomputedPaths;
    map<int, map<int,int>> suffixTables;
    map<int, map<int,int>> prefixTables;
    map<int, map<int,int>> corePrefixTables;
    map<int, map< pair<int,int>,int>> flowTables; //map từ 1 cặp nguồn đích đến ip đầu ra
    map<int, map<int,int>> flowTraficPorts;// map số lần có gói tin đi ra của 1 cổng
    map<int, map< pair<int,int>,int>> flowTraficTables;// map giá trị flow của 1 cặp nguồn đích
public:
    FatTreeRoutingAlgorithm(FatTreeGraph G, bool precomputed);
    void buildTables();
    void PrintTables();
    int next(int source, int current, int destination);
    void increaseTraficPorts(int port,int current);
    void increaseTraficTables(int source,int des,int current);
    void shufflerPort(int curent);
    void printPath(int current);
    void makePath(int source,int des,int current);
    RoutingPath path(int source, int destination);
};

#endif /* FATTREEROUTINGALGORITHM_H_ */
