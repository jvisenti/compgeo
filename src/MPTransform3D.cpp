//
//  MPTransform3D.cpp
//
//  Created by John Visentin on 4/3/14.
//  Copyright (c) 2014 John Visentin. All rights reserved.
//

#include "MPTransform3D.h"

namespace MP
{
#pragma mark - public methods
    
Transform3D::Transform3D()
{
    this->init(MPVec3Zero, MPVec3Make(1.0f, 1.0f, 1.0f), MPQuaternionIdentity);
}

Transform3D::Transform3D(const MPVec3 &pos, const MPVec3 &scale, const MPQuaternion &rotation)
{
    this->init(pos, scale, rotation);
}

Transform3D::Transform3D(const Transform3D &other)
{
    this->position = other.getPosition();
    this->rotation = other.getRotation();
    this->scale = other.getScale();
    
    this->matrixCache = nullptr;
}

Transform3D::~Transform3D()
{
    this->invalidateMatrixCache();
}

void Transform3D::setPosition(const MPVec3 &position)
{
    this->position = position;
    this->invalidateMatrixCache();
}

MPVec3 Transform3D::getPosition() const
{
    return this->position;
}

void Transform3D::setScale(const MPVec3 &scale)
{
    this->scale = scale;
    this->invalidateMatrixCache();
}

MPVec3 Transform3D::getScale() const
{
    return this->scale;
}

void Transform3D::setRotation(const MPQuaternion &rotation)
{
    this->rotation = rotation;
    this->invalidateMatrixCache();
}

MPQuaternion Transform3D::getRotation() const
{
    return this->rotation;
}

MPMat4 Transform3D::getMatrix()
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
    
void Transform3D::transformVec3(MPVec3 &vec)
{
    vec = MPMat4TransformVec3(this->getMatrix(), vec);
}

MPVec3 Transform3D::transformVec3(const MPVec3 &vec)
{
    MPVec3 tVec = vec;
    
    transformVec3(tVec);
    
    return tVec;
}

#pragma mark - private methods

void Transform3D::init(const MPVec3 &pos, const MPVec3 &scale, const MPQuaternion &rotation)
{
    this->matrixCache = nullptr;
    
    this->position = pos;
    this->scale = scale;
    this->rotation = rotation;
}

void Transform3D::invalidateMatrixCache()
{
    if (this->matrixCache != nullptr)
    {
        delete this->matrixCache;
        this->matrixCache = nullptr;
    }
}
}
