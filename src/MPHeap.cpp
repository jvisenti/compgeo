//
//  MPHeap.cpp
//
//  Created by Ellis Ratner on 4/2/14.
//  Copyright (c) 2014 Ellis Ratner. All rights reserved.
//

#include "MPHeap.h"
#include <iostream>

namespace MP
{

Heap::Heap()
{
}

Heap::~Heap()
{
  clear();
}

void Heap::buildHeap(const std::vector<HeapElement> &data)
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

HeapElement Heap::remove()
{
  // Remove the first element in the heap and replace it with the last
  HeapElement temp = data_[0];
  data_[0] = data_[size()-1];
  data_.pop_back();

  // Restore the min-heap property
  heapify(0);

  return temp;
}

void Heap::decreaseKey(State *s, double k)
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
    HeapElement temp = data_[i];
    data_[i] = data_[getParent(i)];
    data_[i].state->setHeapIndex(i);
    data_[getParent(i)] = temp;
    data_[getParent(i)].state->setHeapIndex(getParent(i));

    i = getParent(i);
  }
}

void Heap::insertState(State *s, double k)
{
  HeapElement e;
  e.key = k;
  e.state = s;
  e.state->setHeapIndex(data_.size());
  data_.push_back(e);
  // Percolate up
  int i = size()-1;
  int parent = getParent(i);
  while(parent != INVALID_INDEX && data_[i].key < data_[parent].key)
  {
    // Perform a swap
    HeapElement temp = data_[i];
    data_[i] = data_[parent];
    data_[i].state->setHeapIndex(i);
    data_[parent] = temp;
    data_[parent].state->setHeapIndex(parent);
    // Continue to percolate up the tree
    i = parent;
    parent = getParent(parent);
  }
}

void Heap::print()
{
  std::vector<HeapElement>::iterator it;
  for(it = data_.begin(); it != data_.end(); ++it)
  {
    std::cout << "(index = " << it->state->getHeapIndex() << ", key = "
	      << it->key << ") ";
  }
  std::cout << std::endl;
}

void Heap::heapify(int i)
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
    HeapElement temp = data_[i];
    data_[i] = data_[root];
    data_[i].state->setHeapIndex(i);
    data_[root] = temp;
    data_[root].state->setHeapIndex(root);

    heapify(root);
  }
}

void Heap::clear()
{
  for(int i = 0; i < size(); ++i)
  {
    // Delete the allocated state @todo this could be problematic if others hold 
    // a pointer to this state
    delete data_[i].state;
    data_[i].state = 0;
  }
  data_.clear();
}
  
}
