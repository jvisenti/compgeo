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
#include "MPHashTable.h"
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

inline int point2DHash(Point2D p)
{
  const int p1 = 73856093;
  const int p2 = 19349663;
  return ((p.x_*p1) ^ (p.y_*p2));
}

class Environment2D : public Environment<Point2D>
{
public:
  Environment2D(int xMin, int xMax, int yMin, int yMax);

  ~Environment2D();

  void getSuccessors(SearchState2D *s,
		     std::vector<SearchState2D *> &successors,
		     std::vector<double> &costs);

  inline int getNumStates() const { return states_.size(); }

private:
  int xMin_, xMax_;
  int yMin_, yMax_;

  HashTable<Point2D> states_;

};

}

#endif
