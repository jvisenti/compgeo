//
//  main.cpp
//
//  Created by John Visentin on 4/2/14.
//  Copyright (c) 2014 John Visentin. All rights reserved.
//

#include "MPHeap.h"
#include "MPReader.h"

void testHeap();

void test2DPlanner();

int main(int argc, char** argv)
{
    MP::Reader reader("/Users/Rob/Desktop/scene.env");
    reader.generateEnvironment3D();
    
    return 0;
}