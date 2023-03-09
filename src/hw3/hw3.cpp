#include "hw3/utils.h"
#include <iostream>
#include <fstream>
#include "hw3/json.hpp"


using json = nlohmann::json;

struct ProblemData {
    float start_x;
    float start_y;
    float start_theta;
    float goal_x;
    float goal_y;
    float goal_radius;
    float epsilon;
};


void rrt_with_dynamics_and_volume(std::string problem) {
    std::ifstream inp("./data/hw3/problems.json");
    json problem_data;
    inp >> problem_data;

    ProblemData P = ProblemData();
    P.start_x = problem_data[std::string(problem)]["start_x"];
    P.start_y = problem_data[std::string(problem)]["start_y"];
    P.start_theta = problem_data[std::string(problem)]["start_theta"];
    P.goal_x = problem_data[std::string(problem)]["goal_x"];
    P.goal_y = problem_data[std::string(problem)]["goal_y"];
    P.goal_radius = problem_data[std::string(problem)]["goal_radius"];
    P.epsilon = problem_data[std::string(problem)]["epsilon"];
    
    srand(time(NULL)); // seed for random number generator
       
    Collidable goal_region = Collidable(P.goal_x, P.goal_y, P.goal_radius);
    float epsilon = P.epsilon;
    
    Obstacles O = Obstacles();
    O.parse_from_obstacle_file("./data/hw3/obstacles.txt");
    
};


void test_dynamics() {
    RobotState start = RobotState();
    start.t = 0;
    start.x = 0;
    start.y = 0;
    start.theta = 0;
    start.v = 0;
    start.w = 0;
    
    RobotTrajectory T = RobotTrajectory(start);
    T.propogate_until_distance(1, 1, 1, 0.1);
    T.export_trajectory("./output/hw3/tests/trajectory.csv", 1, 1);
}

int main(int argc, char** argv) {
    test_dynamics();
    
    return 0;
}


