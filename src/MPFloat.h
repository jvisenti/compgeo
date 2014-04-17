//
//  MPFloat.h
//  MotionPlanner
//
//  Created by John Visentin on 4/10/14.
//  Copyright (c) 2014 Brockenhaus Studio. All rights reserved.
//

#ifndef MotionPlanner_MPFloat_h
#define MotionPlanner_MPFloat_h

#include <float.h>

#if defined(__cplusplus)
extern "C" {
#endif
    
// we can change this if we want
#define MP_FLT_EPSILON 1e-4
    
static const float Inf = FLT_MAX;
    
static inline int MPFloatZero(float f)
{
    return fabs(f) <= MP_FLT_EPSILON;
}

static inline int MPFloatEqual(float a, float b)
{
    return MPFloatZero(a - b);
}

static inline int MPFloatLess(float a, float b)
{
    return (b - a) > MP_FLT_EPSILON;
}

static inline int MPFloatGreater(float a, float b)
{
    return (a - b) > MP_FLT_EPSILON;
}

static inline int MPFloatLessEq(float a, float b)
{
    return MPFloatLess(a, b) || MPFloatEqual(a, b);
}

static inline int MPFloatGreaterEq(float a, float b)
{
    return MPFloatGreater(a, b) || MPFloatEqual(a, b);
}

#if defined(__cplusplus)
}
#endif

#endif
