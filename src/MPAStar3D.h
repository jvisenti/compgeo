//
//  MPAStar3D.h
//
//  Created by John Visentin on 4/14/14.
//  Copyright (c) 2014 John Visentin. All rights reserved.
//

#ifndef __MPAStar3D__
#define __MPAStar3D__

#include "MPAStarPlanner.h"
#include "MPEnvironment3D.h"

namespace MP
{
    
extern double distanceHeuristic(const Transform3D &start, const Transform3D &goal);
    
class AStar3D : public AStarPlanner<Transform3D>
{
public:
    AStar3D(Environment3D *environment, heuristicptr heuristic);
    
    virtual ~AStar3D();
    
    bool plan(Transform3D start, Transform3D goal, std::vector<Transform3D> &plan);
    
    void plannerToWorld(Transform3D &T);
    
};
    
}

#endif
