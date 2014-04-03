//
//  MPState.h
//
//  Created by Ellis Ratner on 4/2/14.
//  Copyright (c) 2014 Ellis Ratner. All rights reserved.
//

#ifndef MotionPlanner_MPState_h
#define MotionPlanner_MPState_h

namespace MP
{

/*
 * An abstract state for use in graph search problems
 */
class State
{
public:
  State() : heapIndex_(-1) { }

  virtual ~State() { }

  inline int getHeapIndex() const { return heapIndex_; }

  inline void setHeapIndex(int h) { heapIndex_ = h; }

  virtual int hash() = 0;

protected:
  int heapIndex_;
  
};

}

#endif
