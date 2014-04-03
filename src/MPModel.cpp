//
//  MPModel.cpp
//  MotionPlanner
//
//  Created by John Visentin on 4/3/14.
//  Copyright (c) 2014 John Visentin. All rights reserved.
//

#include "MPModel.h"

#pragma mark - public methods

MPModel::MPModel(MPMesh *mesh)
{
    this->setMesh(mesh);
    
    this->modelMatrixCache = nullptr;
    
    this->setPosition(MPVec3Zero);
    this->setScale(MPVec3Make(1.0f, 1.0f, 1.0f));
    this->setRotation(MPQuaternionIdentity);
}

void MPModel::setMesh(MPMesh *mesh)
{
    if (mesh != this->mesh)
    {
        if (this->mesh != nullptr)
        {
            MPMeshRelease(this->mesh);
        }
        
        MPMeshRetain(mesh);
        this->mesh = mesh;
    }
}

MPMesh* MPModel::getMesh()
{
    return this->mesh;
}

void MPModel::setPosition(const MPVec3 &position)
{
    this->position = position;
    this->invalidateMatrixCache();
}

MPVec3& MPModel::getPosition()
{
    return this->position;
}

void MPModel::setScale(const MPVec3 &scale)
{
    this->scale = scale;
    this->invalidateMatrixCache();
}

MPVec3& MPModel::getScale()
{
    return this->scale;
}

void MPModel::setRotation(const MPQuaternion &rotation)
{
    this->rotation = rotation;
    this->invalidateMatrixCache();
}

MPQuaternion& MPModel::getRotation()
{
    return this->rotation;
}

MPMat4& MPModel::modelMatrix()
{
    if (this->modelMatrixCache == nullptr)
    {
        MPMat4 s = MPMat4MakeScale(MPVec3Make(this->scale.x, this->scale.y, this->scale.z));
        MPMat4 r = MPMat4MakeRotation(this->rotation);
        
        MPMat4 mat = MPMat4Multiply(r, s);
        
        // shortcut to apply a translation transformation
        mat.m[12] += this->position.x;
        mat.m[13] += this->position.y;
        mat.m[14] += this->position.z;
        
        this->modelMatrixCache = new MPMat4;
        *(this->modelMatrixCache) = mat;
    }
    
    return *(this->modelMatrixCache);
}

#pragma mark - private methods

void MPModel::invalidateMatrixCache()
{
    if (this->modelMatrixCache != nullptr)
    {
        delete this->modelMatrixCache;
        this->modelMatrixCache = NULL;
    }
}