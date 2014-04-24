//
//  MPEnvironment3D.cpp
//
//  Created by Ellis Ratner on 4/9/14.
//  Copyright (c) 2014 Ellis Ratner. All rights reserved.
//

#include "MPEnvironment3D.h"
#include "MPTimer.h"
#include "MPUtils.h"

namespace MP
{

double distanceHeuristic(const Transform3D &start, const Transform3D &goal)
{
    return MPVec3EuclideanDistance(start.getPosition(), goal.getPosition());
}

double manhattanHeuristic(const Transform3D &start, const Transform3D &goal)
{
    MPQuaternion sRot = start.getRotation();
    MPQuaternion tRot = goal.getRotation();
    
    MPVec3 difference = MPVec3Subtract(start.getPosition(), goal.getPosition());
    
    float pitchDiff = sRot.x - tRot.x;
    float yawDiff = sRot.y - tRot.y;
    float rollDiff = sRot.z - tRot.z;
    
    return std::abs(difference.x) + std::abs(difference.y) + std::abs(difference.z) + std::abs(pitchDiff) + std::abs(yawDiff) + std::abs(rollDiff);
}

bool operator==(const Transform3D &lhs, const Transform3D &rhs)
{
    MPVec3 leftPos = lhs.getPosition();
    MPVec3 rightPos = rhs.getPosition();
    
    MPQuaternion leftRot = lhs.getRotation();
    MPQuaternion rightRot = rhs.getRotation();
    
    // Only consider the (x, y, z) projection of the transform
    // Also, assume that the coordinates are integer-valued
    return (((int)leftPos.x == (int)rightPos.x) &&
            ((int)leftPos.y == (int)rightPos.y) &&
            ((int)leftPos.z == (int)rightPos.z) &&
            // We store the pitch/yaw/roll as rotation xyz
            ((int)leftRot.x == (int)rightRot.x) &&
            ((int)leftRot.y == (int)rightRot.y) &&
            ((int)leftRot.z == (int)rightRot.z));
}

int transform3DHash(const Transform3D &t)
{
    const int p1 = 73856093;
    const int p2 = 19349663;
    const int p3 = 83492791;
    const int p4 = 3331333;
    const int p5 = 393919;
    const int p6 = 39916801;
    
    MPVec3 pos = t.getPosition();
    MPQuaternion rot = t.getRotation();
    
    // Assume that the coordinates are integer-valued
    int x = (int)pos.x;
    int y = (int)pos.y;
    int z = (int)pos.z;
    
    int pitch = (int)rot.x;
    int yaw = (int)rot.y;
    int roll = (int)rot.z;
    
    return ((x*p1) ^ (y*p2) ^ (z*p3) ^ (pitch*p4) ^ (yaw*p5) ^ (roll*p6));
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
    
void Environment3D::setOrigin(const MPVec3 &origin)
{
    this->origin_ = origin;
    this->updateBoundingBox();
}
    
void Environment3D::setSize(const MPVec3 &size)
{
    this->size_ = size;
    this->updateBoundingBox();
}

void Environment3D::getSuccessors(SearchState3D *s,
                                  std::vector<SearchState3D *> &successors,
                                  std::vector<double> &costs)
{
    Transform3D sT = s->getValue();
    
    if(states_.get(sT) == nullptr)
        states_.insert(s);
    
    plannerToWorld(sT);
    
//    Timer timer;
//    timer.start();
    
    for(auto action : actionSet_)
    {
        Transform3D neighborTransformWorld = action(sT);
        Transform3D neighborTransformPlanner = neighborTransformWorld;
        worldToPlanner(neighborTransformPlanner);
        
        SearchState3D *neighborState = states_.get(neighborTransformPlanner);
        if(neighborState == nullptr)
        {
            // Check if active object collides with any obstacle at this state
            if(!Environment<Transform3D>::stateValid(neighborTransformPlanner)) continue;
            
            if(!this->isValid(neighborTransformWorld))
            {
                SearchState3D *s = new SearchState3D();
                s->setValue(neighborTransformPlanner);
                invalidStates_.insert(s);
                continue;
            }
            
            // Has not seen this state yet
            neighborState = new SearchState3D();
            neighborState->setValue(neighborTransformPlanner);
            states_.insert(neighborState);
        }
        successors.push_back(neighborState);
        costs.push_back(action.getCost());
    }
    
//    std::cout << "Successor generation took "
//    << GET_ELAPSED_MICRO(timer) << " microseconds" << std::endl;
}

bool Environment3D::getCost(SearchState3D *s, SearchState3D *t, double &cost)
{
    // If the distance between the two states is > stepSize_, then there is no edge
    // (i.e. primitive motion) between them
    
    MPQuaternion sRot = s->getValue().getRotation();
    MPQuaternion tRot = t->getValue().getRotation();
    
    MPVec3 difference = MPVec3Subtract(s->getValue().getPosition(), t->getValue().getPosition());
    
    float pitchDiff = sRot.x - tRot.x;
    float yawDiff = sRot.y - tRot.y;
    float rollDiff = sRot.z - tRot.z;
    
    if(std::abs(difference.x) <= 1.0f && std::abs(difference.y) <= 1.0f && std::abs(difference.z) <= 1.0f)
    {
        cost = std::abs(difference.x) + std::abs(difference.y) + std::abs(difference.z) + (std::abs(pitchDiff) + std::abs(yawDiff) + std::abs(rollDiff));
        
        return true;
    }
    
    std::cout << "Error: no edge between (" << s->getValue().getPosition().x*stepSize_ <<
    ", " << s->getValue().getPosition().y*stepSize_ << ", " <<
    s->getValue().getPosition().z*stepSize_ << ") and (" << t->getValue().getPosition().x*stepSize_
    << ", " << t->getValue().getPosition().y*stepSize_ << ", " << t->getValue().getPosition().z*stepSize_ << ")" << std::endl;
    
    return false;
}

bool Environment3D::stateValid(const Transform3D &T)
{
    if(!Environment<Transform3D>::stateValid(T)) return false;
    
    Transform3D worldT = this->plannerToWorld(T);
    
    if(!this->isValid(worldT))
    {
        SearchState3D *s = new SearchState3D();
        s->setValue(T);
        invalidStates_.insert(s);
        return false;
    }
    
    return true;
}
    
void Environment3D::plannerToWorld(Transform3D &state) const
{
    state.setPosition(MPVec3MultiplyScalar(state.getPosition(), this->stepSize_));
    
    MPQuaternion plannerRotation = state.getRotation();

    state.setRotation(MPRPYToQuaternion(plannerRotation.x * this->rotationStepSize_, plannerRotation.y * this->rotationStepSize_, plannerRotation.z * this->rotationStepSize_));
}

Transform3D Environment3D::plannerToWorld(const Transform3D &state) const
{
    Transform3D worldState = state;
    
    plannerToWorld(worldState);
    
    return worldState;
}

void Environment3D::worldToPlanner(Transform3D &state) const
{
    MPVec3 pPos = state.getPosition();
    pPos.x = int(pPos.x / this->stepSize_);
    pPos.y = int(pPos.y / this->stepSize_);
    pPos.z = int(pPos.z / this->stepSize_);
    
    int numRotations = 2.0f * M_PI / this->getRotationStepSize();
    
    MPQuaternion pRot;
    float r, p, y;
    MPQuaternionToRPY(state.getRotation(), &r, &p, &y);
    pRot.x = (int(p / this->rotationStepSize_) + numRotations) % numRotations;
    pRot.y = (int(y / this->rotationStepSize_) + numRotations) % numRotations;
    pRot.z = (int(r / this->rotationStepSize_) + numRotations) % numRotations;
    pRot.w = 0.0f;
    
    state.setPosition(pPos);
    state.setRotation(pRot);
}

Transform3D Environment3D::worldToPlanner(const Transform3D &state) const
{
    Transform3D plannerState = state;
    
    worldToPlanner(plannerState);
    
    return plannerState;
}

bool Environment3D::isValid(Transform3D &T) const
{
    return this->isValidForModel(T, this->activeObject_);
}

bool Environment3D::isValidForModel(Transform3D &T, Model *model) const
{
//    Timer timer;
//    timer.start();
    
    bool valid = this->inBoundsForModel(T, model);
    
    for(auto it = obstacles_.begin(); valid && it != obstacles_.end(); ++it)
    {
        if(model->wouldCollideWithModel(T, **it))
        {
            valid = false;
        }
    }
    
//    std::cout << "Collision detection took "
//    << GET_ELAPSED_MICRO(timer) << " microseconds" << std::endl;
    
    return valid;
}
    
bool Environment3D::inBounds(Transform3D &T) const
{
    return this->inBoundsForModel(T, this->activeObject_);
}

bool Environment3D::inBoundsForModel(MP::Transform3D &T, MP::Model *model) const
{
    const MPVec3 *extremePoints = MPMeshGetExtremePoints(model->getMesh());
    
    bool inBounds = true;
    
    for (int i = 0; i < 6 && inBounds; ++i)
    {
        inBounds = MPAABoxContainsPoint(this->boundingBox_, T.transformVec3(extremePoints[i]));
    }
    
    return inBounds;
}
    
void Environment3D::generate6DActions()
{
//    MPQuaternion q = MPRPYToQuaternion(0.2f, 0.3f, 0.7f);
//    MPQuaternion q2 = MPQuaternionMultiply(MPQuaternionMultiply(MPQuaternionMakeWithAngleAndAxis(0.7f, 0.0f, 1.0f, 0.0f), MPQuaternionMakeWithAngleAndAxis(0.3f, 1.0f, 0.0f, 0.0f)), MPQuaternionMakeWithAngleAndAxis(0.2f, 0.0f, 0.0f, 1.0f));
//    float r, p, y;
//    MPQuaternionToRPY(q, &r, &p, &y);
//    std::cout << "(r, p, y) = (" << r << ", " << p << ", " << y << ")" << std::endl;
//    MPQuaternionToRPY(q2, &r, &p, &y);
//    std::cout << "(r, p, y) = (" << r << ", " << p << ", " << y << ")" << std::endl;
    
    actionSet_.clear();
    
    for(int i = -1; i <= 1; ++i)
    {
        for(int j = -1; j <= 1; ++j)
        {
            for(int k = -1; k <= 1; ++k)
            {
                if(i == 0 && j == 0 & k == 0) continue;
                
                for(int p = -1; p <= 1; ++p)
                {
                    for (int y = -1; y <= 1; ++y)
                    {
                        for (int r = -1; r <= 1; ++r)
                        {
                            MPVec3 translation = MPVec3Make(i*stepSize_, j*stepSize_, k*stepSize_);

                            MPQuaternion rotation = MPRPYToQuaternion(r * this->rotationStepSize_, p * this->rotationStepSize_, y * this->rotationStepSize_);
                            
                            double cost = std::abs(i) + std::abs(j) + std::abs(k) + std::abs(p) + std::abs(y) + std::abs(r);
                            
                            Action6D action(cost, translation, rotation);
                            actionSet_.push_back(action);
                        }
                    }
                }
            }
        }
    }
}
    
void Environment3D::generate3DActions()
{
    actionSet_.clear();
    
    for(int i = -1; i <= 1; ++i)
    {
        for(int j = -1; j <= 1; ++j)
        {
            for(int k = -1; k <= 1; ++k)
            {
                if(i == 0 && j == 0 & k == 0) continue;
                
                MPVec3 translation = MPVec3Make(i*stepSize_, j*stepSize_, k*stepSize_);
                MPQuaternion rotation = MPQuaternionIdentity;
                
                double cost = std::abs(i) + std::abs(j) + std::abs(k);
                
                Action6D action(cost, translation, rotation);
                actionSet_.push_back(action);
            }
        }
    }
}
    
#pragma mark - protected methods
    
void Environment3D::updateBoundingBox()
{
    MPVec3 halfSize = MPVec3MultiplyScalar(this->size_, 0.5f);
    
    MPVec3 min = MPVec3Subtract(this->origin_, halfSize);
    MPVec3 max = MPVec3Add(this->origin_, halfSize);
    
    this->boundingBox_ = MPAABoxMake(min, max);    
}
    
}
