//
//  shortest_path.cpp
//  shortest path
//
//  Created by Kevin Conner on 7/24/18.
//  Copyright © 2018 Kevin Conner. All rights reserved.
//

#include "shortest_path.hpp"

/******************************************************************************
 *                                Node                                        *
 * Node constructor.                                                          *
 ******************************************************************************/

Node::Node(double c, vector<double> ce, vector<string> cv, string name) {
    this->cost = c;
    this->connected_edges = ce;
    this->connected_vertices = cv;
    this->id = name;
}

/******************************************************************************
 *                                addNode                                     *
 * Push back a node in nodesInGraph                                           *
 ******************************************************************************/

void Graph::addNode(Node aNode) {
    nodesInGraph.push_back(aNode);
}

/******************************************************************************
 *                                aVectIndex                                  *
 * Finds the index of an individual node object used in the graph.            *
 * Args - An identfier of the node.                                           *
 * Returns - An integer of the index in aVect.                                *
 ******************************************************************************/

int Graph::aVectIndex(string str) {
    for (int i = 0; i < nodesInGraph.size(); i++) {
        if (nodesInGraph[i].id == str) {
            return i;
        }
    }
    return -1;
}

/******************************************************************************
 *                                Display                                     *
 * Displays the shortest path and route taken.                                *
 * Args - Start and ending vertices.                                          *
 ******************************************************************************/

void Graph::display(string start, string end) {
    
    double path = shortestPath(start, end);
    string route = getRoute(start, end);
    
    cout << "The shortest path taken has a distance of " << path << ".\n"
    << "The following route was used, " << route << "." << endl;
}

/******************************************************************************
 *                                findMinIndex                                *
 * Finds the index of an individual node object with the lowest node cost     *
 * excluding the nodes that have been added to the burned_nodes.              *
 * Returns - The index of the node with the lowest value excluding the nodes  *
 * that are no longer being considered because their lowest value has already *
 * been calculated.                                                           *
 ******************************************************************************/

int Graph::findMinIndex() {
    double curr_cost = 0;
    // Get a node to compare that has a cost greater than 0 and not already in burned_nodes.
    for (int i = 0; i < nodesInGraph.size(); i++) {
        if (!search(burned_nodes, nodesInGraph[i].id) && nodesInGraph[i].cost > 0) {
            curr_cost = nodesInGraph[i].cost;
            break;
        }
    }
    // Compare this number to rest to get minimal value and must greater than 0.
    for (int i = 0; i < nodesInGraph.size(); i++) {
        if ( nodesInGraph[i].cost < curr_cost && (!search(burned_nodes, nodesInGraph[i].id)) && nodesInGraph[i].cost > 0)  {
            curr_cost = nodesInGraph[i].cost;
        }
    }
    // Get the index where the value is at.
    for (int i = 0; i < nodesInGraph.size(); i++) {
        if (curr_cost == nodesInGraph[i].cost && !search(burned_nodes, nodesInGraph[i].id)) {
            return i;
        }
    }
    return -1;
}

/******************************************************************************
 *                                search                                      *
 * Searches a vector for a string.                                            *
 * Args - A vector to be searched and the string to be searched for.          *
 * Returns - True if the string is found, otherwise false.                    *
 ******************************************************************************/

bool Graph::search(vector<string> vec, string str) {
    for (int i = 0; i < vec.size(); i++) {
        if (str == vec[i]) {
            return true;
        }
    }
    return false;
}

/******************************************************************************
 *                                setRoute                                    *
 * Get method to recursively get the route taken from one node to another.    *
 * shortestPath must be used first.                                           *
 * Args - Starting and ending node id.                                        *
 ******************************************************************************/

string Graph::getRoute(string start, string end) {
    
    if (start == end) {
        return start;
    }
    
    // !! make these a pointer later
    int eIndex = aVectIndex(end);
    vector<string> eVertices = nodesInGraph[eIndex].connected_vertices;
    vector<double> eEdges = nodesInGraph[eIndex].connected_edges;
    
    for (int i = 0; i < eVertices.size(); i++) {
        if (isEqual(nodesInGraph[eIndex].cost - eEdges[i], nodesInGraph[aVectIndex(eVertices[i])].cost)) {
            return getRoute(start, eVertices[i]) + " -> " + end;
        }
    }
    
    return "Unable to find route..";
}

/******************************************************************************
 *                                shortestPath                                *
 * Finds the shortest distance from one vertex to another.                    *
 * Args - A starting node id and an ending node id.                           *
 * Returns - A number with the shortest distance between the two chosen nodes.*
 ******************************************************************************/

double Graph::shortestPath(string start, string end) {
    
    // Push start node in burned_nodes
    Node beg = nodesInGraph[aVectIndex(start)];
    burned_nodes.push_back(start);
    
    // Calculate the cost of the nodes connected to the start node.
    for (int i = 0; i < beg.connected_edges.size(); i++) {
        int index = aVectIndex(beg.connected_vertices[i]);
        nodesInGraph[index].cost = beg.connected_edges[i];
    }
    
    // Loop until all vertices are added to burned_nodes.
    while (burned_nodes.size() < nodesInGraph.size()) {
        int minIndex = findMinIndex();
        
        // Calculate value of each node connected to the minimal value node.
        for (int i = 0; i < nodesInGraph[minIndex].connected_vertices.size(); i++) {
            // Get index of each node connected to the minimal node.
            string nodeChar = nodesInGraph[minIndex].connected_vertices[i];
            int vecIndex = aVectIndex(nodeChar);
            
            // Possible sum is the value of the connected edge + the value of the connected node.
            double possSum = nodesInGraph[minIndex].connected_edges[i] + nodesInGraph[minIndex].cost;
            
            // (If the node hasn't been burned and if the sum is less than the current value) or (hasn't been burned and is 0).
            if ((!search(burned_nodes, nodesInGraph[minIndex].connected_vertices[i]) && possSum < nodesInGraph[vecIndex].cost) || (!search(burned_nodes, nodesInGraph[minIndex].connected_vertices[i]) && nodesInGraph[vecIndex].cost == 0))
            {
                nodesInGraph[vecIndex].cost = possSum;
            }
        }
        // Add minimal value to "burn list" soFar so that it won't be considered again.
        burned_nodes.push_back(nodesInGraph[minIndex].id);
    }
    return nodesInGraph[aVectIndex(end)].cost;
}

/******************************************************************************
 *                                isEqual                                     *
 * Compares doubles to check equal decimals that differ due to rounding error.*
 * uses an absolute epsilon for numbers close to zero.                        *
 * Args - Two doubles to compare.                                             *
 * Returns - Returns true if they are nearly or equal, or false if not.       *
 ******************************************************************************/

bool Graph::isEqual(double a, double b) {
    const double EPSILON = 1e-5;
    
    // Check if the numbers are really close -- needed when comparing numbers near zero.
    double diff = fabs(a - b);
    if (diff <= EPSILON) {
        return true;
    }
    return false;
}

/******************************************************************************
 *                                loadG1                                      *
 * Creates Graph1 to be worked with.                                          *
 ******************************************************************************/

void Graph::loadG1() {
    
    /*
     Creates the following graph to be solved. The distances of each edge are to the right of the edge.
     
              c
             /  \
            /3   \2
           /      \
          a        f
         /  \     / \
        /1   \1  /7  \5
       /      \ /     \
      s        d       t
       \      / \      /
        \9   /1  \8   /2
         \  /     \  /
          b         g
           \       /
            \2    /3
             \   /
              \ /
               e
     
     */
    
    // Ensure vectors are empty
    nodesInGraph.empty();
    burned_nodes.empty();
    
    // Make the vertices and edges that are connected to each node.
    vector<string> sv = {"a", "b"};
    vector<double> se = {1, 9};
    
    vector<string> av = {"s", "c", "d"};
    vector<double> ae = {1, 3, 1};
    
    vector<string> bv = {"s", "d", "e"};
    vector<double> be = {9, 1, 2};
    
    vector<string> cv = {"a", "f"};
    vector<double> ce = {3, 2};
    
    vector<string> dv = {"a", "b", "f", "g"};
    vector<double> de = {1, 1, 7, 8};
    
    vector<string> ev = {"b", "g"};
    vector<double> ee = {2, 3};
    
    vector<string> fv = {"c", "d", "t"};
    vector<double> fe = {2, 7, 5};
    
    vector<string> gv = {"d", "e", "t"};
    vector<double> ge = {8, 3, 2};
    
    vector<string> tv = {"f", "g"};
    vector<double> te = {5, 2};
    
    // Create each node. Has a cost to reach the node, connected vectors, connected edges, and id.
    // Actual cost is calculated in shortest_path(). Set to 0 for now.
    Node n1(0, se, sv, "s");
    Node n2(0, ae, av, "a");
    Node n3(0, be, bv, "b");
    Node n4(0, ce, cv, "c");
    Node n5(0, de, dv, "d");
    Node n6(0, ee, ev, "e");
    Node n7(0, fe, fv, "f");
    Node n8(0, ge, gv, "g");
    Node n9(0, te, tv, "t");
    
    // Add the nodes to create a graph.
    addNode(n1);
    addNode(n2);
    addNode(n3);
    addNode(n4);
    addNode(n5);
    addNode(n6);
    addNode(n7);
    addNode(n8);
    addNode(n9);
}


