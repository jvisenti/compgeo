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

#include "MPSearchState.h"
#include "MPEnvironment.h"
#include "MPTransform3D.h"
#include "MPModel.h"
#include <cmath>

namespace MP
{

static bool operator==(const Transform3D &lhs, const Transform3D &rhs)
{
  // Only consider the (x, y, z) projection of the transform
  // Also, assume that the coordinates are integer-valued
  return (((int)lhs.getPosition().x == (int)rhs.getPosition().x) &&
	  ((int)lhs.getPosition().y == (int)rhs.getPosition().y) &&
	  ((int)lhs.getPosition().z == (int)rhs.getPosition().z));
}

static int transform3DHash(Transform3D t)
{
  const int p1 = 73856093;
  const int p2 = 19349663;
  const int p3 = 83492791;

  // Assume that the coordinates are integer-valued
  int x = (int)t.getPosition().x;
  int y = (int)t.getPosition().y;
  int z = (int)t.getPosition().z;
  return ((x*p1) ^ (x*p2) ^ (x*p3));
}

typedef SearchState<Transform3D> SearchState3D;

class Environment3D : public Environment<Transform3D>
{
public:
  Environment3D(const MPVec3 &origin, const MPVec3 &size);

  ~Environment3D();

  void getSuccessors(SearchState3D *s, std::vector<SearchState3D *> &successors, std::vector<double> &costs);

  bool getCost(SearchState3D *s, SearchState3D *t, double &cost);

  void setOrigin(const MPVec3 &origin) { origin_ = origin; }

  MPVec3 getOrigin() const { return origin_; }

  void setSize(const MPVec3 &size) { size_ = size; }

  MPVec3 getSize() const { return size_; }

  double getStepSize() const { return stepSize_; }

  void setStepSize(double s) { stepSize_ = s; }

private:
  bool inBounds(int x, int y, int z);

  bool isValid(Transform3D &T);

  MPVec3 origin_;
  MPVec3 size_;

  double stepSize_;

  Model *activeObject_;
  std::vector<Model *> obstacles_;

};

}

#endif 
