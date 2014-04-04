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
    MPModel(MPMesh *mesh=NULL);
    ~MPModel();
    
    void setMesh(MPMesh *mesh);
    MPMesh* getMesh();
    
    void setState(const MPState &state);
    MPState& getState();
    
    void setPosition(const MPVec3 &position);
    MPVec3& getPosition();
    
    void setScale(const MPVec3 &scale);
    MPVec3& getScale();
    
    void setRotation(const MPQuaternion &rotation);
    MPQuaternion& getRotation();
    
    MPMat4& getModelMatrix();
    
    bool collidesWithModel(const MPModel &model);
    bool stateCollidesWithModel(const MPState &state, const MPModel &model);
        
private:
    MPMesh *mesh;
    
    MPState state;
};

#endif
