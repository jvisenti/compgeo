//
//  MPEnvironment3D.cpp
//
//  Created by Ellis Ratner on 4/9/14.
//  Copyright (c) 2014 Ellis Ratner. All rights reserved.
//

#include "MPEnvironment3D.h"

namespace MP
{

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
  // @todo
}

bool Environment3D::getCost(SearchState3D *s, SearchState3D *t, double &cost)
{
  // @todo
  // If the distance between the two states is > 1, then there is no edge
  // (i.e. primitive motion) between them
  return false;
}

}
