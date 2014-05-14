//
//  main.cpp
//
//  Created by John Visentin on 4/2/14.
//  Copyright (c) 2014 John Visentin. All rights reserved.
//

#include <iostream>
#include "MPBenchmarker.h"
using namespace MP;

int main(int argc, char** argv)
{
    Benchmarker benchmarker;
    
    benchmarker.loadEnvironment("/Users/eratner/Documents/Bowdoin/Spring 2014/Computational Geometry/compgeo/src/geometry/simple.env");
    
    benchmarker.benchmark3D(20);
    
//    MPAABox boundingBox = MPAABoxMake(MPVec3Make(1.0f, 1.0f, 1.0f),
//                                      MPVec3Make(2.0f, 2.0f, 2.0f));
//    VoxelGrid<int> grid(boundingBox, 0.5f);
//    
//    int value;
//    float x, y, z;
//    
//    bool quit = false;
//    while(!quit)
//    {
//        char action;
//        std::cout << "(s)et or (g)et value (or (q)uit)? ";
//        std::cin >> action;
//        switch(action)
//        {
//            case 's':
//            {
//                std::cin >> x >> y >> z >> value;
//                if(grid.setVoxel(MPVec3Make(x, y, z), value))
//                {
//                    std::cout << "set value!" << std::endl;
//                }
//                else
//                {
//                    std::cout << "didn't set value :(" << std::endl;
//                }
//                break;
//            }
//            case 'g':
//            {
//                std::cin >> x >> y >> z;
//                if(grid.getVoxel(MPVec3Make(x, y, z), value))
//                {
//                    std::cout << "found it! val = " << value << std::endl;
//                }
//                else
//                {
//                    std::cout << "not found :(" << std::endl;
//                }
//                break;
//            }
//            case 'q':
//                return 0;
//            default:
//                break;
//        }
//        
//    }
    
    return 0;
}