//
//  MPEnvironment2D.h
//
//  Created by Ellis Ratner on 4/4/14.
//  Copyright (c) 2014 Ellis Ratner. All rights reserved.
//

#ifndef _MPEnvironment2D_h
#define _MPEnvironment2D_h

#include "MPSearchState.h"
#include "MPEnvironment.h"
#include <vector>

namespace MP
{

struct Point2D
{
  Point2D() : x_(0), y_(0) { }

  Point2D(int x, int y) : x_(x), y_(y) { }

  int x_;
  int y_;

  bool operator==(const Point2D &rhs)
  {
    return (x_ == rhs.x_ && y_ == rhs.y_);
  }
};

typedef SearchState<Point2D> SearchState2D;

class Environment2D : public Environment<Point2D>
{
public:
  Environment2D(int xMin, int xMax, int yMin, int yMax);

  ~Environment2D();

  void getSuccessors(SearchState2D *s,
		     std::vector<SearchState2D *> &successors,
		     std::vector<double> &costs);
  
private:
  int xMin_, xMax_;
  int yMin_, yMax_;

};

}

#endif