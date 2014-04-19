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
    MPQuaternion sRot = start.getRotation();
    MPQuaternion tRot = goal.getRotation();
    
    MPVec3 difference = MPVec3Subtract(start.getPosition(), goal.getPosition());
    
    float pitchDiff = sRot.x - tRot.x;
    float yawDiff = sRot.y - tRot.y;
    float rollDiff = sRot.z - tRot.z;
    
    return std::abs(difference.x) + std::abs(difference.y) + std::abs(difference.z) + std::abs(pitchDiff) + std::abs(yawDiff) + std::abs(rollDiff);
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
    return AStarPlanner<Transform3D>::plan(start, goal, plan);
}

}