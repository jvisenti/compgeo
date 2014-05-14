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

typedef struct _MPMeshPrivate
{
    int refCount;
    MPVec3 extremePoints[6]; // left, bottom, far, right, top, near
    MPSphere boundingSphere;
} MPMeshPrivate;

void _MPMeshComputePrivate(MPMesh *mesh);

int _MPMeshVoxelCollision(const MPTriangle *faces, size_t n, const MPMesh *voxMesh, MPMat4 voxTransform);

const float CubeVertices[24][6] = {
    // Front
    {0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f},
    {0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f},
    {-0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f},
    {-0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f},
    // Back
    {0.5, -0.5, -0.5f, 0.0f, 0.0f, -1.0f},
    {-0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f},
    {-0.5, 0.5, -0.5f, 0.0f, 0.0f, -1.0f},
    {0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f},
    // Left
    {-0.5f, -0.5f, 0.5f, -1.0f, 0.0f, 0.0f},
    {-0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f},
    {-0.5f, 0.5f, -0.5f, -1.0f, 0.0f, 0.0f},
    {-0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f},
    // Right
    {0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f},
    {0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f},
    {0.5f, 0.5, 0.5, 1.0f, 0.0f, 0.0f},
    {0.5, -0.5, 0.5, 1.0f, 0.0f, 0.0f},
    // Top
    {0.5, 0.5, 0.5, 0.0f, 1.0f, 0.0f},
    {0.5, 0.5, -0.5, 0.0f, 1.0f, 0.0f},
    {-0.5, 0.5, -0.5, 0.0f, 1.0f, 0.0f},
    {-0.5, 0.5, 0.5, 0.0f, 1.0f, 0.0f},
    // Bottom
    {0.5, -0.5, -0.5, 0.0f, -1.0f, 0.0f},
    {0.5, -0.5, 0.5, 0.0f, -1.0f, 0.0f},
    {-0.5, -0.5, 0.5, 0.0f, -1.0f, 0.0f},
    {-0.5, -0.5f, -0.5, 0.0f, -1.0f, 0.0f}
};

const unsigned int CubeIndices[] = {
    // Front
    0, 1, 2,
    2, 3, 0,
    // Back
    4, 5, 6,
    6, 7, 4,
    // Left
    8, 9, 10,
    10, 11, 8,
    // Right
    12, 13, 14,
    14, 15, 12,
    // Top
    16, 17, 18,
    18, 19, 16,
    // Bottom
    20, 21, 22,
    22, 23, 20
};

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
    
    mesh->texName = NULL;
    
    MPMeshPrivate *priv = malloc(sizeof(MPMeshPrivate));
    priv->refCount = 0;
    
    mesh->_reserved = priv;
    
    _MPMeshComputePrivate(mesh);
    
    return mesh;
}

MPMesh* MPMeshCreateCube()
{
    return MPMeshCreate((const MPVec3 *)CubeVertices, sizeof(CubeVertices[0]), sizeof(CubeVertices)/sizeof(CubeVertices[0]), (const void *)CubeIndices, sizeof(CubeIndices[0]), sizeof(CubeIndices)/sizeof(CubeIndices[0]));
}

void MPMeshFree(MPMesh *mesh)
{
    if (mesh)
    {
        free((void *)mesh->texName);
        free(mesh->_reserved);
        free(mesh);
    }
}

void MPMeshRetain(MPMesh *mesh)
{
    if (mesh == NULL) return;
    
    ++(((MPMeshPrivate *)mesh->_reserved)->refCount);
}

void MPMeshRelease(MPMesh *mesh)
{
    if (mesh == NULL) return;
    
    int refCount = ((MPMeshPrivate *)mesh->_reserved)->refCount;
    
    if (!refCount)
    {
        printf("warning: releasing unretained mesh!\n");
    }
    
    if (refCount < 2)
    {
        MPMeshFree(mesh);
    }
    else
    {
        --(((MPMeshPrivate *)mesh->_reserved)->refCount);
    }
}

int MPMeshGetRefCount(const MPMesh *mesh)
{
    return ((MPMeshPrivate *)mesh->_reserved)->refCount;
}

size_t MPMeshGetTriangleCount(const MPMesh *mesh)
{
    return mesh->numIndices / 3;
}

void MPMeshGetTriangle(const MPMesh *mesh, size_t n, MPVec3 *triangle)
{
    if (triangle == NULL) return;
    
    const void *index = (char *)mesh->indexData + (3 * n * mesh->indexSize);
    
    switch (mesh->indexSize)
    {
        case sizeof(char):
            triangle[0] = *(MPVec3 *)((char *)mesh->vertexData + ((char *)index)[0] * mesh->stride);
            triangle[1] = *(MPVec3 *)((char *)mesh->vertexData + ((char *)index)[1] * mesh->stride);
            triangle[2] = *(MPVec3 *)((char *)mesh->vertexData + ((char *)index)[2] * mesh->stride);
            break;
            
        case sizeof(short):
            triangle[0] = *(MPVec3 *)((char *)mesh->vertexData + ((short *)index)[0] * mesh->stride);
            triangle[1] = *(MPVec3 *)((char *)mesh->vertexData + ((short *)index)[1] * mesh->stride);
            triangle[2] = *(MPVec3 *)((char *)mesh->vertexData + ((short *)index)[2] * mesh->stride);
            break;
            
        case sizeof(int):
            triangle[0] = *(MPVec3 *)((char *)mesh->vertexData + ((int *)index)[0] * mesh->stride);
            triangle[1] = *(MPVec3 *)((char *)mesh->vertexData + ((int *)index)[1] * mesh->stride);
            triangle[2] = *(MPVec3 *)((char *)mesh->vertexData + ((int *)index)[2] * mesh->stride);
            break;
            
        default:
            break;
    }
}

const MPVec3* MPMeshGetExtremePoints(const MPMesh *mesh)
{
    return ((MPMeshPrivate *)mesh->_reserved)->extremePoints;
}

MPSphere MPMeshGetBoundingSphere(const MPMesh *mesh, const MPMat4 *transform)
{    
    MPSphere boundingSphere = ((MPMeshPrivate *)mesh->_reserved)->boundingSphere;
    
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

MPVec3* MPMeshGetVoxels(const MPMesh *mesh, MPVec3 scale, float voxelSize, int *n)
{
    MPVec3 *extremes = ((MPMeshPrivate *)mesh->_reserved)->extremePoints;
    
    // some arbitrary start size
    int arraySize = 10;
    int count = 0;
    
    MPVec3 *voxels = malloc(arraySize * sizeof(MPVec3));
    
    // temporary cube mesh to represent a voxel
    MPMesh *cubeMesh = MPMeshCreateCube();
    MPMat4 transform = MPMat4MakeScale(MPVec3Make(voxelSize, voxelSize, voxelSize));
    
    float startX = scale.x * extremes[0].x + 0.5f * voxelSize;
    float endX = scale.x * extremes[3].x;
    
    float startY = scale.y * extremes[1].y + 0.5f * voxelSize;
    float endY = scale.y * extremes[4].y;
    
    float startZ = scale.z * extremes[2].z + 0.5f * voxelSize;
    float endZ = scale.z * extremes[5].z;
    
    // compute triangles of the mesh
    size_t numTriangles = MPMeshGetTriangleCount(mesh);
    MPTriangle triangles[numTriangles];
    
    MPMat4 scaleT = MPMat4MakeScale(scale);
    
    int i;
    for (i = 0; i < numTriangles; ++i)
    {
        MPMeshGetTriangle(mesh, i, triangles[i].p);
        MPTriangleApplyTransform(&triangles[i], scaleT);
    }
    
    float x, y, z;
    for (x = startX; x < endX; x += voxelSize)
    {
        transform.m[12] = x;
        
        for (y = startY; y < endY; y += voxelSize)
        {
            transform.m[13] = y;
            
            for (z = startZ; z < endZ; z += voxelSize)
            {
                transform.m[14] = z;
                
                if (_MPMeshVoxelCollision(triangles, numTriangles, cubeMesh, transform))
                {
                    if (count >= arraySize)
                    {
                        // double array if necessary
                        voxels = realloc(voxels, 2 * arraySize * sizeof(MPVec3));
                        arraySize *= 2;
                    }
                    
                    voxels[count] = MPVec3Make(x, y, z);
                    ++count;
                }
            }
        }
    }
    
    MPMeshFree(cubeMesh);
    
    // resize array to fit
    if (count > 0)
    {
        voxels = realloc(voxels, count * sizeof(MPVec3));
    }
    else
    {
        voxels = NULL;
    }
    
    if (n != NULL)
    {
        *n = count;
    }
    
    return voxels;
}

#pragma mark - private functions

void _MPMeshComputePrivate(MPMesh *mesh)
{
    MPVec3 *left, *right, *bottom, *top, *back, *front;
    left = right = bottom = top = back = front = (MPVec3 *)mesh->vertexData;
    
    int i;
    for (i = 0; i < mesh->numVertices; ++i)
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
    
    MPMeshPrivate *private = (MPMeshPrivate *)mesh->_reserved;
    
    private->extremePoints[0] = *left;
    private->extremePoints[1] = *bottom;
    private->extremePoints[2] = *back;
    private->extremePoints[3] = *right;
    private->extremePoints[4] = *top;
    private->extremePoints[5] = *front;
                              
    private->boundingSphere = MPSphereMake(center, maxDist);
}

// NOTE: this method is faster than full collision detection, but is not guaranteed to catch all cases
int _MPMeshVoxelCollision(const MPTriangle *faces, size_t n, const MPMesh *voxMesh, MPMat4 voxTransform)
{
    int numInside = 0;
    int numOutside = 0;
    
    size_t v, t;
    for (v = 0; v < voxMesh->numVertices && !(numOutside && numInside); ++v)
    {
        int inside = 1;
        int on = 0;
        
        for (t = 0; t < n && inside; ++t)
        {
            MPVec3 vertex = *(MPVec3 *)((char *)voxMesh->vertexData + v * voxMesh->stride);
            MPVec3ApplyTransform(&vertex, voxTransform);
            
            MPVec3 normal = MPTriangleNormal(faces[t]);
            
            float dot = MPVec3DotProduct(MPVec3Subtract(vertex, faces[t].v1), normal);
            
            if (dot >= 0.0f)
            {
                inside = 0;
                on = (dot == 0.0f);
            }
        }
        
        if (on || inside)
        {
            ++numInside;
        }
        
        if (on || !inside)
        {
            ++numOutside;
        }
    }
    
    return (numInside && numOutside);
}