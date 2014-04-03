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
}

void Heap::buildHeap(const std::vector<HeapElement> &data)
{
  data_ = data;
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
    // Exchange data_[i] with data_[root]
    HeapElement temp = data_[i];
    data_[i] = data_[root];
    data_[root] = temp;

    heapify(root);
  }
}
  
}
