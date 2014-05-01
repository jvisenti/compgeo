//
//  MPPotentialFieldPlanner.h
//
//  Created by Ellis Ratner on 5/1/14.
//

#ifndef __MPPotentialFieldPlanner__
#define __MPPotentialFieldPlanner__

#include <iostream>
#include "MPModel.h"

namespace MP
{

class PotentialFieldController
{
public:
    PotentialFieldController(const std::vector<Model *> &obstacles, Model *activeObject);
    
    PotentialFieldController();
    
    void setGoal(const Transform3D &goal);
    
    void move();
    
private:
    /* Potential functions inspired by http://www.cs.cmu.edu/~motionplanning/lecture/Chap4-Potential-Field_howie.pdf */
    MPVec3 potentialGrad(const MPVec3 &p);
    MPVec3 attractivePotentialGrad(const MPVec3 &p);
    /* pObs is the position of the obstacle
     * p is the position of the moving object
     * P is the cutoff beyond which there is no repuslive effect */
    MPVec3 repulsivePotentialGrad(const MPVec3 &pObs, const MPVec3 &p, float P);
    
    std::vector<Model *> obstacles_;
    Model *activeObject_;
    Transform3D goal_;
    
};
    
}

#endif
