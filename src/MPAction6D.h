//
//  MPAction6D.h
//  MotionPlanner
//
//  Created by Ellis M. Ratner on 4/21/14.
//

#ifndef __MPAction6D__
#define __MPAction6D__

#include <vector>
#include "MPAction.h"
#include "MPTransform3D.h"

namespace MP
{
    
class Action6D : public Action<Transform3D>
{
public:
    typedef std::vector<Action6D> ActionSet;

    Action6D(double cost, const MPVec3 &translation, const MPQuaternion &rotation);
    
    virtual ~Action6D();
    
    Transform3D operator()(const Transform3D& transform);
    
    MPVec3 getTranslation() const;
    
    MPQuaternion getRotation() const;
    
    static ActionSet generate3DActions(float translationStep);
    
    static ActionSet generate6DActions(float translationStep, float rotationStep);
    
private:
    MPVec3 translation_;
    MPQuaternion rotation_;
    
};

}

#endif
