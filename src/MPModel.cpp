//
//  MPModel.cpp
//
//  Created by John Visentin on 4/3/14.
//  Copyright (c) 2014 John Visentin. All rights reserved.
//

#include "MPModel.h"

namespace MP
{
#pragma mark - public methods

Model::Model()
{
    this->mesh = nullptr;
}

Model::Model(MPMesh *mesh)
{
    this->mesh = nullptr;
    this->setMesh(mesh);
}

Model::~Model()
{
    MPMeshRelease(this->mesh);
}

void Model::setMesh(MPMesh *mesh)
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

MPMesh* Model::getMesh() const
{
    return this->mesh;
}

void Model::setTransform(const Transform3D &transform)
{
    this->transform = transform;
}

Transform3D& Model::getTransform()
{
    return this->transform;
}

void Model::setPosition(const MPVec3 &position)
{
    this->transform.setPosition(position);
}

MPVec3 Model::getPosition() const
{
    return this->transform.getPosition();
}

void Model::setScale(const MPVec3 &scale)
{
    this->transform.setScale(scale);
}

MPVec3 Model::getScale() const
{
    return this->transform.getScale();
}

void Model::setRotation(const MPQuaternion &rotation)
{
    this->transform.setRotation(rotation);
}

MPQuaternion Model::getRotation() const
{
    return this->transform.getRotation();
}

MPMat4 Model::getModelMatrix()
{
    return transform.getMatrix();
}

bool Model::collidesWithModel(Model &model)
{
    return this->wouldCollideWithModel(this->transform, model);
}

bool Model::wouldCollideWithModel(Transform3D &transform, Model &model)
{
    MPMat4 modelMatrix = transform.getMatrix();
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
}