//
//  MPPyramid.h
//
//  Created by John Visentin on 4/4/14.
//  Copyright (c) 2014 John Visentin. All rights reserved.
//

#ifndef _MPPyramid_h
#define _MPPyramid_h

const float PyramidVertices[16][6] = {
    // Front
    {0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f},
    {0.0f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f},
    {-0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f},
    // Back
    {-0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f},
    {0.0f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f},
    {0.5, -0.5, -0.5f, 0.0f, 0.0f, -1.0f},
    // Left
    {-0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f},
    {-0.5f, -0.5f, 0.5f, -1.0f, 0.0f, 0.0f},
    {0.0f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f},
    // Right
    {0.5, -0.5, 0.5, 1.0f, 0.0f, 0.0f},
    {0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f},
    {0.0f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f},
    // Bottom
    {0.5, -0.5, -0.5, 0.0f, -1.0f, 0.0f},
    {0.5, -0.5, 0.5, 0.0f, -1.0f, 0.0f},
    {-0.5, -0.5, 0.5, 0.0f, -1.0f, 0.0f},
    {-0.5, -0.5f, -0.5, 0.0f, -1.0f, 0.0f}
};

const unsigned char PyramidIndices[] = {
    // Front
    0, 1, 2,
    // Back
    3, 4, 5,
    // Left
    6, 7, 8,
    // Right
    9, 10, 11,
    // Bottom
    12, 13, 14,
    14, 15, 12
};

#endif
