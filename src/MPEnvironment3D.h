//
//  MPEnvironment3D.h
//
//  Created by Ellis Ratner on 4/9/14.
//  Copyright (c) 2014 Ellis Ratner. All rights reserved.
//
//  This environment considers only (x, y, z) and no rotations. Furthermore, we assume
//  integer coordinates, which may be scaled by the actual step size determined by the
//  discretization of the space. For example, if the actual step size is 0.01 m, then
//  we assume that 0.01 maps to 1, 0.02 maps to 2, and so on.

#ifndef _MPEnvironment3D_h
#define _MPEnvironment3D_h

#include "MPEnvironment.h"
#include "MPModel.h"
#include "MPAction6D.h"
#include <cmath>

namespace MP
{
    
extern double distanceHeuristic(const Transform3D &start, const Transform3D &goal);
extern double manhattanHeuristic(const Transform3D &start, const Transform3D &goal);

extern bool operator==(const Transform3D &lhs, const Transform3D &rhs);

typedef SearchState<Transform3D> SearchState3D;

class Environment3D : public Environment<Transform3D>
{
public:
    Environment3D();
    Environment3D(const MPVec3 &size);
    Environment3D(const MPVec3 &origin, const MPVec3 &size);
    
    ~Environment3D();
    
    void getSuccessors(SearchState3D *s, std::vector<SearchState3D *> &successors, std::vector<double> &costs);
    
    bool getCost(SearchState3D *s, SearchState3D *t, double &cost);
    
    void setOrigin(const MPVec3 &origin);
    
    MPVec3 getOrigin() const { return origin_; }
    
    void setSize(const MPVec3 &size);
    
    MPVec3 getSize() const { return size_; }
    
    void setActiveObject(Model *activeObject);
    
    Model* getActiveObject() { return activeObject_; }
    
    void addObstacle(Model *obstacle) { obstacles_.push_back(obstacle); }
    
    const std::vector<Model *>& getObstacles() const { return obstacles_; }
    
    double getStepSize() const { return stepSize_; }
    
    void setStepSize(double s) { stepSize_ = s; }
    
    double getRotationStepSize() const { return rotationStepSize_; }
    
    void setRotationStepSize(double s);
    
    bool stateValid(const Transform3D &T);
    
    void plannerToWorld(Transform3D &state) const;
    Transform3D plannerToWorld(const Transform3D &state) const;
    
    void worldToPlanner(Transform3D &state) const;
    Transform3D worldToPlanner(const Transform3D &state) const;
    
    bool isValid(Transform3D &T) const;
    bool isValidForModel(Transform3D &T, Model *model) const;
    
    bool inBounds(Transform3D &T) const;
    bool inBoundsForModel(Transform3D &T, Model *model) const;
    
protected:
    
    void updateBoundingBox();
    
    void generateActionSet();
    
    void applyAction(const Action6D &action, Transform3D &stateTransform);
    
    void plannerToWorld(MPVec3 &vec) const;
    void plannerToWorld(MPQuaternion &q) const;
    
    void worldToPlanner(MPVec3 &vec) const;
    void worldToPlanner(MPQuaternion &q) const;
    
    MPVec3 origin_;
    MPVec3 size_;
    
    MPAABox boundingBox_;
    
    double stepSize_;
    double rotationStepSize_;
    int numRotations_;
    
    Model *activeObject_;
    std::vector<Model *> obstacles_;
    
    Action6D::ActionSet actionSet_;

};
    
}

#endif 
