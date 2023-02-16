/*
Copyright 2018, Michael Otte

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/


#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <sys/types.h>
#include <time.h>
#include <sys/time.h>
#include <math.h>
#include <string>
#include <queue>
#include <iostream>


//#include "heap.h"
//#include "heap.cpp"

#include "graph.h"

using namespace std;

// returns random number between 0 and 1
float rand_f()
{
  return (float)rand() / (float)RAND_MAX;
}

struct node_compare
{
    bool operator()(const Node* lhs, const Node* rhs) const
    {
        return lhs->priority > rhs->priority;
    }
};


double heuristic(Node* start, Node* goal)
{
  double x = start->x - goal->x;
  double y = start->y - goal->y;
  return sqrt(x*x + y*y);
}

void A_star(priority_queue<Node*, vector<Node*>, node_compare> &Q, Node* goalNode) {
  while (!Q.empty()) {
    Node* thisNode = Q.top();
    Q.pop();
    
    for(int n = 0; n < thisNode->numOutgoingEdges; n++)
    {
      Edge* thisEdge = thisNode->outgoingEdges[n];  // pointer to this edge
      Node* neighborNode = thisEdge->endNode;  // pointer to the node on the 
                                              // other end of this edge
      
      if (neighborNode->status == 0) {
        neighborNode->parentNode = thisNode;
        neighborNode->cost_from_start = thisNode->cost_from_start + thisEdge->edgeCost;
        neighborNode->priority = neighborNode->cost_from_start + heuristic(neighborNode, goalNode);
        Q.push(neighborNode);
        neighborNode->status = 1;
      }
      
      else if (neighborNode->status == 1) {
        double new_cost = thisNode->cost_from_start + thisEdge->edgeCost;
        if (new_cost < neighborNode->cost_from_start) {
          neighborNode->parentNode = thisNode;
          neighborNode->cost_from_start = new_cost;
          neighborNode->priority = neighborNode->cost_from_start + heuristic(neighborNode, goalNode);
        }
      }
      
      else if (neighborNode->status == 2) {
        double new_cost = thisNode->cost_from_start + thisEdge->edgeCost;
        if (new_cost < neighborNode->cost_from_start) {
          neighborNode->parentNode = thisNode;
          neighborNode->cost_from_start = new_cost;
          neighborNode->priority = neighborNode->cost_from_start + heuristic(neighborNode, goalNode);
          Q.push(neighborNode);
        }
      }
    }
    
    if (thisNode == goalNode) {
      return;
    }
    
    thisNode->status = 2;    // now this node is in the closed list
  }
}

// main function
// call with three args: ./app.exe <file_id> <start_node_id> <goal_node_id>
int main(int argc, char** argv)
{
  srand(time(NULL)); // seed random number generator
  
  std::string node_file = "./data/hw1/nodes_" + std::string(argv[1]) + ".txt";
  std::string edge_file = "./data/hw1/edges_with_costs_" + std::string(argv[1]) + ".txt";
  
  Graph G;
  G.readGraphFromFiles(node_file.c_str(), edge_file.c_str());
  //G.printGraph();

  // we want to find a path that goes from here to here
  int startNodeIndex = atoi(argv[2]) - 1;
  int goalNodeIndex = atoi(argv[3]) - 1;
  
  // A* search ------------------------------------------------------------
  
  Node* startNode = &G.nodes[startNodeIndex];
  Node* goalNode = &G.nodes[goalNodeIndex];
  
  priority_queue<Node*, vector<Node*>, node_compare> Q;
  startNode->cost_from_start = 0;
  startNode->priority = 0;

  Q.push(startNode);
  
  A_star(Q, goalNode);  
  
  std::string path_output_path = "./output/hw1/output_path_" + std::string(argv[1]) + "_test2.txt";
  std::string search_tree_path = "./output/hw1/search_tree_" + std::string(argv[1]) + "_test2.txt";

  G.savePathToFile(path_output_path.c_str(), goalNode);
  G.saveSearchTreeToFile(search_tree_path.c_str());
  

  return 0;
}
