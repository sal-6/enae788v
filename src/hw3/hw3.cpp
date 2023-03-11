#include "hw3/utils.h"
#include <iostream>
#include <fstream>
#include <time.h>
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


void test_dynamics() {
    // create a list of accelerations and angular velocities to test with a step size of CONTROL_ACCERATION_FIDELITY and CONTROL_STEEERING_FIDELITY
    std::list<float> accelerations;
    std::list<float> angular_velocities;
    for (float a = -MAX_ACCELERATION; a <= MAX_ACCELERATION; a += CONTROL_ACCERATION_FIDELITY) {
        accelerations.push_back(a);
    }
    
    for (float w = -MAX_STEEERING_ACCELERATION; w <= MAX_STEEERING_ACCELERATION; w += CONTROL_STEEERING_FIDELITY) {
        angular_velocities.push_back(w);
    }
    
    // create a list of test robot states with each combination of acceleration and angular velocity
    std::list<RobotState> test_states;
    for (float a : accelerations) {
        for (float w : angular_velocities) {
            RobotState s = RobotState();
            s.t = 0;
            s.x = 0;
            s.y = 0;
            s.theta = 0;
            s.v = 0;
            s.w = 0;
            s.a = a;
            s.gamma = w;
            test_states.push_back(s);
        }
    }
    
    int i = 0;
    // propogate each test state for 1 second and log the results
    for (RobotState s : test_states) {
        RobotTrajectory T = RobotTrajectory(s);
        std::cout << T.propogate_until_distance(1, 0.1) << std::endl;
        T.export_trajectory("./output/hw3/tests/trajectory_" + std::to_string(i) + ".csv");
        i++;
    }
}


void test_collision() {
    RobotState s = RobotState();
    s.t = 0;
    s.x = 0;
    s.y = 0;
    s.theta = 0;
    s.v = 0;
    s.w = 0;
    s.a = 1;
    s.gamma = .1;
    
    RobotTrajectory T = RobotTrajectory(s);
    T.propogate_until_distance(8, 0.1);
    T.export_trajectory("./output/hw3/trajectory.csv");
    
    Obstacles O = Obstacles();
    O.parse_from_obstacle_file("./data/hw3/obstacles.txt");
    
    std::cout << O.is_trajectory_in_collision(&T, 0.5) << std::endl;
    
}

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
    
    //srand(time(NULL)); // seed for random number generator
       
    Collidable goal_region = Collidable(P.goal_x, P.goal_y, P.goal_radius);
    float epsilon = P.epsilon;
    
    Obstacles O = Obstacles();
    O.parse_from_obstacle_file("./data/hw3/obstacles.txt");
    
    SearchTree tree = SearchTree();
    RobotState start = RobotState();
    start.x = P.start_x;
    start.y = P.start_y;
    start.theta = P.start_theta;
    start.v = 0;
    start.w = 0;
    start.a = 0;
    start.gamma = 0;
    
    RobotTrajectory start_trajectory = RobotTrajectory(start);
    //start_trajectory.final_state = start;
    tree.add_trajectory(&start_trajectory);
    
    //start_trajectory.log_info();
    
    std::list<float> accelerations;
    std::list<float> angular_velocities;
    for (float a = -MAX_ACCELERATION; a <= MAX_ACCELERATION; a += CONTROL_ACCERATION_FIDELITY) {
        accelerations.push_back(a);
    }
    
    for (float w = -MAX_STEEERING_ACCELERATION; w <= MAX_STEEERING_ACCELERATION; w += CONTROL_STEEERING_FIDELITY) {
        angular_velocities.push_back(w);
    }
    
    int iters = 0;
    while (iters < 2000) {
        std::cout << "Iteration: " << iters << std::endl;
        Node rand_node = random_node();
        //std::cout << "Random Node: " << rand_node.x << ", " << rand_node.y << std::endl;
        RobotTrajectory* nearest_traj = tree.get_closest_trajectory_end(rand_node);
        
        //std::cout << "Nearest Trajectory: " << nearest_traj->final_state.x << ", " << nearest_traj->final_state.y << std::endl;
        
        std::list<RobotState> test_states;
        for (float a : accelerations) {
            for (float w : angular_velocities) {
                RobotState s = RobotState();
                s.t = nearest_traj->states.back().t;
                s.x = nearest_traj->states.back().x;
                s.y = nearest_traj->states.back().y;
                s.theta = nearest_traj->states.back().theta;
                s.v = nearest_traj->states.back().v;
                s.w = nearest_traj->states.back().w;
                s.a = a;
                s.gamma = w;
                test_states.push_back(s);
            }
        }
        
        std::list<RobotTrajectory*> test_trajectories;
        for (RobotState s : test_states) {
            RobotTrajectory* traj = new RobotTrajectory(s);
            traj->propogate_until_distance(epsilon, 0.1);
            test_trajectories.push_back(traj);
        }
        
        
        //determine the trajectory that ends closest to the random node and is collision free and is valid
        RobotTrajectory* best_trajectory = NULL;
        float best_distance = 1000000;
        
        for (RobotTrajectory* each_t : test_trajectories) {
            
            if (O.is_trajectory_in_collision(each_t, 0.5)) {
                continue;
            }
            
            if (!each_t->is_valid) {
                continue;
            }
            
            float distance = sqrt(pow(each_t->states.back().x - rand_node.x, 2) + pow(each_t->states.back().y - rand_node.y, 2));
            if (distance < best_distance) {
                best_distance = distance;
                best_trajectory = each_t;
            }
        }
        
        if (best_trajectory != NULL) {
            
            best_trajectory->parent = nearest_traj;
            tree.add_trajectory(best_trajectory);
            
            if (goal_region.is_point_in_collision(best_trajectory->states.back().x, best_trajectory->states.back().y)) {
                std::cout << "Goal Reached!" << std::endl;
                break;
            }
            //best_trajectory->log_info();
        }
        
        
        
        iters++;
    }
    
    // compare final states to last entry in trajectory
    /* for (RobotTrajectory* each_t : tree.trajectories) {
        std::cout << each_t->states.back().x << ", " << each_t->states.back().y << std::endl;
        std::cout << each_t->states.back().x << ", " << each_t->states.back().y << std::endl;
        std::cout << "----------------" << std::endl;
    } */
    
    tree.export_tree("./output/hw3/tree_" + problem + ".csv");
    
};


int main(int argc, char** argv) {
    srand (time(NULL));
    rrt_with_dynamics_and_volume(argv[1]);
    
    return 0;
}


