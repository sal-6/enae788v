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
    
    srand(time(NULL)); // seed for random number generator
       
    Node start = Node(0, 0);
    Collidable goal_region = Collidable(-38, 20, 10);
    float epsilon = 10;
    
    Obstacles O = Obstacles();
    O.parse_from_obstacle_file("./data/hw2/obstacles.txt");
    //O.log_info();
    
    Tree T = Tree();
    T.add_node(&start);
    
    int num_iterations = 0;
    while (num_iterations < 1000) {
        std::cout << "iteration: " << num_iterations << std::endl;
        
        Node rand_node = random_node();
        Node* closest_node = T.closest_node(&rand_node);
        
        float distance = distance_between_nodes(&rand_node, closest_node);
        
        if (distance < epsilon) {
            if (!O.is_segment_in_collision(closest_node, &rand_node)) {
                rand_node.parent = closest_node;
                T.add_node(&rand_node);
            }
        }
        else {
            Node new_node = get_node_in_direction(closest_node, &rand_node, epsilon);
            if (!O.is_segment_in_collision(closest_node, &new_node)) {
                new_node.parent = closest_node;
                T.add_node(&new_node);
            }
        }
        
        if (goal_region.is_point_in_collision(&rand_node)) {
            std::cout << "found goal!" << std::endl;
            break;
        }
        
        num_iterations++;
    }
    
    T.log_info();
    
    
    return 0;
}


