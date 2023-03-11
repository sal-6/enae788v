#include "hw3/utils.h"
#include <iostream>
#include <math.h>


// implement Node **************************************************
Node::Node(float x, float y) {
    this->x = x;
    this->y = y;
    this->parent = NULL;
}


// implement Collidable **************************************************
Collidable::Collidable(float x, float y, float radius) {
    this->x = x;
    this->y = y;
    this->radius = radius;
}


void Collidable::log_info() {
    std::cout << "x: " << this->x << std::endl;
    std::cout << "y: " << this->y << std::endl;
    std::cout << "radius: " << this->radius << std::endl;
}


// edges are considered collisions if they intersect the circle
bool Collidable::is_point_in_collision(Node* n) {
    float dist = sqrt(pow(this->x - n->x, 2) + pow(this->y - n->y, 2));
    return dist <= this->radius;
}

bool Collidable::is_point_in_collision(float x, float y) {
    float dist = sqrt(pow(this->x - x, 2) + pow(this->y - y, 2));
    return dist <= this->radius;
}


bool Collidable::is_segment_in_collision(Node* n1, Node* n2, int divisions) {
    float dist = sqrt(pow(this->x - n1->x, 2) + pow(this->y - n1->y, 2));
    if (dist <= this->radius) {
        return true;
    }
    dist = sqrt(pow(this->x - n2->x, 2) + pow(this->y - n2->y, 2));
    if (dist <= this->radius) {
        return true;
    }
    
    // divide segment into divisions + 1 points and check if any of them are in collision
    float x_step = (n2->x - n1->x) / divisions;
    float y_step = (n2->y - n1->y) / divisions;
    for (int i = 1; i < divisions; i++) {
        float x = n1->x + i * x_step;
        float y = n1->y + i * y_step;
        dist = sqrt(pow(this->x - x, 2) + pow(this->y - y, 2));
        if (dist <= this->radius) {
            return true;
        }
    }
    return false;
}


bool Collidable::is_trajectory_in_collision(RobotTrajectory* trajectory, float distance_step) {
    
    // check the first point
    if (this->is_point_in_collision(new Node(trajectory->states.front().x, trajectory->states.front().y))) {
        return true;
    }
    
    
    float curr_delta = 0;
    // check the middle points at every point distance_step apart starting from the second point    
    for (auto it = trajectory->states.begin(); it != std::prev(trajectory->states.end()); ++it) {
        // increment curr_delta by the distance between the current point and the next point
        
        curr_delta += sqrt(pow(it->x - std::next(it)->x, 2) + pow(it->y - std::next(it)->y, 2));
        if (curr_delta >= distance_step) {
            curr_delta = 0;

            if (this->is_point_in_collision(new Node(it->x, it->y))) {
                return true;
            }
        }
    }
    
    
    // check the last point
    if (this->is_point_in_collision(new Node(trajectory->states.back().x, trajectory->states.back().y))) {
        return true;
    }
    
    return false;
}

bool Collidable::is_robot_in_collision(Robot* robot, float x, float y, float theta) {
    
    // transform the robot's points to the new location
    std::list<Node*> transformed_points = std::list<Node*>();
    for (std::list<Node*>::iterator it = robot->points.begin(); it != robot->points.end(); ++it) {
        float x = (*it)->x * cos(theta) - (*it)->y * sin(theta) + x;
        float y = (*it)->x * sin(theta) + (*it)->y * cos(theta) + y;
        transformed_points.push_back(new Node(x, y));
    }
    
    // check if any of the robot's points are in collision
    for (std::list<Node*>::iterator it = transformed_points.begin(); it != transformed_points.end(); ++it) {
        if (this->is_point_in_collision(*it)) {
            return true;
        }
    }
    
    return false;
}


// implement Obstacles **************************************************
Obstacles::Obstacles() {
    this->obstacles = std::list<Collidable*>();
}


void Obstacles::log_info() {
    std::cout << "obstacles: " << std::endl;
    for (std::list<Collidable*>::iterator it = this->obstacles.begin(); it != this->obstacles.end(); ++it) {
        (*it)->log_info();
    }
}


// parse the obstacle file where each line is a circle with x, y, and radius. ignore the first line
void Obstacles::parse_from_obstacle_file(std::string filename) {
    FILE* fp = fopen(filename.c_str(), "r");
    if (fp == NULL) {
        std::cout << "Error opening file" << std::endl;
        return;
    }
    
    char line[256];
    fgets(line, sizeof(line), fp); // ignore the first line
    while (fgets(line, sizeof(line), fp)) {
        float x, y, radius;
        sscanf(line, "%f, %f, %f\n", &x, &y, &radius);
        this->obstacles.push_back(new Collidable(x, y, radius));
    }
    
    fclose(fp);   
}


bool Obstacles::is_point_in_collision(Node* n) {
    for (std::list<Collidable*>::iterator it = this->obstacles.begin(); it != this->obstacles.end(); ++it) {
        if ((*it)->is_point_in_collision(n)) {
            return true;
        }
    }
    return false;
}


bool Obstacles::is_segment_in_collision(Node* n1, Node* n2, int divisions) {
    for (std::list<Collidable*>::iterator it = this->obstacles.begin(); it != this->obstacles.end(); ++it) {
        if ((*it)->is_segment_in_collision(n1, n2, divisions)) {
            return true;
        }
    }
    return false;
}


bool Obstacles::is_trajectory_in_collision(RobotTrajectory* trajectory, float distance_step) {
    
    // check the first point
    if (this->is_point_in_collision(new Node(trajectory->states.front().x, trajectory->states.front().y))) {
        return true;
    }
    
    
    float curr_delta = 0;
    // check the middle points at every point distance_step apart starting from the second point    
    for (auto it = trajectory->states.begin(); it != std::prev(trajectory->states.end()); ++it) {
        // increment curr_delta by the distance between the current point and the next point
        //std::cout << "curr_delta: " << curr_delta << std::endl;
        curr_delta += sqrt(pow(it->x - std::next(it)->x, 2) + pow(it->y - std::next(it)->y, 2));
        if (curr_delta >= distance_step) {
            curr_delta = 0;
            //std::cout << "checking point: " << it->x << ", " << it->y << std::endl;
            if (this->is_point_in_collision(new Node(it->x, it->y))) {
                return true;
            }
        }
    }
    
    
    // check the last point
    if (this->is_point_in_collision(new Node(trajectory->states.back().x, trajectory->states.back().y))) {
        return true;
    }
    
    return false;
}


bool Obstacles::is_trajectory_in_collision(RobotTrajectory* trajectory, Robot* robot, float distance_step) {
    
    // check the first point
    if (this->is_robot_in_collision(robot, trajectory->states.front().x, trajectory->states.front().y, trajectory->states.front().theta)) {
        return true;
    }
    
    
    float curr_delta = 0;
    // check the middle points at every point distance_step apart starting from the second point    
    for (auto it = trajectory->states.begin(); it != std::prev(trajectory->states.end()); ++it) {
        // increment curr_delta by the distance between the current point and the next point
        //std::cout << "curr_delta: " << curr_delta << std::endl;
        curr_delta += sqrt(pow(it->x - std::next(it)->x, 2) + pow(it->y - std::next(it)->y, 2));
        if (curr_delta >= distance_step) {
            curr_delta = 0;
            //std::cout << "checking point: " << it->x << ", " << it->y << std::endl;
            if (this->is_robot_in_collision(robot, it->x, it->y, it->theta)) {
                return true;
            }
        }
    }
    
    
    // check the last point
    if (this->is_robot_in_collision(robot, trajectory->states.back().x, trajectory->states.back().y, trajectory->states.back().theta)) {
        return true;
    }
    
    return false;
}



bool Obstacles::is_robot_in_collision(Robot* robot, float x, float y, float theta) {
    
    // transform the robot's points to the new location
    std::list<Node*> transformed_points = std::list<Node*>();
    for (std::list<Node*>::iterator it = robot->points.begin(); it != robot->points.end(); ++it) {
        float x_new = x + (*it)->x * cos(theta) - (*it)->y * sin(theta);
        float y_new = y + (*it)->x * sin(theta) + (*it)->y * cos(theta);
        transformed_points.push_back(new Node(x_new, y_new));
    }
    
    // check if any of the robot's points are in collision
    for (std::list<Node*>::iterator it = transformed_points.begin(); it != transformed_points.end(); ++it) {
        if (this->is_point_in_collision(*it)) {
            return true;
        }
    }
    
    return false;
}

// implement Robot **************************************************
Robot::Robot() {
    this->points = std::list<Node*>(); // empty list
}

void Robot::parse_from_robot_file(std::string filename) {
    FILE* fp = fopen(filename.c_str(), "r");
    if (fp == NULL) {
        std::cout << "Error opening file" << std::endl;
        return;
    }
    
    char line[256];
    while (fgets(line, sizeof(line), fp)) {
        float x, y;
        sscanf(line, "%f, %f\n", &x, &y);
        this->points.push_back(new Node(x, y));
    }
    
    fclose(fp);
    
}


// implemenmt RobotTrajectory **************************************************

RobotTrajectory::RobotTrajectory(RobotState start_state) {
    this->initial_state = start_state;
    this->states = std::list<RobotState>();
    this->states.push_back(start_state);
    this->is_valid = true;
    this->parent = NULL;
}


void RobotTrajectory::log_info() {
    std::cout << "states: " << std::endl;
    for (std::list<RobotState>::iterator it = this->states.begin(); it != this->states.end(); ++it) {
        std::cout << "t: " << it->t << ", x: " << it->x << ", y: " << it->y << std::endl;
    }
}


bool RobotTrajectory::propogate_until_distance(float distance, float time_step) {
    
    float distance_traveled = 0;
    int num_steps = 0;
    
    while (distance_traveled < distance && num_steps < MAX_EULER_ITERATIONS) {
        RobotState current_state = this->states.back();
        
        // calculate rates of change
        float x_dot = current_state.v * cos(current_state.theta);
        float y_dot = current_state.v * sin(current_state.theta);
        float theta_dot = current_state.w;
        float v_dot = current_state.a;
        float w_dot = current_state.gamma;
        
        // calculate new state
        float t = current_state.t + time_step;
        float x = current_state.x + x_dot * time_step;
        float y = current_state.y + y_dot * time_step;
        float theta = current_state.theta + theta_dot * time_step;
        float v = current_state.v + v_dot * time_step;
        float w = current_state.w + w_dot * time_step;
        
        // add new state to trajectory
        this->states.push_back(RobotState {
            t, 
            x, 
            y, 
            theta, 
            v, 
            w
        } );
        
        if (v > MAX_VELOCITY || v < -MAX_VELOCITY) {
            this->is_valid = false;
            return false;
        }
        
        if (w > MAX_STEEERING_VELOCITY || w < -MAX_STEEERING_VELOCITY) {
            this->is_valid = false;
            return false;
        }
        
        // update distance traveled
        distance_traveled += sqrt(pow(x_dot * time_step, 2) + pow(y_dot * time_step, 2));
        num_steps++;
    }
    
    
    return true;
}


void RobotTrajectory::export_trajectory(std::string filename) {
    FILE* fp = fopen(filename.c_str(), "w");
    if (fp == NULL) {
        std::cout << "Error opening file" << std::endl;
        return;
    }
    
    for (std::list<RobotState>::iterator it = this->states.begin(); it != this->states.end(); ++it) {
        fprintf(fp, "%f, %f, %f, %f, %f, %f, %f, %f\n", it->t, it->x, it->y, it->theta, it->v, it->w, it->a, it->gamma);
    }
    
    fclose(fp);
}
    

// implement SearchTree ********************************************************

SearchTree::SearchTree() {
    this->trajectories = std::list<RobotTrajectory*>();
}


void SearchTree::add_trajectory(RobotTrajectory* trajectory) {
    this->trajectories.push_back(trajectory);
}


RobotTrajectory* SearchTree::get_closest_trajectory_end(Node node) {
    
    // iterate throgugh all trajectories and find the one with the closed end point
    RobotTrajectory* closest_trajectory = NULL;
    float closest_distance = 1000000;
    
    for (std::list<RobotTrajectory*>::iterator it = this->trajectories.begin(); it != this->trajectories.end(); ++it) {
        float distance = sqrt(pow(node.x - (*it)->states.back().x, 2) + pow(node.y - (*it)->states.back().y, 2));
        if (distance < closest_distance) {
            //std::cout << "new closest" << std::endl;
            closest_trajectory = *it;
            closest_distance = distance;
        }
    }
    
    //std::cout << std::endl;
    
    return closest_trajectory;
}


bool SearchTree::export_tree(std::string filename) {
    FILE* fp = fopen(filename.c_str(), "w");
    if (fp == NULL) {
        return false;
    }

    // log each trajectory
    for (std::list<RobotTrajectory*>::iterator it = this->trajectories.begin(); it != this->trajectories.end(); ++it) {

        for (std::list<RobotState>::iterator it2 = (*it)->states.begin(); it2 != (*it)->states.end(); ++it2) {
            fprintf(fp, "%f, %f, %f, %f, %f, %f, %f, %f\n", it2->t, it2->x, it2->y, it2->theta, it2->v, it2->w, it2->a, it2->gamma);
        }
        
    }
    
    
    fclose(fp);
    return true;
}


// function implementations **************************************************
Node random_node() {
    float rand_x = static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / 100.0)) - 50;
    float rand_y = static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / 100.0)) - 50;
    return Node(rand_x, rand_y);
}


float distance_between_nodes(Node* n1, Node* n2) {
    return sqrt(pow(n1->x - n2->x, 2) + pow(n1->y - n2->y, 2));
}


Node get_node_in_direction(Node* n1, Node* n2, float distance) {
    // get x and y components of the vector between n1 and n2
    float x_component = n2->x - n1->x;
    float y_component = n2->y - n1->y;
    
    // normalize the vector
    float magnitude = sqrt(pow(x_component, 2) + pow(y_component, 2));
    x_component /= magnitude;
    y_component /= magnitude;
    
    // multiply the vector by the distance
    x_component *= distance;
    y_component *= distance;
    
    // add the vector to n1
    float x = n1->x + x_component;
    float y = n1->y + y_component;
    
    return Node(x, y);
}