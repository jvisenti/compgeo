//
//  MPEnvironment3D.cpp
//
//  Created by Ellis Ratner on 4/9/14.
//  Copyright (c) 2014 Ellis Ratner. All rights reserved.
//

#include "MPEnvironment3D.h"
#include "MPTimer.h"

namespace MP
{
    
bool operator==(const Transform3D &lhs, const Transform3D &rhs)
{
    // Only consider the (x, y, z) projection of the transform
    // Also, assume that the coordinates are integer-valued
    return (((int)lhs.getPosition().x == (int)rhs.getPosition().x) &&
            ((int)lhs.getPosition().y == (int)rhs.getPosition().y) &&
            ((int)lhs.getPosition().z == (int)rhs.getPosition().z) &&
            // We store the roll as rotation w
            ((int)lhs.getRotation().w == (int)rhs.getRotation().w));
}

int transform3DHash(Transform3D t)
{
    const int p1 = 73856093;
    const int p2 = 19349663;
    const int p3 = 83492791;
    const int p4 = 3331333;
    
    // Assume that the coordinates are integer-valued
    int x = (int)t.getPosition().x;
    int y = (int)t.getPosition().y;
    int z = (int)t.getPosition().z;
    int roll = (int)t.getRotation().w;
    return ((x*p1) ^ (y*p2) ^ (z*p3) ^ (roll*p4));
}

Environment3D::Environment3D()
: Environment<Transform3D>(transform3DHash), origin_(MPVec3Zero), size_(MPVec3Zero), activeObject_(nullptr)
{
}

Environment3D::Environment3D(const MPVec3 &size)
: Environment<Transform3D>(transform3DHash), origin_(MPVec3Zero), size_(size), activeObject_(nullptr)
{
}

Environment3D::Environment3D(const MPVec3 &origin, const MPVec3 &size)
: Environment<Transform3D>(transform3DHash), origin_(origin), size_(size), activeObject_(nullptr)
{
}

Environment3D::~Environment3D()
{
}

void Environment3D::getSuccessors(SearchState3D *s,
                                  std::vector<SearchState3D *> &successors,
                                  std::vector<double> &costs)
{
    if(states_.get(s->getValue()) == nullptr)
        states_.insert(s);
    
//    Timer timer;
//    timer.start();
    
    int xs = (int)s->getValue().getPosition().x;
    int ys = (int)s->getValue().getPosition().y;
    int zs = (int)s->getValue().getPosition().z;
    int roll = (int)s->getValue().getRotation().w;
    
    // TODO: What if rotationStepSize >= 2*M_PI?
    int numRotations = (int)(2*M_PI/rotationStepSize_);
    
    for(int i = -1; i <= 1; ++i)
    {
        for(int j = -1; j <= 1; ++j)
        {
            for(int k = -1; k <= 1; ++k)
            {
                if(i == 0 && j == 0 & k == 0) continue;
                
                for(int r = -1; r <= 1; ++r)
                {
                    if(inBounds(xs+i, ys+j, zs+k))
                    {
                        Transform3D T(MPVec3Make((float)(xs+i), (float)(ys+j), (float)(zs+k)), s->getValue().getScale(), MPQuaternionMake(0.0f, 0.0f, 0.0f, (roll + r + numRotations) % numRotations));
                        
                        // Check if active object collides with any obstacle at this state
                        if(!this->stateValid(T))
                        {
                            continue;
                        }
                        
                        SearchState3D *neighbor = states_.get(T);
                        if(neighbor == nullptr)
                        {
                            // Has not seen this state yet
                            neighbor = new SearchState3D();
                            neighbor->setValue(T);
                            neighbor->setParent(s);
                            states_.insert(neighbor);
                        }
                        else
                        {
                            // Already seen
                        }
                        successors.push_back(neighbor);
                        double cost;
                        getCost(s, neighbor, cost);
                        costs.push_back(cost);
                    }
                }
            }
        }
    }
    
//    std::cout << "Successor generation took "
//    << GET_ELAPSED_MICRO(timer) << " microseconds" << std::endl;
}

bool Environment3D::getCost(SearchState3D *s, SearchState3D *t, double &cost)
{
    // If the distance between the two states is > stepSize_, then there is no edge
    // (i.e. primitive motion) between them
    MPVec3 difference = MPVec3Subtract(s->getValue().getPosition(), t->getValue().getPosition());
    float rollDifference = s->getValue().getRotation().w - t->getValue().getRotation().w;
    
    if(std::abs(difference.x) <= 1.0f && std::abs(difference.y) <= 1.0f && std::abs(difference.z) <= 1.0f)
    {
//        cost = MPVec3EuclideanDistance(s->getValue().getPosition(), t->getValue().getPosition()) + (rollDifference > 0.0f ? 1.0f : 0.0f);
        
        cost = std::abs(difference.x) + std::abs(difference.y) + std::abs(difference.z) + std::abs(rollDifference);
        
        return true;
    }
    std::cout << "Error: no edge between (" << s->getValue().getPosition().x*stepSize_ <<
    ", " << s->getValue().getPosition().y*stepSize_ << ", " <<
    s->getValue().getPosition().z*stepSize_ << ") and (" << t->getValue().getPosition().x*stepSize_
    << ", " << t->getValue().getPosition().y*stepSize_ << ", " << t->getValue().getPosition().z*stepSize_ << ")" << std::endl;
    
    return false;
}

bool Environment3D::inBounds(int x, int y, int z)
{
    // TODO: Does this really check in bounds? (We are just checking the center)
    float worldX = (float)x * stepSize_;
    float worldY = (float)y * stepSize_;
    float worldZ = (float)z * stepSize_;
    
    return (worldX >= origin_.x - size_.w && worldX <= origin_.x + size_.w &&
            worldY >= origin_.y - size_.h && worldY <= origin_.y + size_.h &&
            worldZ >= origin_.z - size_.d && worldZ <= origin_.z + size_.d);
}

bool Environment3D::stateValid(const Transform3D &T) const
{
    Transform3D worldT = T;
    worldT.setPosition(MPVec3MultiplyScalar(T.getPosition(), stepSize_));
    worldT.setRotation(MPQuaternionMakeWithAngleAndAxis(T.getRotation().w * rotationStepSize_, 0.0f, 0.0f, 1.0f));
    
    return this->isValid(worldT);
}

bool Environment3D::isValid(Transform3D &T) const
{
    return this->isValidForModel(T, activeObject_);
}

bool Environment3D::isValidForModel(Transform3D &T, Model *model) const
{
//    Timer timer;
//    timer.start();
    
    bool valid = true;
    
    for(auto it = obstacles_.begin(); it != obstacles_.end(); ++it)
    {
        if(model->wouldCollideWithModel(T, **it))
        {
            valid = false;
            break;
        }
    }
    
//    std::cout << "Collision detection took "
//    << GET_ELAPSED_MICRO(timer) << " microseconds" << std::endl;
    
    return valid;
}
    
}
