//
//  MPMath.h
//
//  Created by John Visentin on 4/2/14.
//  Copyright (c) 2014 John Visentin. All rights reserved.
//

#ifndef MotionPlanner_MPMath_h
#define MotionPlanner_MPMath_h

#include <math.h>

typedef union _MPVector3
{
    struct {float x, y, z;};
    struct {float w, h, d;};
    float v[3];
} MPVector3;

typedef union _MPQuaternion
{
    struct {MPVector3 v; float s;};
    struct {float x, y, z, w;};
    float q[4];
}__attribute__((aligned(16))) MPQuaternion;

static inline MPVector3 MPVector3Make(float x, float y, float z)
{
    MPVector3 v;
    v.v[0] = x; v.v[1] = y; v.v[2] = z;
    return v;
}

static inline MPVector3 MPVector3Add(MPVector3 v1, MPVector3 v2)
{
    return MPVector3Make(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z);
}

static inline MPVector3 MPVector3Subtract(MPVector3 v1, MPVector3 v2)
{
    return MPVector3Make(v1.x - v2.x, v1.y - v2.y, v1.z - v2.z);
}

static inline MPVector3 MPVector3MultiplyScalar(MPVector3 v, float scalar)
{
    return MPVector3Make(v.x * scalar, v.y * scalar, v.z * scalar);
}

static inline int MPVector3EqualToVector3(MPVector3 v1, MPVector3 v2)
{
    return (v1.x == v2.x && v1.y == v2.y && v1.z == v2.z);
}

static inline MPQuaternion MPQuaternionMake(float x, float y, float z, float w)
{
    MPQuaternion q;
    q.q[0] = x; q.q[1] = y; q.q[2] = z; q.q[3] = w;
    
    return q;
}

static inline MPQuaternion MPQuaternionMakeWithAngleAndAxis(float angle, float x, float y, float z)
{
    float half = 0.5f * angle;
    float scale = sinf(half);
    
    return MPQuaternionMake(scale * x, scale * y, scale * z, cosf(half));
}

static inline MPQuaternion MPQuaternionInvert(MPQuaternion q)
{
    float scale = 1.0f / (q.x * q.x + q.y * q.y + q.z * q.z + q.w * q.w);
    
    return MPQuaternionMake(-q.x * scale, -q.y * scale, -q.z * scale, q.w * scale);
}

static inline MPQuaternion MPQuaternionMultiply(MPQuaternion q1, MPQuaternion q2)
{
    return MPQuaternionMake(
        q1.w * q2.x +
        q1.x * q2.w +
        q1.y * q2.z -
        q1.z * q2.y,
        
        q1.w * q2.y +
        q1.y * q2.w +
        q1.z * q2.x -
        q1.x * q2.z,
        
        q1.w * q2.z +
        q1.z * q2.w +
        q1.x * q2.y -
        q1.y * q2.x,
        
        q1.w * q2.w -
        q1.x * q2.x -
        q1.y * q2.y -
        q1.z * q2.z);
}

static inline MPVector3 MPQuaternionRotateVector3(MPQuaternion q, MPVector3 v)
{
    MPQuaternion rq = MPQuaternionMake(v.x, v.y, v.z, 0.0f);
    rq = MPQuaternionMultiply(MPQuaternionMultiply(q, rq), MPQuaternionInvert(q));
    
    return MPVector3Make(round(rq.x), round(rq.y), round(rq.z));
}

#endif
