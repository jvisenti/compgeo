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

  /* Remove the minimum-keyed element in the heap in O(lg n) time */
  HeapElement remove();

  /* Decrease the key of a given state (located by its heap index) in O(lg n) time */
  void decreaseKey(State *s, double k);
  
  /* Insert the state s into the heap with key k in O(lg n) time */
  void insertState(State *s, double k);

  /* Returns the number of elements in the heap */
  inline int size() const { return data_.size(); }

  void print();

private:
  inline int getParent(int i) const { return (i == 0 ? INVALID_INDEX : (i-1)/2); }

  inline int getLeftChild(int i) const { return ((2*i + 1) < size() ? (2*i + 1) : INVALID_INDEX); }

  inline int getRightChild(int i) const { return ((2*i + 2) < size() ? (2*i + 2) : INVALID_INDEX); }

  /* Restore the heap property in O(lg n) time by percolating down the element at index i */
  void heapify(int i);
  
  /* Clear all elements in the heap */
  void clear();

  std::vector<HeapElement> data_;

};

}

#endif
