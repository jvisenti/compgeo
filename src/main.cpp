//
//  main.cpp
//
//  Created by John Visentin on 4/2/14.
//  Copyright (c) 2014 John Visentin. All rights reserved.
//

#include "MPHeap.h"
#include "MPEnvironment2D.h"
#include <iostream>
#include <cmath>
#include <cstdlib>

void test2D();

int main(int argc, char** argv)
{
  test2D();

  return 0;
}

void test2D()
{
  // Initialize the data
  std::vector<MP::HeapElement<MP::Point2D> > data;
  for(int i = 0; i < 10; ++i)
  {
    MP::HeapElement<MP::Point2D> h;
    int k = rand() % 100 + 1;
    //int k = i+1;
    // Distance from the origin
    h.key = std::sqrt(2*k*k);
    h.state = new MP::SearchState2D();
    h.state->setValue(MP::Point2D(k, k));
    std::cout << "Adding state (" << h.state->getValue().x_ << ", " 
	      << h.state->getValue().y_ << ")" << std::endl;
    data.push_back(h);
  }

  // Make the heap
  MP::Heap<MP::Point2D> H;
  H.buildHeap(data);

  H.print();

  H.decreaseKey(data[3].state, 0.5);

  H.print();

  // Print the states in order of their distance from the origin
  int N = H.size();
  for(int i = 0; i < N; ++i)
  {
    MP::HeapElement<MP::Point2D> h = H.remove();
    std::cout << h.state->getHeapIndex() << ": (" << h.state->getValue().x_ << ", " 
	      << h.state->getValue().y_ << ")" << std::endl;
  }

  // Make a [-10, 10] x [-20, 20] 2D grid world environment
  MP::Environment2D env(-10, 10, -20, 20);
  MP::SearchState2D *start = new MP::SearchState2D();
  start->setValue(MP::Point2D(0, 0));
  std::vector<MP::SearchState2D *> succ;
  std::vector<double> costs;
  env.getSuccessors(start, succ, costs);
  std::cout << "State (" << start->getValue().x_ << ", " << start->getValue().y_ << ") has "
	    << "the following neighbors: " << std::endl;
  for(int i = 0; i < succ.size(); ++i)
  {
    std::cout << "(" << succ[i]->getValue().x_ << ", " << succ[i]->getValue().y_
	      << ") with cost " << costs[i] << std::endl;
  }

  std::cout << env.getNumStates() << " different states stored so far" << std::endl;
  std::cout << std::endl;

  std::vector<MP::SearchState2D *> succ2;
  std::vector<double> costs2;
  env.getSuccessors(succ[3], succ2, costs2);
  std::cout << "State (" << succ[3]->getValue().x_ << ", " << succ[3]->getValue().y_ << ") has "
	    << "the following neighbors: " << std::endl;
  for(int i = 0; i < succ2.size(); ++i)
  {
    std::cout << "(" << succ2[i]->getValue().x_ << ", " << succ2[i]->getValue().y_
	      << ") with cost " << costs2[i] << std::endl;
  }
  std::cout << env.getNumStates() << " different states stored so far" << std::endl;
}

