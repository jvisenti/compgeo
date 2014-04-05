//
//  MPSearchState.h
//
//  Created by Ellis Ratner on 4/2/14.
//  Copyright (c) 2014 Ellis Ratner. All rights reserved.
//

#ifndef _MPSearchState_h
#define _MPSearchState_h

namespace MP
{

/*
 * An abstract state for use in graph search problems
 */
template <typename T>
class SearchState
{
public:
  SearchState() : heapIndex_(-1) { }

  virtual ~SearchState() { }

  inline int getHeapIndex() const { return heapIndex_; }

  inline void setHeapIndex(int h) { heapIndex_ = h; }

  inline T getValue() const { return value_; }

  inline void setValue(T value) { value_ = value; }

protected:
  int heapIndex_;
  T value_;
  
};

}

#endif
