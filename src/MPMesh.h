//
//  MPMesh.h
//
//  Created by John Visentin on 4/2/14.
//  Copyright (c) 2014 John Visentin. All rights reserved.
//
// A 3D mesh defined by vertices and indices that form a triangulation of the mesh.
// Note: it is assumed that the triangulation uses distinct triangles and not strips.

#ifndef _MPMesh_h
#define _MPMesh_h

#include <stddef.h>
#include "MPMath.h"

#if defined(__cplusplus)
extern "C" {
#endif

typedef struct _MPMesh
{    
    const MPVec3 *vertexData;
    size_t stride;
    size_t numVertices;
    
    const void *indexData;
    size_t indexSize;
    size_t numIndices;
    
    void *_reserved;
} MPMesh;

/* initialize a new mesh. mesh members shouldn't be changed after creation. */
MPMesh* MPMeshCreate(const MPVec3 *vertexData, size_t stride, size_t numVertices, const void *indexData, size_t indexSize, size_t numIndices);

/* free all memory used by the mesh, including the mesh itself. */
void MPMeshFree(MPMesh *mesh);

/* increment the retain counter of the given mesh, signaling that it is use and shouldn't be freed. */
void MPMeshRetain(MPMesh *mesh);

/* decrement the retain counter of the given mesh. the mesh will be freed if its retain count reaches 0. */
void MPMeshRelease(MPMesh *mesh);
    
int MPMeshGetRefCount(const MPMesh *mesh);
    
/* returns the number of triangles in the triangulation of the mesh represented by the index array. */
size_t MPMeshGetTriangleCount(const MPMesh *mesh);
    
/* finds vertices of the nth triangle in the triangulation. 
   when the method returns, the array pointed to by triangle will contain the 3 vertices. */
void MPMeshGetTriangle(const MPMesh *mesh, size_t n, MPVec3 *triangle);

/* returns the extreme points of the mesh. order is: left, bottom, far, right, top, near */
const MPVec3* MPMeshGetExtremePoints(const MPMesh *mesh);
    
/* returns the bounding sphere of the mesh using the given transform. pass NULL to use identity. */
MPSphere MPMeshGetBoundingSphere(const MPMesh *mesh, const MPMat4 *transform);
    
#if defined(__cplusplus)
}
#endif

#endif
