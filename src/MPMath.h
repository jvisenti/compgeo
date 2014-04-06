//
//  MPMath.h
//
//  Created by John Visentin on 4/2/14.
//  Copyright (c) 2014 John Visentin. All rights reserved.
//

#ifndef _MPMath_h
#define _MPMath_h

#include <math.h>

#if defined(__cplusplus)
extern "C" {
#endif
    
typedef union _MPVec2
{
    struct {float x, y;};
    struct {float w, h;};
    float v[2];
} MPVec2;

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
    
extern const MPVec3 MPVec3Zero;
extern const MPQuaternion MPQuaternionIdentity;
extern const MPMat4 MPMat4Identity;
    
static inline int MPFloatZero(float f)
{
    return fabs(f) <= 1e-5;
}

#pragma mark - vector2 functions
    
static inline MPVec2 MPVec2Make(float x, float y)
{
    MPVec2 v;
    v.v[0] = x; v.v[1] = y;
    return v;
}
    
static inline MPVec2 MPVec2FromVec3(MPVec3 v3)
{
    return MPVec2Make(v3.v[0], v3.v[1]);
}
    
static inline float MPVec2SignedArea(MPVec2 a, MPVec2 b, MPVec2 c)
{
    
    MPVec2 p1 = MPVec2Make(b.x - a.x, b.y - a.y);
    MPVec2 p2 = MPVec2Make(c.x - a.x, c.y - a.y);
    
    return p1.x * p2.y - p1.y * p2.x;
}

static inline int MPVec2CCW(MPVec2 a, MPVec2 b, MPVec2 c)
{
    return MPVec2SignedArea(a, b, c) > 0;
}
    
static inline int MPVec2Collinear(MPVec2 a, MPVec2 b, MPVec2 c)
{
    return MPVec2SignedArea(a, b, c) == 0;
}

static inline int MPVec2ProperLineIntersection(MPVec2 a, MPVec2 b, MPVec2 c, MPVec2 d)
{
    int abOpposite = MPVec2CCW(a, b, c) != MPVec2CCW(a, b, d);
    int cdOpposite = MPVec2CCW(c, d, a) != MPVec2CCW(c, d, b);
    
    int cCollinear = MPVec2Collinear(a, b, c);
    int dCollinear = MPVec2Collinear(a, b, d);
    
    return abOpposite && cdOpposite && !cCollinear && !dCollinear;
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
    return (MPFloatZero(v1.x - v2.x) && MPFloatZero(v1.y - v2.y) && MPFloatZero(v1.z -v2.z));
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
    
#pragma mark - triangle functions
    
static inline void MPTriangleApplyTransform(MPTriangle *t, MPMat4 m)
{
    t->p[0] = MPMat4TransformVec3(m, t->p[0]);
    t->p[1] = MPMat4TransformVec3(m, t->p[1]);
    t->p[2] = MPMat4TransformVec3(m, t->p[2]);
}
    
/* returns 1 if the line segment v1v2 intersects the line with parametric form
   x = p0 + tV */
static inline int MPTriangleEdgeIntersectsLine(MPVec3 v1, MPVec3 v2, MPVec3 p0, MPVec3 v)
{
    MPVec3 diff = MPVec3Subtract(v2, v1);
    
    float s = ((-v.y) * (p0.x - v1.x) + v.x * (p0.y - v1.y)) / ((-v.y) * diff.x + v.x * diff.y);
    
    return (s >= 0.0f && s <= 1.0f);
}
    
/* a line is represented in parametric form as x = p0 + tV */
static inline int MPTriangleIntersectsLine(MPTriangle t, MPVec3 p0, MPVec3 v)
{
    return (MPTriangleEdgeIntersectsLine(t.v1, t.v2, p0, v) ||
            MPTriangleEdgeIntersectsLine(t.v1, t.v3, p0, v) ||
            MPTriangleEdgeIntersectsLine(t.v2, t.v3, p0, v));
}
    
/* adapted from www.applet-magic.com/trintersection.htm */
static inline int MPTrianglesIntersect(MPTriangle t1, MPTriangle t2)
{
    MPVec3 n1 = MPVec3CrossProduct(MPVec3Subtract(t1.v2, t1.v1), MPVec3Subtract(t1.v3, t1.v1));
    MPVec3 n2 = MPVec3CrossProduct(MPVec3Subtract(t2.v2, t2.v1), MPVec3Subtract(t2.v3, t2.v1));
    
    // parallel to line of intersection of planes
    MPVec3 v = MPVec3CrossProduct(n1, n2);
    
    MPVec3 p0;
    
    if (MPVec3EqualToVec3(v, MPVec3Zero))
    {
        float d = MPVec3DotProduct(n1, t1.v1);
        float diff = MPVec3DotProduct(n1, t2.v1) - d;
        
        if (!MPFloatZero(diff))
        {
            // triangles are in parallel planes and not coplanar, so no intersection
            return 0;
        }
        else
        {
            // triangles are coplanar, so use 2D interseciton test
            MPVec2 t1v1 = MPVec2FromVec3(t1.v1);
            MPVec2 t1v2 = MPVec2FromVec3(t1.v2);
            MPVec2 t1v3 = MPVec2FromVec3(t1.v3);

            MPVec2 t2v1 = MPVec2FromVec3(t2.v1);
            MPVec2 t2v2 = MPVec2FromVec3(t2.v2);
            MPVec2 t2v3 = MPVec2FromVec3(t2.v3);

            return (MPVec2ProperLineIntersection(t1v1, t1v2, t2v1, t2v2) ||
                    MPVec2ProperLineIntersection(t1v1, t1v2, t2v1, t2v3) ||
                    MPVec2ProperLineIntersection(t1v1, t1v2, t2v2, t2v3) ||
                    MPVec2ProperLineIntersection(t1v1, t1v3, t2v1, t2v2) ||
                    MPVec2ProperLineIntersection(t1v1, t1v3, t2v1, t2v3) ||
                    MPVec2ProperLineIntersection(t1v1, t1v3, t2v2, t2v3) ||
                    MPVec2ProperLineIntersection(t1v2, t1v3, t2v1, t2v2) ||
                    MPVec2ProperLineIntersection(t1v2, t1v3, t2v1, t2v3) ||
                    MPVec2ProperLineIntersection(t1v2, t1v3, t2v2, t2v3));
        }
        
    }
    
    // let p0.z = 0 and solve for p0.x and p0.y
    
    float n1dott1 = MPVec3DotProduct(n1, t1.v1);
    float n2dott2 = MPVec3DotProduct(n2, t2.v1);
    
    p0.x = ((-n2.y) * n1dott1 + (n1.y) * n2dott2) / (n1.y * n2.x - n1.x * n2.y);
    p0.y = ((-n2.x) * n1dott1 + (n1.x) * n2dott2) / (n1.x * n2.y - n1.y * n2.x);
    
    return MPTriangleIntersectsLine(t1, p0, v) || MPTriangleIntersectsLine(t2, p0, v);
}
    
#if defined(__cplusplus)
}
#endif

#endif
