#include "hw2/utils.h"
#include <iostream>


int main(int argc, char** argv) {
    
    Obstacles O = Obstacles();
    O.parse_from_obstacle_file("./data/hw2/obstacles.txt");
    O.log_info();
    
    return 0;
}