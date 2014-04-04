//
//  MPModel.cpp
//
//  Created by John Visentin on 4/3/14.
//  Copyright (c) 2014 John Visentin. All rights reserved.
//

#include "MPModel.h"

#pragma mark - public methods

MPModel::MPModel(MPMesh *mesh)
{
    this->setMesh(mesh);
    
    MPState defaultState(MPVec3Zero, MPVec3Make(1.0f, 1.0f, 1.0f), MPQuaternionIdentity);
    state = defaultState;
}

MPModel::~MPModel()
{
    MPMeshRelease(this->mesh);
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

void MPModel::setState(const MPState &state)
{
    this->state = state;
}

MPState& MPModel::getState()
{
    return this->state;
}

void MPModel::setPosition(const MPVec3 &position)
{
    this->state.setPosition(position);
}

MPVec3& MPModel::getPosition()
{
    return this->state.getPosition();
}

void MPModel::setScale(const MPVec3 &scale)
{
    this->state.setScale(scale);
}

MPVec3& MPModel::getScale()
{
    return this->state.getScale();
}

void MPModel::setRotation(const MPQuaternion &rotation)
{
    this->state.setRotation(rotation);
}

MPQuaternion& MPModel::getRotation()
{
    return this->state.getRotation();
}

MPMat4& MPModel::getModelMatrix()
{
    return this->state.getTransform();
}

bool MPModel::collidesWithModel(const MPModel &model)
{
    return this->stateCollidesWithModel(this->state, model);
}

bool MPModel::stateCollidesWithModel(const MPState &state, const MPModel &model)
{
    // TODO: the collision test
    
    return false;
}