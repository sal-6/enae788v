#include "hw2/utils.h"
#include <iostream>
#include <fstream>
#include "hw2/json.hpp"


using json = nlohmann::json;

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

struct ProblemData {
    float start_x;
    float start_y;
    float goal_x;
    float goal_y;
    float goal_radius;
    float epsilon;
};

int main(int argc, char** argv) {
    
    
    std::ifstream inp("./data/hw2/problems.json");
    json problem_data;
    inp >> problem_data;

    ProblemData P = ProblemData();
    P.start_x = problem_data[std::string(argv[1])]["start_x"];
    P.start_y = problem_data[std::string(argv[1])]["start_y"];
    P.goal_x = problem_data[std::string(argv[1])]["goal_x"];
    P.goal_y = problem_data[std::string(argv[1])]["goal_y"];
    P.goal_radius = problem_data[std::string(argv[1])]["goal_radius"];
    P.epsilon = problem_data[std::string(argv[1])]["epsilon"];
    
    srand(time(NULL)); // seed for random number generator
       
    Node start = Node(P.start_x, P.start_y);
    Collidable goal_region = Collidable(P.goal_x, P.goal_y, P.goal_radius);
    float epsilon = P.epsilon;
    
    Obstacles O = Obstacles();
    O.parse_from_obstacle_file("./data/hw2/obstacles.txt");
    //O.log_info();
    
    Tree T = Tree();
    T.add_node(&start);
    Node* goal_node = NULL;
    
    int num_iterations = 0;
    while (num_iterations < 10000) {
        std::cout << "iteration: " << num_iterations << std::endl;
        
        Node rand_node = random_node();
        Node* closest_node = T.closest_node(&rand_node);
        
        float distance = distance_between_nodes(&rand_node, closest_node);
        
        if (distance < epsilon) {
            if (!O.is_segment_in_collision(closest_node, &rand_node, 100)) {
                Node* new_node = new Node(rand_node.x, rand_node.y);
                new_node->parent = closest_node;
                T.add_node(new_node);
                
                if (goal_region.is_point_in_collision(new_node)) {
                    std::cout << "found goal!" << std::endl;
                    goal_node = new_node;
                    break;
                }
            }
        }
        else {
            Node new_node = get_node_in_direction(closest_node, &rand_node, epsilon);
            if (!O.is_segment_in_collision(closest_node, &new_node, 100)) {
                Node* new_ptr = new Node(new_node.x, new_node.y);
                new_ptr->parent = closest_node;
                T.add_node(new_ptr);
                
                if (goal_region.is_point_in_collision(new_ptr)) {
                    std::cout << "found goal!" << std::endl;
                    goal_node = new_ptr;
                    break;
                }
            }
        }
        
        num_iterations++;
    }
    
    
    //T.log_info();
    T.export_tree("./output/hw3/tree_" + std::string(argv[1]) + ".txt");
    T.export_path("./output/hw3/path_" + std::string(argv[1]) + ".txt", goal_node);
    
    return 0;
}


