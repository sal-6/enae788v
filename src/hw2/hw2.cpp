#include "hw2/utils.h"
#include <iostream>

void test_obstacles() {
    Obstacles O = Obstacles();
    O.parse_from_obstacle_file("./data/hw2/obstacles.txt");
    O.log_info();
}


void test_closest_node() {
    Tree T = Tree();
    Node* n1 = new Node(0, 0);
    Node* n2 = new Node(1, 1);
    Node* n3 = new Node(2, 2);
    T.add_node(n1);
    T.add_node(n2);
    T.add_node(n3);
    Node* n4 = new Node(.5, 1);
    Node* closest_node = T.closest_node(n4);
    std::cout << "closest node: " << closest_node->x << ", " << closest_node->y << std::endl;
}

int main(int argc, char** argv) {
    
    
}


