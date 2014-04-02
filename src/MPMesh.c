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

const int kNumExtrema = 6;

MPVec3** _MPMeshFindExtrema(MPMesh *mesh);

#pragma mark - public functions

MPMesh* MPMeshCreate(const MPVec3 *vertexData, size_t stride, size_t numVertices, const void *indexData, size_t indexSize, size_t numIndices)
{
    MPMesh *mesh = malloc(sizeof(MPMesh));
    
    mesh->vertexData = vertexData;
    mesh->stride = stride;
    mesh->numVertices = numVertices;
    
    mesh->indexData = indexData;
    mesh->indexSize = indexSize;
    mesh->numIndices = numIndices;
    
    mesh->_reserved = _MPMeshFindExtrema(mesh);
    
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

MPSphere MPMeshGetBoundingSphere(const MPMesh *mesh, const MPMat4 *transform)
{    
    MPVec3 **extrema = (MPVec3 **)mesh->_reserved;
    
    MPVec3 left     = *extrema[0];
    MPVec3 right    = *extrema[1];
    
    MPVec3 bottom   = *extrema[2];
    MPVec3 top      = *extrema[3];
    
    MPVec3 back     = *extrema[4];
    MPVec3 front    = *extrema[5];
    
    if (transform != NULL)
    {
        left = MPMat4MultiplyVec3(*transform, left);
        right = MPMat4MultiplyVec3(*transform, right);
        
        bottom = MPMat4MultiplyVec3(*transform, bottom);
        top = MPMat4MultiplyVec3(*transform, top);
        
        back = MPMat4MultiplyVec3(*transform, back);
        front = MPMat4MultiplyVec3(*transform, front);
    }
    
    float midX = left.x + 0.5f * (right.x - left.x);
    float midY = bottom.y + 0.5f * (top.y - bottom.y);
    float midZ = back.z + 0.5f * (front.z - back.z);
    
    MPVec3 center = MPVec3Make(midX, midY, midZ);
    
    float maxDist = fmaxf(MPVec3EuclideanDistance(left, right), MPVec3EuclideanDistance(bottom, top));
    maxDist = fmaxf(maxDist, MPVec3EuclideanDistance(back, front));
    
    return MPSphereMake(center, maxDist);
}

#pragma mark - private functions

MPVec3** _MPMeshFindExtrema(MPMesh *mesh)
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
    
    MPVec3 **extrema = malloc(kNumExtrema * sizeof(MPVec3 *));
    
    extrema[0] = left;     extrema[1] = right;
    extrema[2] = bottom;   extrema[3] = top;
    extrema[4] = back;     extrema[5] = front;
    
    return extrema;
}