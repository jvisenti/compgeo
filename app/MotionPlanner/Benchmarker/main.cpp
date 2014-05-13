//
//  main.cpp
//  Benchmarker
//
//  Created by Ellis Ratner on 5/10/14.
//

#include <iostream>
#include "MPBenchmarker.h"

int main(int argc, const char * argv[])
{
    MP::Benchmarker benchmarker;
    
    benchmarker.loadEnvironment("/Users/eratner/Documents/Bowdoin/Spring 2014/Computational Geometry/compgeo/src/geometry/simple.env");
    
//    benchmarker.benchmark(100, MP::Action6D::generate6DActions(benchmarker.getEnvStepSize(), benchmarker.getEnvRotationStepSize()));
    benchmarker.benchmark(100, MP::Action6D::generate3DActions(benchmarker.getEnvStepSize()));
    
    return 0;
}

