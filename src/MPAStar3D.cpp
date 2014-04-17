//
//  MPAStar3D.cpp
//  MotionPlanner
//
//  Created by John Visentin on 4/14/14.
//  Copyright (c) 2014 Brockenhaus Studio. All rights reserved.
//

#include "MPAStar3D.h"

namespace MP
{

double distanceHeuristic(const Transform3D &start, const Transform3D &goal)
{
    return MPVec3EuclideanDistance(start.getPosition(), goal.getPosition());
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
    Transform3D plannerStart;
    MPVec3 pstart;
    pstart.x = std::floor(start.getPosition().x/eps);
    pstart.y = std::floor(start.getPosition().y/eps);
    pstart.z = std::floor(start.getPosition().z/eps);
    plannerStart.setPosition(pstart);
    
    Transform3D plannerGoal;
    MPVec3 pgoal;
    pgoal.x = std::floor(goal.getPosition().x/eps);
    pgoal.y = std::floor(goal.getPosition().y/eps);
    pgoal.z = std::floor(goal.getPosition().z/eps);
    plannerGoal.setPosition(pgoal);
    
    bool result = AStarPlanner<Transform3D>::plan(plannerStart, plannerGoal, plan);
    if(result)
    {
        for(auto it = plan.begin(); it != plan.end(); ++it)
        {
            plannerToWorld(*it);
        }
        std::cout << "AStar 3D planner succeeded with " << plan.size() << " states" << std::endl;
        return true;
    }
    else return false;
}

void AStar3D::plannerToWorld(Transform3D &T)
{
    float eps = static_cast<Environment3D *>(this->environment_)->getStepSize();
    MPVec3 pos = T.getPosition();
    pos.x *= eps;
    pos.y *= eps;
    pos.z *= eps;
    T.setPosition(pos);
}
    
}