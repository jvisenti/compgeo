//
//  MPUtils.h
//
//  Created by John Visentin on 4/2/14.
//  Copyright (c) 2014 John Visentin. All rights reserved.
//

#ifndef _MPUtils_h
#define _MPUtils_h

#include <stdio.h>
#include "MPMath.h"

#if defined(__cplusplus)
extern "C" {
#endif

static inline void MPVec3Print(MPVec3 v)
{
    printf("(%.2f, %.2f, %.2f)", v.v[0], v.v[1], v.v[2]);
}

static inline void MPQuaternionPrint(MPQuaternion q)
{
    printf("(%.2f, %.2f, %.2f, %.2f)", q.q[0], q.q[1], q.q[2], q.q[3]);
}

static inline void MPMat4Print(MPMat4 m)
{
    printf("| %.2f, %.2f, %.2f, %.2f |\n| %.2f, %.2f, %.2f, %.2f |\n| %.2f, %.2f, %.2f, %.2f |\n| %.2f, %.2f, %.2f, %.2f |\n", m.m[0], m.m[1], m.m[2], m.m[3], m.m[4], m.m[5], m.m[6], m.m[7], m.m[8], m.m[9], m.m[10], m.m[11], m.m[12], m.m[13], m.m[14], m.m[15]);
}

static inline void MPSpherePrint(MPSphere s)
{
    printf("{c:");
    MPVec3Print(s.center);
    printf(" r:%.2f}", s.radius);
}
    
static inline void MPAABoxPrint(MPAABox b)
{
    printf("{min:");
    MPVec3Print(b.min);
    printf(" max:");
    MPVec3Print(b.max);
    printf("}");
}
    
#if defined(__cplusplus)
}
#endif

#endif
