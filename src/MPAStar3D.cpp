//
//  MPAStar3D.cpp
//
//  Created by John Visentin on 4/14/14.
//  Copyright (c) 2014 John Visentin. All rights reserved.
//

#include "MPAStar3D.h"

namespace MP
{

double distanceHeuristic(const Transform3D &start, const Transform3D &goal)
{
    return MPVec3EuclideanDistance(start.getPosition(), goal.getPosition());
}
    
double manhattanHeuristic(const Transform3D &start, const Transform3D &goal)
{
    MPVec3 difference = MPVec3Subtract(start.getPosition(), goal.getPosition());
    float rollDifference = start.getRotation().w - goal.getRotation().w;
    
    return std::abs(difference.x) + std::abs(difference.y) + std::abs(difference.z) + std::abs(rollDifference);
}
    
AStar3D::AStar3D(Environment3D *environment, heuristicptr heuristic)
: AStarPlanner<Transform3D>(environment, heuristic)
{
}

AStar3D::~AStar3D()
{
}

bool AStar3D::plan(Transform3D start, Transform3D goal, std::vector<Transform3D>& plan)
{
    double eps = static_cast<Environment3D *>(this->environment_)->getStepSize();
    double rollEps = static_cast<Environment3D *>(this->environment_)->getRotationStepSize();
    Transform3D plannerStart;
    MPVec3 pstart;
    pstart.x = std::floor(start.getPosition().x/eps);
    pstart.y = std::floor(start.getPosition().y/eps);
    pstart.z = std::floor(start.getPosition().z/eps);
    plannerStart.setScale(start.getScale());
    plannerStart.setRotation(MPQuaternionMake(0.0f, 0.0f, 0.0f, 0.0f));
    plannerStart.setPosition(pstart);
    
    Transform3D plannerGoal;
    MPVec3 pgoal;
    pgoal.x = std::floor(goal.getPosition().x/eps);
    pgoal.y = std::floor(goal.getPosition().y/eps);
    pgoal.z = std::floor(goal.getPosition().z/eps);
    int roll = (int)(MPQuaternionRoll(goal.getRotation())/rollEps);
    plannerGoal.setRotation(MPQuaternionMake(0.0f, 0.0f, 0.0f, roll));
    plannerGoal.setScale(goal.getScale());
    plannerGoal.setPosition(pgoal);
    
    bool result = AStarPlanner<Transform3D>::plan(plannerStart, plannerGoal, plan);
    if(result)
    {
        for(auto it = plan.begin(); it != plan.end(); ++it)
        {
            plannerToWorld(*it);
        }
        
        // "snap to" exact goal state
        plan.push_back(goal);
        
        std::cout << "AStar 3D planner succeeded with " << plan.size() << " states" << std::endl;
        return true;
    }
    else return false;
}

void AStar3D::plannerToWorld(Transform3D &T)
{
    float eps = static_cast<Environment3D *>(this->environment_)->getStepSize();
    float rotationEps = static_cast<Environment3D *>(this->environment_)->getRotationStepSize();
    MPVec3 pos = T.getPosition();
    float roll = T.getRotation().w;
    pos.x *= eps;
    pos.y *= eps;
    pos.z *= eps;
    T.setPosition(pos);
    T.setRotation(MPQuaternionMakeWithAngleAndAxis(roll*rotationEps, 0.0f, 0.0f, 1.0f));
}
    
}