//
//  MPModel.cpp
//
//  Created by John Visentin on 4/3/14.
//  Copyright (c) 2014 John Visentin. All rights reserved.
//

#include "MPModel.h"

#pragma mark - public methods

MPModel::MPModel()
{
    this->mesh = nullptr;
}

MPModel::MPModel(MPMesh *mesh)
{
    this->mesh = nullptr;
    this->setMesh(mesh);
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
            // no longer using the mesh, so decrease reference count
            MPMeshRelease(this->mesh);
        }
        
        // increase the reference count of the mesh
        MPMeshRetain(mesh);
        this->mesh = mesh;
    }
}

MPMesh* MPModel::getMesh() const
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

MPVec3 MPModel::getPosition() const
{
    return this->state.getPosition();
}

void MPModel::setScale(const MPVec3 &scale)
{
    this->state.setScale(scale);
}

MPVec3 MPModel::getScale() const
{
    return this->state.getScale();
}

void MPModel::setRotation(const MPQuaternion &rotation)
{
    this->state.setRotation(rotation);
}

MPQuaternion MPModel::getRotation() const
{
    return this->state.getRotation();
}

MPMat4 MPModel::getModelMatrix()
{
    return state.getTransform();
}

bool MPModel::collidesWithModel(MPModel &model)
{
    return this->stateCollidesWithModel(this->state, model);
}

bool MPModel::stateCollidesWithModel(MPState &state, MPModel &model)
{
    MPMat4 modelMatrix = state.getTransform();
    MPMat4 otherModelMatrix = model.getModelMatrix();
    
    MPSphere boundingSphere = MPMeshGetBoundingSphere(this->mesh, &modelMatrix);
    MPSphere otherBoundingSphere = MPMeshGetBoundingSphere(model.getMesh(), &otherModelMatrix);
    
    // bounding spheres don't intersect, so neither can the models
    if (!MPSphereIntersectsSphere(boundingSphere, otherBoundingSphere))
    {
        return false;
    }
    
    // TODO: precise collision test brute force
    
    return true;
}