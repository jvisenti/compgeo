//
//  MPAStarPlanner.h
//
//  Created by Ellis Ratner on 4/8/14.
//  Copyright (c) 2014 Ellis Ratner. All rights reserved.
//

#ifndef _MPAStarPlanner_h
#define _MPAStarPlanner_h

#include "MPPlanner.h"
#include "MPHeap.h"
#include "MPHashTable.h"
#include "MPTimer.h"
#include <algorithm>

namespace MP
{

template <typename T>
class AStarPlanner : public Planner<T>
{
public:
  typedef double (*heuristicptr)(const T&, const T&);

  AStarPlanner(Environment<T> *environment, heuristicptr heuristic)
    : Planner<T>(environment), heuristic_(heuristic), CLOSED_(environment->getHashFunction()), stateExpansions_(0)
  {
  }

  virtual ~AStarPlanner()
  {
  }

  bool plan(T start, T goal, std::vector<T> &plan)
  {
    SearchState<T> *s = this->environment_->addState(start);
    SearchState<T> *g = this->environment_->addState(goal);
    stateExpansions_ = 0;
    Timer timer;
    timer.start();
    bool success = aStarSearch(s, g);
    std::cout << "A* search terminated after "
	      << stateExpansions_ << " state expansions in " 
	      << GET_ELAPSED_MICRO(timer) << " microseconds" << std::endl;

    if(success)
    {
      // Re-construct the path from s to g by following pointers
      SearchState<T> *v = g;
      while(v != nullptr && v != s)
      {
        plan.push_back(v->getValue());
        v = v->getParent();
      }
      std::reverse(plan.begin(), plan.end());
      return true;
    }

    return false;
  }

  bool aStarSearch(SearchState<T> *startState, SearchState<T> *goalState)
  {
    CLOSED_.clear();
    Heap<T> OPEN;

    //startState->setParent(startState); ??
    startState->setPathCost(0.0f);
    OPEN.insertState(startState, heuristic_(startState->getValue(), goalState->getValue()));

    while(OPEN.size() > 0)
    {
      SearchState<T> *s = OPEN.remove().state;
      // Check if s is the goal state
      if(s->getValue() == goalState->getValue())
      {
	    std::cout << "A* search found goal state" << std::endl;
	    return true;
      }
      CLOSED_.insert(s);
      std::vector<SearchState<T> *> neighbors;
      std::vector<double> costs;
      stateExpansions_++;
      // If s is not the goal state, expand it
      this->environment_->getSuccessors(s, neighbors, costs);
      // Iterate over each neigbor s' of s
      for(auto it = neighbors.begin(); it != neighbors.end(); ++it)
      {
	// If s' not in CLOSED
	if(CLOSED_.get((*it)->getValue()) == nullptr)
	{
	  // If s' not already in OPEN
	  if((*it)->getHeapIndex() == INVALID_INDEX)
	  {
	    (*it)->setPathCost(INFINITE_COST);
	    (*it)->setParent(nullptr);
	  }
	  update(s, (*it));
	  if((*it)->getHeapIndex() == INVALID_INDEX)
	  {
	    OPEN.insertState(*it, (*it)->getPathCost() + 
			     heuristic_((*it)->getValue(), goalState->getValue()));
	  }
	  else
	  {
	    OPEN.decreaseKey(*it, (*it)->getPathCost() + 
			     heuristic_((*it)->getValue(), goalState->getValue()));
	  }
	}
      }
    }
    
    return false;
  }

protected:
  void update(SearchState<T> *s, SearchState<T> *sp)
  {
    double c;
    if(!this->environment_->getCost(s, sp, c))
      c = INFINITE_COST;

    if(s->getPathCost() + c < sp->getPathCost())
    {
      sp->setPathCost(s->getPathCost() + c);
      sp->setParent(s);
    }
  }

  heuristicptr heuristic_;

  HashTable<T> CLOSED_;

  int stateExpansions_;

};

}

#endif 
