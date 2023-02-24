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


void test_collision() {
    Collidable C = Collidable(2, 2, 1);
    Node* n1 = new Node(0, 0);
    Node* n2 = new Node(1, 4);
    Node* n3 = new Node(3, 3);
    
    std::cout << "n1 to n2 in collision: " << C.is_segment_in_collision(n1, n2) << std::endl;
    std::cout << "n2 to n3 in collision: " << C.is_segment_in_collision(n2, n3) << std::endl;
    std::cout << "n3 to n1 in collision: " << C.is_segment_in_collision(n3, n1) << std::endl;
}

int main(int argc, char** argv) {
    
    test_collision();
    return 0;
}


