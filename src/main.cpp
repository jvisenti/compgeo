//
//  main.cpp
//
//  Created by John Visentin on 4/2/14.
//  Copyright (c) 2014 John Visentin. All rights reserved.
//

#include "MPState.h"
#include "MPHeap.h"
#include <iostream>
#include <cmath>
#include <cstdlib>

#define TEST_STATE(x) ((TestState *)x)

class TestState : public MP::State
{
public:
  TestState(int x, int y) : x_(x), y_(y) { }

  ~TestState() { }
  
  int hash() { return 1; }

  int x_;
  int y_;

};

int main(int argc, char** argv)
{
  // Initialize the data
  std::vector<MP::HeapElement> data;
  for(int i = 0; i < 10; ++i)
  {
    MP::HeapElement h;
    int k = rand() % 100 + 1;
    // Distance from the origin
    h.key = std::sqrt(2*k*k);
    h.state = new TestState(k, k);
    std::cout << "Adding state (" << TEST_STATE(h.state)->x_ << ", " 
	      << TEST_STATE(h.state)->y_ << ")" << std::endl;
    data.push_back(h);
  }

  // Make the heap
  MP::Heap H;
  H.buildHeap(data);

  // Print the states in order of their distance from the origin
  int N = H.size();
  for(int i = 0; i < N; ++i)
  {
    MP::HeapElement h = H.remove();
    std::cout << "(" << TEST_STATE(h.state)->x_ << ", " 
	      << TEST_STATE(h.state)->y_ << ")" << std::endl;
  }

  // Clean up
  for(std::vector<MP::HeapElement>::iterator it = data.begin();
      it != data.end(); 
      ++it)
  {
    if(it->state)
    {
      std::cout << "Deleting state (" << TEST_STATE(it->state)->x_ 
		<< ", " << TEST_STATE(it->state)->y_ << ")" << std::endl;
      delete it->state;
    }
  }

  return 0;
}

