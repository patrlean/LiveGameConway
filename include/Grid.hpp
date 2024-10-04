/*
Author: Tianyou Zhao 
Class: ECE6122 
Last Date Modified: 03-10-2024
Description:
This is the header file of Grid class, which is used to store the status of the grid and some functions to generate the living status of the blocks.
Some header files are included.
*/
#ifndef GRID_HPP
#define GRID_HPP

#include <vector>
#include <thread>
#include <random>   

#include "Block.hpp"
#include "Globals.hpp"
#include <omp.h>
#include <iostream>

class Grid {
public:
    std::vector<std::vector<Block>> blocks;

    Grid(int rows, int cols);
    
    void generateLivingStatusSequentially();
    void generateLivingStatusForRange(int startRow, int endRow);
    void generateLivingStatusParallel();
    void generateLivingStatusOMP();

    bool updateByRule(int aliveNeighbors, bool isAlive);
    void updateLivingStatus();
    
private:
    
    int rows;
    int cols;
    
    int countAliveNeighbors(int x, int y);
};

#endif // GRID_HPP
