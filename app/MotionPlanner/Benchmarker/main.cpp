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
    printf("%d\n", argc);
    if (argc != 2)
    {
        printf("invalid arguments\n");
        printf("usage: benchmarker [env_file]\n");
        exit(1);
    }
    
    MP::Benchmarker benchmarker;
    
    benchmarker.loadEnvironment(argv[1]);
    
//    benchmarker.benchmark(100, MP::Action6D::generate6DActions(benchmarker.getEnvStepSize(), benchmarker.getEnvRotationStepSize()));
//    benchmarker.benchmark(100, MP::Action6D::generate3DActions(benchmarker.getEnvStepSize()));
        benchmarker.benchmark(100, MP::Action6D::generateFalconActions(benchmarker.getEnvStepSize(), benchmarker.getEnvRotationStepSize()));
    
    return 0;
}

