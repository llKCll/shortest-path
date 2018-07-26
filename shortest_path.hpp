//
//  shortest_path.hpp
//  shortest path
//
//  Created by Kevin Conner on 7/24/18.
//  Copyright © 2018 Kevin Conner. All rights reserved.
//

#ifndef shortest_path_h
#define shortest_path_h

using namespace std;
#include <iostream>
#include <string>
#include <vector>
#include <cmath>

struct Node
{
    Node(double, vector<double>, vector<string>, string);
    double cost;
    vector<double> connected_edges;
    vector<string> connected_vertices;
    string id;
};

class Graph
{
    private:
        vector<Node> nodesInGraph;
        vector<string> burned_nodes;
    public:
        bool isEqual(double, double);
        bool search(vector<string>, string);
        double shortestPath(string, string);
        int aVectIndex(string);
        int findMinIndex();
        string getRoute(string, string);
        void addNode(Node);
        void display(string, string);
        void loadG1();
    
};

#endif /* shortest_path_h */
