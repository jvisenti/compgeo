//
//  MPState.h
//
//  Created by John Visentin on 4/3/14.
//  Copyright (c) 2014 John Visentin. All rights reserved.
//

#ifndef __MotionPlanner__MPState__
#define __MotionPlanner__MPState__

#import "MPMath.h"

class MPState
{
public:
    MPState();
    MPState(const MPVec3 &pos, const MPVec3 &scale, const MPQuaternion &rotation);
    ~MPState();
    
    void setPosition(const MPVec3 &position);
    MPVec3 getPosition() const;
    
    void setScale(const MPVec3 &scale);
    MPVec3 getScale() const;
    
    void setRotation(const MPQuaternion &rotation);
    MPQuaternion getRotation() const;
    
    MPMat4 getTransform();

private:
    MPMat4 *matrixCache;
    
    MPVec3 position;
    MPVec3 scale;
    MPQuaternion rotation;
    
    void init(const MPVec3 &pos, const MPVec3 &scale, const MPQuaternion &rotation);
    void invalidateMatrixCache();
};

#endif