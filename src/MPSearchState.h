//
//  MPSearchState.h
//
//  Created by Ellis Ratner on 4/2/14.
//  Copyright (c) 2014 Ellis Ratner. All rights reserved.
//

#ifndef _MPSearchState_h
#define _MPSearchState_h

#define INFINITE_COST 1000000.0f

namespace MP
{

/*
 * An abstract state for use in graph search problems
 */
template <typename T>
class SearchState
{
public:
  SearchState() : heapIndex_(-1), g_(INFINITE_COST), parent_(nullptr) { }

  virtual ~SearchState() { }

  inline int getHeapIndex() const { return heapIndex_; }

  inline void setHeapIndex(int h) { heapIndex_ = h; }

  inline T getValue() const { return value_; }

  inline void setValue(const T &value) { value_ = value; }

  inline double getPathCost() const { return g_; }

  inline void setPathCost(int g) { g_ = g; }

  inline SearchState<T> *getParent() { return parent_; }

  inline void setParent(SearchState<T> *p) { parent_ = p; }

protected:
  int heapIndex_;
  T value_;

  double g_;
  SearchState<T> *parent_;
  
};

}

#endif
