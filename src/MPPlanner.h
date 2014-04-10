//
//  MPPlanner.h
//
//  Created by Ellis Ratner on 4/6/14.
//  Copyright (c) 2014 Ellis Ratner. All rights reserved.
//

#ifndef _MPPlanner_h
#define _MPPlanner_h

#include "MPEnvironment.h"
#include <vector>

namespace MP
{

template <typename T>
class Planner
{
public:
  Planner(Environment<T> *environment) : environment_(environment)
  {
  }

  virtual ~Planner()
  {
  }

  virtual bool plan(T start, T goal, std::vector<T> &plan) = 0;

protected:
  Environment<T> *environment_;

};

}

#endif
