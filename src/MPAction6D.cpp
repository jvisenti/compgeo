//
//  MPAction6D.cpp
//  MotionPlanner
//
//  Created by Ellis M. Ratner on 4/21/14.
//

#include "MPAction6D.h"

namespace MP
{
    
Action6D::Action6D(double cost, const MPVec3 &translation, const MPQuaternion &rotation)
: Action<Transform3D>(cost), translation_(translation), rotation_(rotation)
{
}

Action6D::~Action6D()
{
}

Transform3D Action6D::operator()(const MP::Transform3D &transform)
{
    Transform3D t = transform;
    t.setPosition(MPVec3Add(translation_, transform.getPosition()));
    t.setRotation(MPQuaternionMultiply(rotation_, transform.getRotation()));
    return t;
}

}