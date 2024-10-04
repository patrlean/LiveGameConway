/*
Author: Tianyou Zhao 
Class: ECE6122 
Last Date Modified: 03-10-2024
Description:
This is the main file of Game of Life.
*/

// src/main.cpp
#include <SFML/Graphics.hpp>
#include <iostream>
#include <random>
#include <algorithm>
#include <thread>
#include <chrono>
#include <omp.h>


#include "Globals.hpp"
#include "Block.hpp"
#include "Grid.hpp"

using namespace std;

void parseArgs(int argc, char *argv[]);

int main(int argc, char *argv[])
{

    parseArgs(argc, argv);

    // calculate the number of blocks in the window
    horizontalBlockNumbers = windowWidth / cellSize;
    verticalBlockNumbers = windowHeight / cellSize;

    // generate the grid
    Grid grid(verticalBlockNumbers, horizontalBlockNumbers);
    // Generate main window 
    sf::VideoMode vm(windowWidth, windowHeight);
    sf::RenderWindow window(vm, "Live Game", sf::Style::Default);
    
    // set input indicator
    bool acceptInput = false;

    // get the number of threads
    int maxNumThreads = std::thread::hardware_concurrency();

    numThreads = min(maxNumThreads, numThreads);    
    // set timer
    
    //
    int loopNumber = 0;
    int totalTime = 0;
    int averageTime = 0;

    int maxOMPThread = omp_get_max_threads();

    /*
     * ************************
     * Start the game
     * ************************
     */ 
    while (window.isOpen())
    {
        /* Three parts: processInput, update, render */
        loopNumber++;
        /*
        ************************
         Process Input
        ************************ 
         */ 
        auto event = sf::Event{};
        while( window.pollEvent(event)){
            //close the window
            if (event.type == sf::Event::KeyReleased){
                acceptInput = true;
            }
            if (event.type == sf::Event::Closed){
                window.close();
            }
        }
        
        // close the window
        if (acceptInput){
            // close the window
            if ( sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
            {
                window.close();
            }
        }
        
        /*
        ***************************
        Update the scene
        ***************************
        */ 
       // sequential processing
        if (processingType == "SEQ"){
            auto start = chrono::high_resolution_clock::now();
            
            grid.generateLivingStatusSequentially();

            // grid.updateLivingStatus();
                        
            auto end = chrono::high_resolution_clock::now();

            auto duration = chrono::duration_cast<chrono::microseconds>(end - start).count();

            totalTime += duration;

            if (loopNumber % 100 == 0){
                
                cout << "100 generations took " << totalTime << " microseconds with single thread." << endl;

                totalTime = 0;
            }
        }

        // parallel processing
        else if (processingType == "THRD"){
            auto start = chrono::high_resolution_clock::now();

            // generate the living status of the blocks
            grid.generateLivingStatusParallel();

            // update the living status of the blocks
            // grid.updateLivingStatus();

            auto end = chrono::high_resolution_clock::now();

            auto duration = chrono::duration_cast<chrono::microseconds>(end - start).count();

            totalTime += duration;

            if (loopNumber % 100 == 0){
                
                cout << "100 generations took " << totalTime << " microseconds with "<<numThreads<<" std::threads." << endl;

                totalTime = 0;
            }
        } else if (processingType == "OMP"){
            auto start = chrono::high_resolution_clock::now();

            // generate the living status of the blocks
            grid.generateLivingStatusOMP();

            // update the living status of the blocks
            // grid.updateLivingStatus();

            auto end = chrono::high_resolution_clock::now();

            auto duration = chrono::duration_cast<chrono::microseconds>(end - start).count();

            totalTime += duration;

            if (loopNumber % 100 == 0){
                
                cout << "100 generations took " << totalTime << " microseconds with "<<numThreads<<" OMP threads." << endl;

                totalTime = 0;
            }
        }

        // update the living status of the blocks
        grid.updateLivingStatus();

        /* 
        ***************************
        Render Graphics 
        ***************************
        */ 

        // clear windows
        window.clear();
        
        // show things
        for (auto &row : grid.blocks){
            for (auto &block : row){
                if (block.isAlive){
                    window.draw(block);
                    
                }
            }
        }

        // display
        window.display();
    }
    return 0;
}

void parseArgs(int argc, char *argv[])
{
    for (int i = 1; i < argc; i++) {
        string arg = argv[i];

        if (arg == "-n" && i + 1 < argc) {
            numThreads = atoi(argv[++i]); // no less than 2
            if (numThreads < 2){
                cerr << "Error: Invalid number of threads.\n Number of threads must be greater than 1." << endl;
                exit(EXIT_FAILURE);  // exit
            }
        } else if (arg == "-c" && i + 1 < argc) {
            cellSize = atoi(argv[++i]);   // no less than 1
            if (cellSize < 1){
                cerr << "Error: Invalid cell size.\n Cell size must be greater than 0." << endl;
                exit(EXIT_FAILURE);  // exit
            }
        } else if (arg == "-x" && i + 1 < argc) {
            windowWidth = atoi(argv[++i]);
            if (windowWidth < 0){
                cerr << "Error: Invalid window width." << endl;
                exit(EXIT_FAILURE);  // exit
            }
        } else if (arg == "-y" && i + 1 < argc) {
            windowHeight = atoi(argv[++i]);
            if (windowHeight < 0){
                cerr << "Error: Invalid window height." << endl;
                exit(EXIT_FAILURE);  // exit
            }
        } else if (arg == "-t" && i + 1 < argc) {
            processingType = argv[++i];
            // check processingType valid?
            if (processingType != "SEQ" && processingType != "THRD" && processingType != "OMP") {
                cerr << "Error: Invalid processing type. Must be one of SEQ, THRD, OMP." << endl;
                exit(EXIT_FAILURE);  // exit
            }
        } else {
            cerr << "Usage: " << argv[0] << " [-n numThreads] [-c cellSize] [-x windowWidth] [-y windowHeight] [-t processingType]" << endl;
            exit(EXIT_FAILURE);
        }
    }
}