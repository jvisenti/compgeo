//
//  MPModel.h
//
//  Created by John Visentin on 4/3/14.
//  Copyright (c) 2014 John Visentin. All rights reserved.
//
//  An 3D object prepresented by its mesh (vertices) and a state (pose)

#ifndef __MotionPlanner__MPModel__
#define __MotionPlanner__MPModel__

#include "MPMesh.h"
#include "MPState.h"

class MPModel
{
public:
    MPModel();
    MPModel(MPMesh *mesh);
    ~MPModel();
    
    void setMesh(MPMesh *mesh);
    MPMesh* getMesh() const;
    
    void setState(const MPState &state);
    MPState& getState();
    
    void setPosition(const MPVec3 &position);
    MPVec3 getPosition() const;
    
    void setScale(const MPVec3 &scale);
    MPVec3 getScale() const;
    
    void setRotation(const MPQuaternion &rotation);
    MPQuaternion getRotation() const;
    
    /* returns the model/TRS matrix for the current pose */
    MPMat4 getModelMatrix();
    
    /* returns true if the current state causes a collision with the given model */
    bool collidesWithModel(MPModel &model);
    
    /* returns true if moving to the given state would cause a collision with the given model */
    bool stateCollidesWithModel(MPState &state, MPModel &model);
        
private:
    MPMesh *mesh;
    
    MPState state;
};

#endif
