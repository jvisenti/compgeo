//
//  MPUtils.h
//
//  Created by John Visentin on 4/2/14.
//  Copyright (c) 2014 John Visentin. All rights reserved.
//

#ifndef MotionPlanner_MPUtils_h
#define MotionPlanner_MPUtils_h

#include "MPMath.h"

static inline void MPVec3Print(MPVec3 v)
{
    printf("(%.2f, %.2f, %.2f)", v.v[0], v.v[1], v.v[2]);
}

static inline void MPSpherePrint(MPSphere s)
{
    printf("{c:");
    MPVec3Print(s.center);
    printf(" r:%.2f}", s.radius);
}

#endif
