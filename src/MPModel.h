//
//  MPModel.h
//
//  Created by John Visentin on 4/3/14.
//  Copyright (c) 2014 John Visentin. All rights reserved.
//
//  An 3D object prepresented by its mesh (vertices) and a state (pose)

#ifndef __MPModel__
#define __MPModel__

#include "MPMesh.h"
#include "MPTransform3D.h"
#include "MPAction6D.h"
#include "MPMotion.h"

namespace MP
{
class Model
{
public:
    Model();
    Model(MPMesh *mesh);
    ~Model();
    
    void setMesh(MPMesh *mesh);
    MPMesh* getMesh() const;
    
    void setMotion(const Motion &motion);
    Motion* getMotion() const;
    
    void setTransform(const Transform3D &transform);
    Transform3D& getTransform();
    
    void setPosition(const MPVec3 &position);
    MPVec3 getPosition() const;
    
    void setScale(const MPVec3 &scale);
    MPVec3 getScale() const;
    
    void setRotation(const MPQuaternion &rotation);
    MPQuaternion getRotation() const;
    
    MPMat4 getModelMatrix();
    
    /* returns true if the current state causes a collision with the given model */
    bool collidesWithModel(Model &model);
    
    /* returns true if moving to the given transform would cause a collision with the given model */
    bool wouldCollideWithModel(Transform3D &transform, Model &model);
    
    void setActionSet(const Action6D::ActionSet &);
    Action6D::ActionSet getActionSet() const;
    
protected:
    MPMesh *mesh;
    
    Motion *motion;
    
    Transform3D transform;
    
    Action6D::ActionSet actionSet;
};
}

#endif
