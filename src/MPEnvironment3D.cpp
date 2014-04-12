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
  if(states_.get(s->getValue()) == nullptr)
    states_.insert(s);

  int xs = (int)s->getValue().getPosition().x;
  int ys = (int)s->getValue().getPosition().y;
  int zs = (int)s->getValue().getPosition().z;

  for(int i = -1; i <= 1; ++i)
  {
    for(int j = -1; j <= 1; ++j)
    {
      for(int k = -1; k <= 1; ++k)
      {
	if(i == 0 && j == 0 & k == 0) continue;

	if(inBounds(xs+i, ys+j, zs+k))
	{
	  Transform3D T;
	  MPVec3 position;
	  position.x = (float)(xs+i);
	  position.y = (float)(ys+j);
	  position.z = (float)(zs+k);
	  T.setPosition(position);
	  // Check if active object collides with any obstacle at this state
	  if(!isValid(T))
	  {
	    std::cout << "(" << (xs+i)*stepSize_ << ", " << (ys+j)*stepSize_
		      << ", " << (zs+k)*stepSize_ << ") is an invalid state"
		      << std::endl;
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

bool Environment3D::getCost(SearchState3D *s, SearchState3D *t, double &cost)
{
  // If the distance between the two states is > stepSize_, then there is no edge
  // (i.e. primitive motion) between them
  double distance = std::sqrt(s->getValue().getPosition().x*s->getValue().getPosition().x + 
			      s->getValue().getPosition().y*s->getValue().getPosition().y +
			      s->getValue().getPosition().z*s->getValue().getPosition().z);
  if(distance <= 1.0f)
  {
    // @todo variable costs?
    cost = 1.0f;
    return true;
  }
  std::cout << "Error: no edge between (" << s->getValue().getPosition().x*stepSize_ <<
    ", " << s->getValue().getPosition().y*stepSize_ << ", " << 
    s->getValue().getPosition().z*stepSize_ << ")" << std::endl;
  return false;
}

bool Environment3D::inBounds(int x, int y, int z)
{
  float worldX = (float)x * stepSize_;
  float worldY = (float)y * stepSize_;
  float worldZ = (float)z * stepSize_;

  if(worldX >= origin_.x - size_.w/2.0f && worldX <= origin_.x + size_.w/2.0f &&
     worldY >= origin_.y - size_.h/2.0f && worldY <= origin_.y + size_.h/2.0f &&
     worldZ >= origin_.z - size_.d/2.0f && worldZ <= origin_.z + size_.d/2.0f)
  {
    return true;
  }

  return false;
}

bool Environment3D::isValid(Transform3D &T)
{
  bool valid = true;
  for(auto it = obstacles_.begin(); it != obstacles_.end(); ++it)
  {
    if(activeObject_->wouldCollideWithModel(T, **it))
    {
      valid = false;
      break;
    }
  }

  return valid;
}

}