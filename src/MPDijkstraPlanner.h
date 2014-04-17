//
//  MPDijkstraPlanner.h
//
//  Created by Ellis Ratner on 4/7/14.
//  Copyright (c) 2014 Ellis Ratner. All rights reserved.
//

#ifndef _MPDijkstraPlanner_h
#define _MPDijkstraPlanner_h

#include "MPPlanner.h"
#include "MPHeap.h"
#include "MPHashTable.h"
#include <algorithm>

namespace MP
{

template <typename T>
class DijkstraPlanner : public Planner<T>
{
public:
  DijkstraPlanner(Environment<T> *environment) 
    : Planner<T>(environment), CLOSED_(environment->getHashFunction()), stateExpansions_(0)
  {
  }

  virtual ~DijkstraPlanner()
  {
  }

  virtual bool plan(T start, T goal, std::vector<T> &plan)
  {
    if (!this->environment_->stateValid(start) || !this->environment_->stateValid(goal))
    {
        std::cout << "Dijkstra plan failed because start/goal states are invalid" << std::endl;
        return false;
    }
      
    SearchState<T> *s = this->environment_->addState(start);
    SearchState<T> *g = this->environment_->addState(goal);
    stateExpansions_ = 0;
    bool success = dijkstraSearch(s, g);
    std::cout << "Dijkstra's search terminated after "
	      << stateExpansions_ << " state expansions" << std::endl;

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
      
    std::cout << "Dijkstra's search failed after " << stateExpansions_
	      << " state expansions" << std::endl;

    return false;
  }

  bool dijkstraSearch(SearchState<T> *startState, SearchState<T> *goalState)
  {
    CLOSED_.clear();
    Heap<T> Q;
    Q.insertState(startState, 0.0f);
    startState->setPathCost(0.0f);
    while(Q.size() > 0)
    {
      SearchState<T> *x = Q.remove().state;
      CLOSED_.insert(x);
      // Check if x is the goal state
      if(equals(x->getValue(), goalState->getValue()))
      {
        /* std::cout << "Dijkstra's search found goal state" << std::endl; */
        return true;
      }
      std::vector<SearchState<T> *> neighbors;
      std::vector<double> costs;
      stateExpansions_++;
      this->environment_->getSuccessors(x, neighbors, costs);
      for(auto it = neighbors.begin(); it != neighbors.end(); ++it)
      {
        // Check if already visited
        if(CLOSED_.get((*it)->getValue()) == nullptr)
	{
          // Relax the edge between x and its neighbor
          relax(x, *it);
          CLOSED_.insert(*it);
          Q.insertState(*it, (*it)->getPathCost());
        }
        /* else std::cout << "Already visited!" << std::endl; */
      }
    }
    return false;
  }

  virtual bool equals(T a, T b)
  {
    return (a == b);
  }

protected:
  /* Relax the edge (u, v) where cost is the cost of traversing that edge */
  void relax(SearchState<T> *u, SearchState<T> *v)
  {
    double cost = INFINITE_COST;
    this->environment_->getCost(u, v, cost);
    if(v->getPathCost() > u->getPathCost() + cost)
    {
       v->setPathCost(u->getPathCost() + cost);
       v->setParent(u);
    }
  }

  HashTable<T> CLOSED_;

  int stateExpansions_;

};

}

#endif
