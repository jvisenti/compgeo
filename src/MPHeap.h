//
//  MPHeap.h
//
//  Created by Ellis Ratner on 4/2/14.
//  Copyright (c) 2014 Ellis Ratner. All rights reserved.
//

#ifndef _MPHeap_h
#define _MPHeap_h

#include "MPSearchState.h"
#include <vector>
#include <iostream>

#define INVALID_INDEX -1

namespace MP
{

template <typename T>
struct HeapElement
{
  double key;
  SearchState<T> *state;
};

/*
 * A (min) heap data structure that stores states
 */
template <typename T>
class Heap
{
public:
  Heap()
  {
  }

  ~Heap()
  {
    clear();
  }

  /* Build a min heap from the given data */
  void buildHeap(const std::vector<HeapElement<T> > &data)
  {
    clear();
    for(int k = 0; k < (int)data.size(); ++k)
    {
      data[k].state->setHeapIndex(k);
      data_.push_back(data[k]);
    }

    for(int i = size()/2; i >= 0; --i)
    {
      heapify(i);
    }
  }

  /* Remove the minimum-keyed element in the heap in O(lg n) time */
  HeapElement<T> remove()
  {
    // Remove the first element in the heap and replace it with the last
    HeapElement<T> temp = data_[0];
    data_[0] = data_[size()-1];
    data_.pop_back();

    // Restore the min-heap property
    heapify(0);

    temp.state->setHeapIndex(INVALID_INDEX);
    return temp;
  }

  /* Decrease the key of a given state (located by its heap index) in O(lg n) time */
  void decreaseKey(SearchState<T> *s, double k)
  {
    // Get the index of the state in data_
    int i = s->getHeapIndex();

    // @todo should throw an exception?
    if(i == INVALID_INDEX)
    {
      std::cout << "Error in decreaseKey: Invalid heap index for state" << std::endl;
      return;
    }

    if(k > data_[i].key)
    {
      std::cout << "Error in decreaseKey: New key must be smaller than existing key" << std::endl;
      return;
    }

    data_[i].key = k;

    while(i > 0 && data_[getParent(i)].key > data_[i].key)
    {
      // Perform a swap of data_[i] with data_[getParent(i)]
      HeapElement<T> temp = data_[i];
      data_[i] = data_[getParent(i)];
      data_[i].state->setHeapIndex(i);
      data_[getParent(i)] = temp;
      data_[getParent(i)].state->setHeapIndex(getParent(i));

      i = getParent(i);
    }
  }
  
  /* Insert the state s into the heap with key k in O(lg n) time */
  void insertState(SearchState<T> *s, double k)
  {
    HeapElement<T> e;
    e.key = k;
    e.state = s;
    e.state->setHeapIndex((int)data_.size());
    data_.push_back(e);
    // Percolate up
    int i = size()-1;
    int parent = getParent(i);
    while(parent != INVALID_INDEX && data_[i].key < data_[parent].key)
    {
      // Perform a swap
      HeapElement<T> temp = data_[i];
      data_[i] = data_[parent];
      data_[i].state->setHeapIndex(i);
      data_[parent] = temp;
      data_[parent].state->setHeapIndex(parent);
      // Continue to percolate up the tree
      i = parent;
      parent = getParent(parent);
    }
  }

  /* Returns the number of elements in the heap */
  inline int size() const { return (int)data_.size(); }

  void print()
  {
    for(auto it = data_.begin(); it != data_.end(); ++it)
    {
      std::cout << "(index = " << it->state->getHeapIndex() << ", key = "
		<< it->key << ") ";
    }
    std::cout << std::endl;
  }

private:
  inline int getParent(int i) const { return (i == 0 ? INVALID_INDEX : (i-1)/2); }

  inline int getLeftChild(int i) const { return ((2*i + 1) < size() ? (2*i + 1) : INVALID_INDEX); }

  inline int getRightChild(int i) const { return ((2*i + 2) < size() ? (2*i + 2) : INVALID_INDEX); }

  /* Restore the heap property in O(lg n) time by percolating down the element at index i */
  void heapify(int i)
  {
    int l = getLeftChild(i);
    int r = getRightChild(i);
    int root;

    if(l != INVALID_INDEX && data_[l].key < data_[i].key)
    {
      root = l;
    }
    else root = i;

    if(r != INVALID_INDEX && data_[r].key < data_[root].key)
    {
      root = r;
    }

    if(root != i)
    {
      // Swap data_[i] with data_[root]
      HeapElement<T> temp = data_[i];
      data_[i] = data_[root];
      data_[i].state->setHeapIndex(i);
      data_[root] = temp;
      data_[root].state->setHeapIndex(root);

      heapify(root);
    }
  }
  
  /* Clear all elements in the heap */
  void clear()
  {
      for(auto it = data_.begin(); it != data_.end(); ++it)
      {
          it->state->setHeapIndex(INVALID_INDEX);
      }
    data_.clear();
  }

  std::vector<HeapElement<T> > data_;

};

}

#endif
