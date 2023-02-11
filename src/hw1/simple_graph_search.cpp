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


#include "heap.h"
//#include "heap.cpp"

#include "graph.h"

// returns random number between 0 and 1
float rand_f()
{
  return (float)rand() / (float)RAND_MAX;
}


// does the node expand opperation (note: we are
// using a very dumb random walk)
void expand(Heap<Node> &H, Node* thisNode)
{
  for(int n = 0; n < thisNode->numOutgoingEdges; n++)
  {
    Edge* thisEdge = thisNode->outgoingEdges[n];  // pointer to this edge
    Node* neighborNode = thisEdge->endNode;  // pointer to the node on the 
                                             // other end of this edge

    if(neighborNode->status == 0)  // neighbor has not yet been visited 
    {
      // add the neighbor to the heap (with a random key)
      double neighborKey = rand_f();
      H.addToHeap(neighborNode, neighborKey);
      
      // remeber this node as its parent
      neighborNode->parentNode = thisNode;

      // make sure it is in the open list
      neighborNode->status = 1;
    }
    else if(neighborNode->status == 1)  // neighbor is in the open list
    {                                   // (but not the closed list)

      // we'll flip a coin, and half the time we'll reset the node
      // to use this node as its parent with a new key
      if(rand_f() < 0.5)
      {

        double newNeighborKey = rand_f();
        H.updateNodeInHeap(neighborNode, newNeighborKey);

        // remeber this node as its parent
        neighborNode->parentNode = thisNode;
      }
    }
  }

  thisNode->status = 2;    // now this node is in the closed list
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
  // these are indicies and NOT the actual ids
  int startNodeIndex = atoi(argv[2]) - 1;
  int goalNodeIndex = atoi(argv[3]) - 1;


  Heap<Node> H(100); // this is the heap (start's with space for 100 items
                     // but will grow automatically as needed).


  // these are pointers to the start and end nodes
  Node* startNode = &G.nodes[startNodeIndex];
  Node* goalNode = &G.nodes[goalNodeIndex];


  // we'll do a random walk search, but we'll do it
  // by assigning nodes a random key in the heap
  // this way it can be showen how the heap works

  double key = rand_f();
  H.addToHeap(startNode, key);
  startNode->status = 1;    // now the start node is in the open list

  // while there are nodes left in the heap
  // (note that a better stopping criteria should be used in
  // most "real" algorithms, but this will cause us to generate
  // a random spanning tree over the graph, which is kind of cool)
  while(H.topHeap() != NULL)
  {
    Node* thisNode = H.popHeap();
    expand(H, thisNode);

    // H.printHeap();
  }
  
  // now we want to save files that contain the search tree that we build
  // and also the best path that we found (NOTE, these add 1 to indicies
  // to make them compativle with the graph file that was used as input for
  // the search)
  
  string path_output_path = "./output/hw1/output_path_" + std::string(argv[1]) + ".txt";
  string search_tree_path = "./output/hw1/search_tree_" + std::string(argv[1]) + ".txt";

  G.savePathToFile(path_output_path.c_str(), goalNode);
  G.saveSearchTreeToFile(search_tree_path.c_str());


  return 0;
}
