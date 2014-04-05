//
//  MPMesh.c
//
//  Created by John Visentin on 4/2/14.
//  Copyright (c) 2014 John Visentin. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include "MPMesh.h"

#pragma mark - private definitions

MPSphere _MPMeshComputeBoundingSphere(MPMesh *mesh);

#pragma mark - public functions

MPMesh* MPMeshCreate(const MPVec3 *vertexData, size_t stride, size_t numVertices, const void *indexData, size_t indexSize, size_t numIndices)
{
    MPMesh *mesh = malloc(sizeof(MPMesh));
    
    mesh->retainCount = 0;
    
    mesh->vertexData = vertexData;
    mesh->stride = stride;
    mesh->numVertices = numVertices;
    
    mesh->indexData = indexData;
    mesh->indexSize = indexSize;
    mesh->numIndices = numIndices;
    
    mesh->_reserved = malloc(sizeof(MPSphere));
    *(MPSphere *)(mesh->_reserved) = _MPMeshComputeBoundingSphere(mesh);
    
    return mesh;
}

void MPMeshFree(MPMesh *mesh)
{
    if (mesh)
    {
        free(mesh->_reserved);
        free(mesh);
    }
}

void MPMeshRetain(MPMesh *mesh)
{
    if (mesh == NULL) return;
    
    mesh->retainCount++;
}

void MPMeshRelease(MPMesh *mesh)
{
    if (mesh == NULL) return;
    
    if (!mesh->retainCount)
    {
        printf("warning: releasing unretained mesh!\n");
    }
    
    if (mesh->retainCount < 2)
    {
        MPMeshFree(mesh);
    }
    else
    {
        mesh->retainCount--;
    }
}

MPSphere MPMeshGetBoundingSphere(const MPMesh *mesh, const MPMat4 *transform)
{    
    MPSphere boundingSphere = *(MPSphere *)(mesh->_reserved);
    
    if (transform != NULL)
    {
        // apply translation to center point (scale and rotation don't matter)
        boundingSphere.center.x -= transform->m[12];
        boundingSphere.center.y -= transform->m[13];
        boundingSphere.center.z -= transform->m[14];
        
        // scale the radius on the x-axis
        MPVec3 xRad = MPVec3Make(boundingSphere.radius, 0.0f, 0.0f);
        xRad = MPMat4TransformVec3(*transform, xRad);
        xRad.x -= transform->m[12]; xRad.y -= transform->m[13]; xRad.z -= transform->m[14];
        
        // scale the radius on the y-axis
        MPVec3 yRad = MPVec3Make(0.0f, boundingSphere.radius, 0.0f);
        yRad = MPMat4TransformVec3(*transform, yRad);
        yRad.x -= transform->m[12]; yRad.y -= transform->m[13]; yRad.z -= transform->m[14];

        // scale the radius on the z-axis
        MPVec3 zRad = MPVec3Make(0.0f, 0.0f, boundingSphere.radius);
        zRad = MPMat4TransformVec3(*transform, zRad);
        zRad.x -= transform->m[12]; zRad.y -= transform->m[13]; zRad.z -= transform->m[14];
        
        // take the new radius to be the max of the radius scaled on each axis
        boundingSphere.radius = fmaxf(MPVec3Length(xRad), MPVec3Length(yRad));
        boundingSphere.radius = fmaxf(boundingSphere.radius, MPVec3Length(zRad));
    }
    
    return  boundingSphere;
}

#pragma mark - private functions

MPSphere _MPMeshComputeBoundingSphere(MPMesh *mesh)
{
    MPVec3 *left, *right, *bottom, *top, *back, *front;
    left = right = bottom = top = back = front = (MPVec3 *)mesh->vertexData;
    
    int i;
    for (i = 0; i < mesh->numVertices; i++)
    {
        MPVec3 *current = (MPVec3 *)((char *)mesh->vertexData + (i * mesh->stride));
        
        if (current->x < left->x)   left = current;
        if (current->x > right->x)  right = current;
        
        if (current->y < bottom->y) bottom = current;
        if (current->y > top->y)    top = current;
        
        if (current->z < back->z)   back = current;
        if (current->z > front->z)  front = current;
    }
    
    float midX = left->x + 0.5f * (right->x - left->x);
    float midY = bottom->y + 0.5f * (top->y - bottom->y);
    float midZ = back->z + 0.5f * (front->z - back->z);
    
    MPVec3 center = MPVec3Make(midX, midY, midZ);
    
    float maxDist = fmaxf(MPVec3EuclideanDistance(center, *left), MPVec3EuclideanDistance(center, *right));
    
    maxDist = fmaxf(maxDist, MPVec3EuclideanDistance(center, *bottom));
    
    maxDist = fmaxf(maxDist, MPVec3EuclideanDistance(center, *top));
    maxDist = fmaxf(maxDist, MPVec3EuclideanDistance(center, *back));
    maxDist = fmaxf(maxDist, MPVec3EuclideanDistance(center, *front));
    
    return MPSphereMake(center, maxDist);
}