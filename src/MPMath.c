//
//  MPMath.c
//
//  Created by John Visentin on 4/2/14.
//  Copyright (c) 2014 John Visentin. All rights reserved.
//

#include <float.h>
#include "MPMath.h"

const MPVec3 MPVec3Zero = {{0.0f, 0.0f, 0.0f}};
const MPQuaternion MPQuaternionIdentity = {{0.0f, 0.0f, 0.0f, 1.0f}};
const MPMat4 MPMat4Identity = {{1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f}};
const float Inf = FLT_MAX;