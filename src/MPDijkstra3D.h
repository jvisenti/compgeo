//
//  MPDijkstra3D.h
//
//  Created by Ellis Ratner on 4/11/14.
//  Copyright (c) 2014 Ellis Ratner. All rights reserved.
//

#ifndef _MPDijkstra3D_h
#define _MPDijkstra3D_h

#include "MPDijkstraPlanner.h"
#include "MPEnvironment3D.h"

namespace MP
{

class Dijkstra3D : public DijkstraPlanner<Transform3D>
{
public:
  Dijkstra3D(Environment3D *environment);

  virtual ~Dijkstra3D();

  bool plan(Transform3D start, Transform3D goal, std::vector<Transform3D> &plan);

  void plannerToWorld(Transform3D &T);

};

}

#endif 
