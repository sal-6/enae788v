#include "hw2/utils.h"
#include <iostream>

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
