//
//  MPMath.h
//
//  Created by John Visentin on 4/2/14.
//  Copyright (c) 2014 John Visentin. All rights reserved.
//

#ifndef _MPMath_h
#define _MPMath_h

#include <math.h>
#include <stdio.h>

#if defined(__cplusplus)
extern "C" {
#endif

typedef union _MPVec3
{
    struct {float x, y, z;};
    struct {float w, h, d;};
    float v[3];
} MPVec3;

typedef union _MPQuaternion
{
    struct {float x, y, z, w;};
    float q[4];
}__attribute__((aligned(16))) MPQuaternion;

typedef union _MPMat4
{
    struct{float m00, m01, m02, m03, m10, m11, m12, m13, m20, m21, m22, m23, m30, m31, m32, m33;};
    float m[16];
} __attribute__((aligned(16))) MPMat4;

typedef struct _MPSphere
{
    MPVec3 center;
    float radius;
} MPSphere;
    
typedef union _MPTriangle
{
    struct {MPVec3 v1, v2, v3;};
    MPVec3 p[3];
} MPTriangle;
    
typedef union _MPLineSegment
{
    struct {MPVec3 p1, p2;};
    MPVec3 p[2];
} MPLineSegment;
    
typedef struct _MPLine
{
    MPVec3 p0, v;  // line of form p0 + vt
} MPLine;
    
extern const MPVec3 MPVec3Zero;
extern const MPQuaternion MPQuaternionIdentity;
extern const MPMat4 MPMat4Identity;
extern const float Inf;
    
static inline int MPFloatZero(float f)
{
    return fabs(f) <= 1e-5;
}
    
static inline int MPFloatEqual(float a, float b)
{
    return MPFloatZero(a - b);
}

#pragma mark - vector3 functions

static inline MPVec3 MPVec3Make(float x, float y, float z)
{
    MPVec3 v;
    v.v[0] = x; v.v[1] = y; v.v[2] = z;
    return v;
}

static inline MPVec3 MPVec3Add(MPVec3 v1, MPVec3 v2)
{
    return MPVec3Make(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z);
}

static inline MPVec3 MPVec3Subtract(MPVec3 v1, MPVec3 v2)
{
    return MPVec3Make(v1.x - v2.x, v1.y - v2.y, v1.z - v2.z);
}

static inline MPVec3 MPVec3MultiplyScalar(MPVec3 v, float scalar)
{
    return MPVec3Make(v.x * scalar, v.y * scalar, v.z * scalar);
}

static inline int MPVec3EqualToVec3(MPVec3 v1, MPVec3 v2)
{
    return (MPFloatEqual(v1.x, v2.x) && MPFloatEqual(v1.y, v2.y) && MPFloatEqual(v1.z, v2.z));
}
    
static inline float MPVec3Length(MPVec3 v)
{
    return sqrt((v.v[0] * v.v[0]) + (v.v[1] * v.v[1]) + (v.v[2] * v.v[2]));
}

static inline MPVec3 MPVec3Normalize(MPVec3 v)
{
    float len = MPVec3Length(v);
    return MPVec3MultiplyScalar(v, 1.0f / len);
}
    
static inline float MPVec3EuclideanDistance(MPVec3 v1, MPVec3 v2)
{
    return sqrt((v1.x - v2.x) * (v1.x - v2.x) + (v1.y - v2.y) * (v1.y - v2.y) + (v1.z - v2.z) * (v1.z - v2.z));
}

static inline float MPVec3DotProduct(MPVec3 v1, MPVec3 v2)
{
    return (v1.v[0] * v2.v[0]) + (v1.v[1] * v2.v[1]) + (v1.v[2] * v2.v[2]);
}

static inline MPVec3 MPVec3CrossProduct(MPVec3 v1, MPVec3 v2)
{
    MPVec3 v;
    
    v.v[0] = (v1.v[1] * v2.v[2]) - (v1.v[2] * v2.v[1]);
    v.v[1] = (v1.v[2] * v2.v[0]) - (v1.v[0] * v2.v[2]);
    v.v[2] = (v1.v[0] * v2.v[1]) - (v1.v[1] * v2.v[0]);
    
    return v;
}
    
static inline MPVec3 MPVec3Project(MPVec3 a, MPVec3 b)
{
    float scale = MPVec3DotProduct(a, b) / MPVec3DotProduct(b, b);
    
    return MPVec3MultiplyScalar(b, scale);
}

#pragma mark - quaternion functions

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
    
static float MPQuaternionLength(MPQuaternion q)
{
    return sqrt((q.q[0] * q.q[0]) + (q.q[1] * q.q[1]) + (q.q[2] * q.q[2]) + (q.q[3] * q.q[3]));
}
    
static inline MPQuaternion MPQuaternionNormalize(MPQuaternion q)
{
    float len = MPQuaternionLength(q);
    
    return MPQuaternionMake(q.q[0] / len, q.q[1] / len, q.q[2] / len, q.q[3] / len);
}

static inline MPQuaternion MPQuaternionInvert(MPQuaternion q)
{
    float scale = 1.0f / (q.x * q.x + q.y * q.y + q.z * q.z + q.w * q.w);
    
    return MPQuaternionMake(-q.x * scale, -q.y * scale, -q.z * scale, q.w * scale);
}

static inline MPQuaternion MPQuaternionMultiply(MPQuaternion q1, MPQuaternion q2)
{
    return MPQuaternionMake(q1.w * q2.x + q1.x * q2.w + q1.y * q2.z - q1.z * q2.y,
                            q1.w * q2.y + q1.y * q2.w + q1.z * q2.x - q1.x * q2.z,
                            q1.w * q2.z + q1.z * q2.w + q1.x * q2.y - q1.y * q2.x,
                            q1.w * q2.w - q1.x * q2.x - q1.y * q2.y - q1.z * q2.z);
}

static inline MPVec3 MPQuaternionRotateVec3(MPQuaternion q, MPVec3 v)
{
    MPQuaternion rq = MPQuaternionMake(v.x, v.y, v.z, 0.0f);
    rq = MPQuaternionMultiply(MPQuaternionMultiply(q, rq), MPQuaternionInvert(q));
    
    return MPVec3Make(rq.x, rq.y, rq.z);
}

#pragma mark - matrix functions

static inline MPMat4 MPMat4MakeTranslation(MPVec3 translation)
{
    MPMat4 m = MPMat4Identity;
    
    m.m[12] = translation.x;
    m.m[13] = translation.y;
    m.m[14] = translation.z;
    
    return m;
}
    
static inline MPMat4 MPMat4MakeScale(MPVec3 scale)
{
    MPMat4 m = MPMat4Identity;
    
    m.m[0]  = scale.x;
    m.m[5]  = scale.y;
    m.m[10] = scale.z;
    
    return m;
}
    
static inline MPMat4 MPMat4MakeRotation(MPQuaternion rotation)
{
    rotation = MPQuaternionNormalize(rotation);
    
    float x = rotation.x;
    float y = rotation.y;
    float z = rotation.z;
    float w = rotation.w;
    
    float xx = x + x;
    float yy = y + y;
    float zz = z + z;
    float ww = w + w;
    
    MPMat4 m = MPMat4Identity;
    
    m.m[0]  = 1.0f - yy*y - zz*z;
    m.m[1]  = xx*y + ww*z;
    m.m[2]  = xx*z - ww*y;
    
    m.m[4]  = xx*y - ww*z;
    m.m[5]  = 1.0f - xx*x - zz*z;
    m.m[6]  = yy*z + ww*x;
    
    m.m[8]  = xx*z + ww*y;
    m.m[9]  = yy*z - ww*x;
    m.m[10] = 1.0f - xx*x - yy*y;
    
    m.m[15] = 1.0f;
    
    return m;
}
    
static inline MPMat4 MPMat4Multiply(MPMat4 left, MPMat4 right)
{
    MPMat4 m;
    
    m.m[0]  = left.m00 * right.m00 + left.m10 * right.m01 + left.m20 * right.m02 + left.m30 * right.m03;
	m.m[4]  = left.m00 * right.m10 + left.m10 * right.m11 + left.m20 * right.m12 + left.m30 * right.m13;
	m.m[8]  = left.m00 * right.m20 + left.m10 * right.m12 + left.m20 * right.m22 + left.m30 * right.m23;
	m.m[12] = left.m00 * right.m30 + left.m10 * right.m13 + left.m20 * right.m32 + left.m30 * right.m33;
    
	m.m[1]  = left.m01 * right.m00 + left.m11 * right.m01 + left.m21 * right.m02 + left.m31 * right.m03;
	m.m[5]  = left.m01 * right.m10 + left.m11 * right.m11 + left.m21 * right.m12 + left.m31 * right.m12;
	m.m[9]  = left.m01 * right.m20 + left.m11 * right.m21 + left.m21 * right.m22 + left.m31 * right.m23;
	m.m[13] = left.m01 * right.m30 + left.m11 * right.m31 + left.m21 * right.m32 + left.m31 * right.m33;
    
	m.m[2]  = left.m02 * right.m00 + left.m12 * right.m01 + left.m22 * right.m02 + left.m32 * right.m03;
	m.m[6]  = left.m02 * right.m10 + left.m12 * right.m11 + left.m22 * right.m12 + left.m32 * right.m13;
	m.m[10] = left.m02 * right.m20 + left.m12 * right.m21 + left.m22 * right.m22 + left.m32 * right.m23;
	m.m[14] = left.m02 * right.m30 + left.m12 * right.m31 + left.m22 * right.m32 + left.m32 * right.m33;
    
	m.m[3]  = left.m03 * right.m00 + left.m13 * right.m01 + left.m23 * right.m02 + left.m33 * right.m03;
	m.m[7]  = left.m03 * right.m10 + left.m13 * right.m11 + left.m23 * right.m12 + left.m33 * right.m13;
	m.m[11] = left.m03 * right.m20 + left.m13 * right.m21 + left.m23 * right.m22 + left.m33 * right.m23;
	m.m[15] = left.m03 * right.m30 + left.m13 * right.m31 + left.m23 * right.m32 + left.m33 * right.m33;
    
    return m;
}

static inline MPVec3 MPMat4TransformVec3(MPMat4 m, MPVec3 v)
{
    MPVec3 mv;
    
    mv.v[0] = m.m00 * v.v[0] + m.m10 * v.v[1] + m.m20 * v.v[2] + m.m30;
    mv.v[1] = m.m01 * v.v[0] + m.m11 * v.v[1] + m.m21 * v.v[2] + m.m31;
    mv.v[2] = m.m02 * v.v[0] + m.m12 * v.v[1] + m.m22 * v.v[2] + m.m32;
    
    return mv;
}

#pragma mark - sphere functions

static inline MPSphere MPSphereMake(MPVec3 center, float radius)
{
    MPSphere m;
    m.center = center; m.radius = radius;
    
    return m;
}

static inline int MPSphereIntersectsSphere(MPSphere s1, MPSphere s2)
{
    return MPVec3EuclideanDistance(s1.center, s2.center) <= s1.radius + s2.radius;
}
    
#pragma mark - line functions
    
static inline MPLineSegment MPLineSegmentMake(MPVec3 p1, MPVec3 p2)
{
    MPLineSegment l;
    l.p1 = p1; l.p2 = p2;
    return l;
}
    
static inline MPLine MPLineMake(MPVec3 p0, MPVec3 v)
{
    MPLine l;
    l.p0 = p0; l.v = v;
    return l;
}
    
/* returns 1 if l intersects the plane of points perpendicular to n, one of which is v0. */
static inline int MPLineSegmentIntersectsPlane(MPLineSegment l, MPVec3 n, MPVec3 v0)
{
    float dot1 = MPVec3DotProduct(n, MPVec3Subtract(l.p1, v0));
    float dot2 = MPVec3DotProduct(n, MPVec3Subtract(l.p2, v0));
    
    return (dot1 >= 0 && dot2 <= 0) || (dot1 <=0 && dot2 >= 0);
}
    
static inline int MPCollinearLineSegmentsIntersect(MPLineSegment l1, MPLineSegment l2)
{
    int intersection;
    
    int l1degenerate = MPVec3EqualToVec3(l1.p1, l1.p2);
    int l2degenerate = MPVec3EqualToVec3(l2.p1, l2.p2);
    
    if (l1degenerate && l2degenerate)
    {
        intersection = MPVec3EqualToVec3(l1.p1, l2.p1);
    }
    else if (l1degenerate)
    {
        MPVec3 dir = MPVec3Subtract(l2.p2, l2.p1);
        float dot1 = MPVec3DotProduct(dir, MPVec3Subtract(l1.p1, l2.p1));
        float dot2 = MPVec3DotProduct(dir, MPVec3Subtract(l1.p1, l2.p2));
        
        intersection = (dot1 < 0 && dot2 > 0) || (dot1 > 0 && dot2 < 0);
    }
    else if (l2degenerate)
    {
        MPVec3 dir = MPVec3Subtract(l1.p2, l1.p1);
        float dot1 = MPVec3DotProduct(dir, MPVec3Subtract(l2.p1, l1.p1));
        float dot2 = MPVec3DotProduct(dir, MPVec3Subtract(l2.p1, l1.p2));
        
        intersection = (dot1 < 0 && dot2 > 0) || (dot1 > 0 && dot2 < 0);
    }
    else if (MPVec3DotProduct(MPVec3Subtract(l1.p2, l1.p1), MPVec3Subtract(l2.p1, l1.p1)) > 0)
    {
        // t1s < t2s on this edge i.e. a.___.b c.___.d
        intersection = MPVec3DotProduct(MPVec3Subtract(l1.p2, l1.p1), MPVec3Subtract(l2.p1, l1.p2)) < 0;
    }
    else
    {
        // t1s > t2s on this edge i.e. c.___.d a.___.b
        intersection = MPVec3DotProduct(MPVec3Subtract(l2.p2, l2.p1), MPVec3Subtract(l1.p1, l2.p2)) < 0;
    }
    
    return intersection;
}
    
/* l1,l2 of the form p0 + Vt, q0 + Us. */
static inline MPVec3 MPLineGetIntersection(MPLine l1, MPLine l2, float *t, float *s)
{
    if (MPFloatEqual(fabsf(MPVec3DotProduct(l1.v, l2.v)), MPVec3Length(l1.v) * MPVec3Length(l2.v)))
    {
        // lines are parallel, so no intersection
        *t = Inf;
        *s = Inf;
        return MPVec3Make(Inf, Inf, Inf);
    }
    
    // NOTE: should probably check if lines are skew as well
    // (doesn't matter in context of triangle intersection though)
    
    MPVec3 diff = MPVec3Subtract(l2.p0, l1.p0);
    
    // find an axis for which U is non-zero
    int a0 = 0;
    
    if (!MPFloatZero(l2.v.y))      a0 = 1;
    else if (!MPFloatZero(l2.v.z)) a0 = 2;
    
    // solve equations for other 2 axes
    int a1 = (a0 + 3 - 1) % 3;
    int a2 = (a0 + 1) % 3;
    
    float denom = (l2.v.v[a1] * l1.v.v[a0] - l2.v.v[a0] * l1.v.v[a1]);
    
    if (!MPFloatZero(denom))
    {
        *t = (diff.v[a0] * l2.v.v[a1] - l2.v.v[a0] * diff.v[a1]) / denom;
    }
    else
    {
        float denom2 = (l2.v.v[a2] * l1.v.v[a0] - l2.v.v[a0] * l1.v.v[a2]);
        
        *t = (diff.v[a0] * l2.v.v[a2] - l2.v.v[a0] * diff.v[a2]) / denom2;
    }
    
    // plug t back in to find s
    *s = (-diff.v[a0] + (*t * l1.v.v[a0])) / l2.v.v[a0];
    
    MPVec3 intersectionPoint = MPVec3Add(MPVec3MultiplyScalar(l1.v, *t), l1.p0);
    
    return intersectionPoint;
}
    
#pragma mark - triangle functions
    
static inline void MPTriangleApplyTransform(MPTriangle *t, MPMat4 m)
{
    t->p[0] = MPMat4TransformVec3(m, t->p[0]);
    t->p[1] = MPMat4TransformVec3(m, t->p[1]);
    t->p[2] = MPMat4TransformVec3(m, t->p[2]);
}
    
/* returns the line segment resulting from projecting the vertices of t onto v. */
static inline MPLineSegment MPTriangleProject(MPTriangle t, MPVec3 v)
{
    // indices into triangle
    int max = 0;
    int min = 0;
    
    int i;
    for (i = 0; i < 3; i++)
    {
        if (MPVec3DotProduct(v, MPVec3Subtract(t.p[i], t.p[max])) > 0)
        {
            max = i;
        }
        
        else if (MPVec3DotProduct(v, MPVec3Subtract(t.p[i], t.p[min])) < 0)
        {
            min = i;
        }
    }
    
    MPLineSegment line;
    line.p1 = MPVec3Project(t.p[min], v);
    line.p2 = MPVec3Project(t.p[max], v);
    
    return line;
}
    
/* results undefined if t1, t2 are not coplanar. used method of separating axis */
static inline int MPCoplanarTrianglesIntersect(MPTriangle t1, MPTriangle t2)
{
    int intersection = 1;
    
    MPLineSegment t1s, t2s;
    
    MPVec3 edges[6];
    edges[0] = MPVec3Subtract(t1.v1, t1.v2);
    edges[1] = MPVec3Subtract(t1.v1, t1.v3);
    edges[2] = MPVec3Subtract(t1.v2, t1.v3);
    
    edges[3] = MPVec3Subtract(t2.v1, t2.v2);
    edges[4] = MPVec3Subtract(t2.v1, t2.v3);
    edges[5] = MPVec3Subtract(t2.v2, t2.v3);
    
    // project each triangle to every possible edge. if it exists,
    // axis of separation will be perpendicular to one of the edges of the triangles
    int i;
    for (i = 0; i < 6; i++)
    {
        t1s = MPTriangleProject(t1, edges[i]);
        t2s = MPTriangleProject(t2, edges[i]);
        
        intersection = MPCollinearLineSegmentsIntersect(t1s, t2s);
        
        // if we found an axis of separation, we're done
        if (!intersection) break;
    }
    
    return intersection;
}
    
/* if intersection exists, returns 1 and intersection is the segment of intersection.
   if no intersection exists, returns 0 and intersection is unchanged. */
static inline int MPTriangleIntersectsLine(MPTriangle tri, MPLine l, MPLineSegment *intersection)
{
    MPVec3 edges[3];
    edges[0] = MPVec3Subtract(tri.v2, tri.v1);
    edges[1] = MPVec3Subtract(tri.v3, tri.v2);
    edges[2] = MPVec3Subtract(tri.v1, tri.v3);
    
    MPVec3 intersections[3];
    
    float t[3];
    float s;
    
    int i;
    for (i = 0; i < 3; i++)
    {
        intersections[i] = MPLineGetIntersection(MPLineMake(tri.p[i], edges[i]), l, &t[i], &s);
    }
    
    // not all 3 edges can intersect, so at least one must not
    int noInt;
    
    // TODO: use epsilon because float arithmetic?
    if (t[0] < 0.0 || t[0] > 1.0)       noInt = 0;
    else if (t[1] < 0.0 || t[1] > 1.0)  noInt = 1;
    else if (t[2] < 0.0 || t[2] > 1.0)  noInt = 2;
    
    int t1 = (noInt + 3 - 1) % 3;
    int t2 = (noInt + 1) % 3;
    
    if (t[t1] >= 0.0 && t[t1] <= 1.0 && t[t2] >= 0.0 && t[t2] <= 1.0)
    {
        if (MPVec3DotProduct(l.v, MPVec3Subtract(intersections[t2], intersections[t1])) > 0)
        {
            intersection->p1 = intersections[t1];
            intersection->p2 = intersections[t2];
        }
        else
        {
            intersection->p1 = intersections[t2];
            intersection->p2 = intersections[t1];
        }

        return 1;
    }

    return 0;
}
    
static inline int MPTrianglesIntersect(MPTriangle t1, MPTriangle t2)
{
    // find plane normals
    MPVec3 n1 = MPVec3CrossProduct(MPVec3Subtract(t1.v2, t1.v1), MPVec3Subtract(t1.v3, t1.v1));
    MPVec3 n2 = MPVec3CrossProduct(MPVec3Subtract(t2.v2, t2.v1), MPVec3Subtract(t2.v3, t2.v1));
    
    float n1dott1 = MPVec3DotProduct(n1, t1.v1);
    float n2dott2 = MPVec3DotProduct(n2, t2.v1);
    
    // intersection line of the two planes
    MPLine pInt;
    
    // parallel to line of intersection of planes
    pInt.v = MPVec3CrossProduct(n1, n2);
    
    // check if planes are parallel
    if (MPVec3EqualToVec3(pInt.v, MPVec3Zero))
    {
        float diff = MPVec3DotProduct(n1, t2.v1) - n1dott1;
        
        if (MPFloatZero(diff))
        {
            // triangles are coplanar, so use 2D intersection test
            return MPCoplanarTrianglesIntersect(t1, t2);
        }

        // triangles are in parallel planes and not coplanar, so no intersection
        return 0;
    }
    
    if (!MPCollinearLineSegmentsIntersect(MPTriangleProject(t1, pInt.v), MPTriangleProject(t2, pInt.v)))
    {
        // projections of triangles onto intersection line don't even intersect, so no intersection
        return 0;
    }
    
    // find an axis for which v is non-zero
    int a0 = 0;
    
    if (!MPFloatZero(pInt.v.y))      a0 = 1;
    else if (!MPFloatZero(pInt.v.z)) a0 = 2;
    
    // let one coordinate of p0 be 0
    pInt.p0.v[a0] = 0.0f;
    
    // solve for the other two coordinates
    int a1 = (a0 + 3 - 1) % 3;
    int a2 = (a0 + 1) % 3;
    
    pInt.p0.v[a1] = (n1.v[a2] * n2dott2 - n2.v[a2] * n1dott1) / (n1.v[a2] * n2.v[a1] - n1.v[a1] * n2.v[a2]);
    pInt.p0.v[a2] = (n1.v[a1] * n2dott2 - n2.v[a1] * n1dott1) / (n1.v[a1] * n2.v[a2] - n1.v[a2] * n2.v[a1]);
    
    MPLineSegment t1s, t2s;
    
    if (MPTriangleIntersectsLine(t1, pInt, &t1s) && MPTriangleIntersectsLine(t2, pInt, &t2s))
    {
        return MPCollinearLineSegmentsIntersect(t1s, t2s);
    }
    
    return 0;
}
    
#if defined(__cplusplus)
}
#endif

#endif
