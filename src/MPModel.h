//
//  MPModel.h
//
//  Created by John Visentin on 4/3/14.
//  Copyright (c) 2014 John Visentin. All rights reserved.
//

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
    
    MPMat4 getModelMatrix();
    
    bool collidesWithModel(MPModel &model);
    bool stateCollidesWithModel(MPState &state, MPModel &model);
        
private:
    MPMesh *mesh;
    
    MPState state;
};

#endif
