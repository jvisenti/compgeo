//
//  MPMesh.h
//
//  Created by John Visentin on 4/2/14.
//  Copyright (c) 2014 John Visentin. All rights reserved.
//

#ifndef MotionPlanner_MPMesh_h
#define MotionPlanner_MPMesh_h

#include <stddef.h>
#include "MPMath.h"

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

MPMesh* MPMeshCreate(const MPVec3 *vertexData, size_t stride, size_t numVertices, const void *indexData, size_t indexSize, size_t numIndices);
void MPMeshFree(MPMesh *mesh);

MPSphere MPMeshGetBoundingSphere(const MPMesh *mesh, const MPMat4 *transform);

#endif
