#include "hw2/utils.h"
#include <iostream>
#include <math.h>


// implement Node **************************************************
Node::Node(float x, float y) {
    this->x = x;
    this->y = y;
    this->parent = NULL;
}

// implement Tree **************************************************
Tree::Tree() {
    this->nodes = std::list<Node*>();
}

void Tree::log_info() {
    std::cout << "nodes: " << std::endl;
    for (std::list<Node*>::iterator it = this->nodes.begin(); it != this->nodes.end(); ++it) {
        std::cout << "x: " << (*it)->x << std::endl;
        std::cout << "y: " << (*it)->y << std::endl;
    }
}

Node* Tree::closest_node(Node* node) {
    float min_dist = 1000000;
    Node* closest_node = NULL;
    for (std::list<Node*>::iterator it = this->nodes.begin(); it != this->nodes.end(); ++it) {
        float dist = sqrt(pow((*it)->x - node->x, 2) + pow((*it)->y - node->y, 2));
        if (dist < min_dist) {
            min_dist = dist;
            closest_node = *it;
        }
    }
    return closest_node;
}

void Tree::add_node(Node* node) {
    this->nodes.push_back(node);
    std::cout << "added node: " << node->x << ", " << node->y << std::endl;
}

bool Tree::export_tree(std::string filename) {
    FILE* pFile = fopen(filename.c_str(), "w");
    if (pFile == NULL) {
        return false;
    }
    
    for (std::list<Node*>::iterator it = this->nodes.begin(); it != this->nodes.end(); ++it) {
        if ((*it)->parent != NULL) {
            fprintf(pFile, "%f, %f, %f, %f\n",
            (*it)->x, (*it)->y,
            (*it)->parent->x, (*it)->parent->y);
        }
    }
    
    fclose(pFile);
    return true;
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