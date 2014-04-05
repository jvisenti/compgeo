//
//  ConversionUtils.h
//  MotionPlanner
//
//  Created by John Visentin on 4/4/14.
//  Copyright (c) 2014 Brockenhaus Studio. All rights reserved.
//

#ifndef MotionPlanner_ConversionUtils_h
#define MotionPlanner_ConversionUtils_h

#include <GLKit/GLKMath.h>
#include "MPMath.h"

static inline GLKVector3 MPVec3ToGLKVector3(MPVec3 v)
{
    GLKVector3 glkv;
    memcpy(glkv.v, v.v, sizeof(v.v));
    
    return glkv;
}

static inline GLKQuaternion MPQuaternionToGLKQuaternion(MPQuaternion q)
{
    GLKQuaternion glkq;
    memcpy(glkq.q, q.q, sizeof(q.q));
    
    return glkq;
}

static inline GLKMatrix4 MPMat4ToGLKMatrix4(MPMat4 m)
{
    GLKMatrix4 glkm;
    memcpy(glkm.m, m.m, sizeof(m.m));
    
    return glkm;
}

static inline MPVec3 GLKVector3ToMPVec3(GLKVector3 glkv)
{
    MPVec3 v;
    memcpy(v.v, glkv.v, sizeof(glkv.v));
    
    return v;
}

static inline MPQuaternion GLKQuaternionToMPQuaternion(GLKQuaternion glkq)
{
    MPQuaternion q;
    memcpy(q.q, glkq.q, sizeof(glkq.q));
    
    return q;
}

static inline MPMat4 GLKMatrix4ToMPMat4(GLKMatrix4 glkm)
{
    MPMat4 m;
    memcpy(m.m, glkm.m, sizeof(glkm.m));
    
    return m;
}

#endif
