//
//  MPModel.h
//
//  Created by John Visentin on 4/3/14.
//  Copyright (c) 2014 John Visentin. All rights reserved.
//

#ifndef __MotionPlanner__MPModel__
#define __MotionPlanner__MPModel__

#include "MPMesh.h"

class MPModel
{
public:
    MPModel(MPMesh *mesh=NULL);
    
    void setMesh(MPMesh *mesh);
    MPMesh* getMesh();
    
    void setPosition(const MPVec3 &position);
    MPVec3& getPosition();
    
    void setScale(const MPVec3 &scale);
    MPVec3& getScale();
    
    void setRotation(const MPQuaternion &rotation);
    MPQuaternion& getRotation();
    
    MPMat4& modelMatrix();
    
private:
    MPMesh *mesh;
    
    MPMat4 *modelMatrixCache;
    
    MPVec3 position;
    MPVec3 scale;
    MPQuaternion rotation;
    
    void invalidateMatrixCache();
};

#endif
