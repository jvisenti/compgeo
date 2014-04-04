//
//  MPState.cpp
//
//  Created by John Visentin on 4/3/14.
//  Copyright (c) 2014 John Visentin. All rights reserved.
//

#include "MPState.h"

#pragma mark - public methods

MPState::MPState()
{
    this->init(MPVec3Zero, MPVec3Make(1.0f, 1.0f, 1.0f), MPQuaternionIdentity);
}

MPState::MPState(const MPVec3 &pos, const MPVec3 &scale, const MPQuaternion &rotation)
{
    this->matrixCache = nullptr;
    
    this->init(pos, scale, rotation);
}

MPState::~MPState()
{
    this->invalidateMatrixCache();
}

void MPState::setPosition(const MPVec3 &position)
{
    this->position = position;
    this->invalidateMatrixCache();
}

MPVec3& MPState::getPosition()
{
    return this->position;
}

void MPState::setScale(const MPVec3 &scale)
{
    this->scale = scale;
    this->invalidateMatrixCache();
}

MPVec3& MPState::getScale()
{
    return this->scale;
}

void MPState::setRotation(const MPQuaternion &rotation)
{
    this->rotation = rotation;
    this->invalidateMatrixCache();
}

MPQuaternion& MPState::getRotation()
{
    return this->rotation;
}

MPMat4& MPState::getTransform()
{
    if (this->matrixCache == nullptr)
    {
        MPMat4 s = MPMat4MakeScale(MPVec3Make(this->scale.x, this->scale.y, this->scale.z));
        MPMat4 r = MPMat4MakeRotation(this->rotation);
        
        MPMat4 mat = MPMat4Multiply(r, s);
        
        // shortcut to apply a translation transformation
        mat.m[12] += this->position.x;
        mat.m[13] += this->position.y;
        mat.m[14] += this->position.z;
        
        this->matrixCache = new MPMat4;
        *(this->matrixCache) = mat;
    }
    
    return *(this->matrixCache);
}

#pragma mark - private methods

void MPState::init(const MPVec3 &pos, const MPVec3 &scale, const MPQuaternion &rotation)
{
    this->position = pos;
    this->scale = scale;
    this->rotation = rotation;
}

void MPState::invalidateMatrixCache()
{
    if (this->matrixCache != nullptr)
    {
        delete this->matrixCache;
        this->matrixCache = nullptr;
    }
}