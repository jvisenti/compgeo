//
//  MPTransform3D.h
//
//  Created by John Visentin on 4/3/14.
//  Copyright (c) 2014 John Visentin. All rights reserved.
//
//  Defines a TRS transform of an object in 3D space

#ifndef __MPTransform3D__
#define __MPTransform3D__

#include "MPMath.h"

namespace MP
{
class Transform3D
{
public:
    Transform3D();
    Transform3D(const MPVec3 &pos, const MPVec3 &scale, const MPQuaternion &rotation);
    Transform3D(const Transform3D &other);
    ~Transform3D();
    
    void setPosition(const MPVec3 &position);
    MPVec3 getPosition() const;
    
    void setScale(const MPVec3 &scale);
    MPVec3 getScale() const;
    
    void setRotation(const MPQuaternion &rotation);
    MPQuaternion getRotation() const;
    
    /* returns the TRS matrix using the position, rotation, and scale properties */
    MPMat4 getMatrix();
    
protected:
    MPMat4 *matrixCache;
    
    MPVec3 position;
    MPVec3 scale;
    MPQuaternion rotation;
    
    void init(const MPVec3 &pos, const MPVec3 &scale, const MPQuaternion &rotation);
    void invalidateMatrixCache();
};
}

#endif