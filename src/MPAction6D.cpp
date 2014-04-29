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
    
MPVec3 Action6D::getTranslation() const
{
    return translation_;
}

MPQuaternion Action6D::getRotation() const
{
    return rotation_;
}

Action6D::ActionSet Action6D::generate3DActions(float translationStep)
{
    ActionSet actions;
    
    for(int i = -1; i <= 1; ++i)
    {
        for(int j = -1; j <= 1; ++j)
        {
            for(int k = -1; k <= 1; ++k)
            {
                if(i == 0 && j == 0 && k == 0) continue;
                
                MPVec3 translation = MPVec3Make(i * translationStep, j * translationStep, k * translationStep);
                MPQuaternion rotation = MPQuaternionIdentity;
                
                double cost = std::abs(i) + std::abs(j) + std::abs(k);
                
                Action6D action(cost, translation, rotation);
                actions.push_back(action);
            }
        }
    }
    
    return actions;
}
    
Action6D::ActionSet Action6D::generate6DActions(float translationStep, float rotationStep)
{
    ActionSet actions;
    
    for(int i = -1; i <= 1; ++i)
    {
        for(int j = -1; j <= 1; ++j)
        {
            for(int k = -1; k <= 1; ++k)
            {
                if(i == 0 && j == 0 && k == 0) continue;
                
                for(int p = -1; p <= 1; ++p)
                {
                    for (int y = -1; y <= 1; ++y)
                    {
                        for (int r = -1; r <= 1; ++r)
                        {
                            MPVec3 translation = MPVec3Make(i * translationStep, j * translationStep, k * translationStep);
                            
                            MPQuaternion rotation = MPRPYToQuaternion(r * rotationStep, p * rotationStep, y * rotationStep);
                            
                            double cost = std::abs(i) + std::abs(j) + std::abs(k) + std::abs(p) + std::abs(y) + std::abs(r);
                            
                            Action6D action(cost, translation, rotation);
                            actions.push_back(action);
                        }
                    }
                }
            }
        }
    }
    
    return actions;
}
    
Action6D::ActionSet Action6D::generateFalconActions(float translationStep, float rotationStep)
{
    ActionSet actions;
    
    for(int i = -1; i <= -1; ++i)
    {
        if (i == 0) continue;
        
        for(int p = -1; p <= 1; ++p)
        {
            for (int y = -1; y <= 1; ++y)
            {
                for (int r = -1; r <= 1; ++r)
                {
                    MPVec3 translation = MPVec3Make(0.0f, 0.0f, i * translationStep);
                    
                    MPQuaternion rotation = MPRPYToQuaternion(r * rotationStep, p * rotationStep, y * rotationStep);
                    
                    double cost = std::abs(i) + std::abs(p) + std::abs(y) + std::abs(r);
                    
                    Action6D action(cost, translation, rotation);
                    actions.push_back(action);
                }
            }
        }
    }
    
    return actions;
}
    
}