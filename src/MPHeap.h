//
//  MPHeap.h
//
//  Created by Ellis Ratner on 4/2/14.
//  Copyright (c) 2014 Ellis Ratner. All rights reserved.
//

#ifndef MotionPlanner_MPHeap_h
#define MotionPlanner_MPHeap_h

#include "MPState.h"
#include <vector>

#define INVALID_INDEX -1

namespace MP
{

struct HeapElement
{
  double key;
  State *state;
};

/*
 * A (min) heap data structure that stores states
 */
class Heap
{
public:
  Heap();

  ~Heap();

  /* Build a min heap from the given data */
  void buildHeap(const std::vector<HeapElement> &);

  /* Remove the minimum-keyed element in the heap in O(lg n) */
  HeapElement remove();

  /* Returns the number of elements in the heap */
  inline int size() const { return data_.size(); }

private:
  inline int getParent(int i) const { return (i == 0 ? INVALID_INDEX : (i-1)/2); }

  inline int getLeftChild(int i) const { return ((2*i + 1) < size() ? (2*i + 1) : INVALID_INDEX); }

  inline int getRightChild(int i) const { return ((2*i + 2) < size() ? (2*i + 2) : INVALID_INDEX); }

  void heapify(int i);

  std::vector<HeapElement> data_;

};

}

#endif
