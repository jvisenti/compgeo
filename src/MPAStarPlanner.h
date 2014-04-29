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
        T wGoal = goal;
        
        // convert world states to planner states
        this->environment_->worldToPlanner(start);
        
        this->environment_->worldToPlanner(goal);
        
        if (!this->environment_->stateValid(start))
        {
            std::cout << "A* plan failed because start state is invalid" << std::endl;
            return false;
        }
        else if (!this->environment_->stateValid(goal))
        {
            std::cout << "A* plan failed because goal state is invalid" << std::endl;
            return false;
        }
        
        SearchState<T> *s = this->environment_->addState(start);
        SearchState<T> *g = this->environment_->addState(goal);
        stateExpansions_ = 0;
        
        Timer timer;
        timer.start();
        
        bool success = aStarSearch(s, g);
        
        std::cout << "A* search terminated after "
        << stateExpansions_ << " state expansions in "
        << GET_ELAPSED_MICRO(timer) / 1000000.0 << " seconds" << std::endl;
        
        if(success)
        {
            // Re-construct the path from s to g by following pointers
            SearchState<T> *v = g;
            while(v != nullptr && v != s)
            {
                T stateVal = v->getValue();
                this->environment_->plannerToWorld(stateVal);
                
                plan.push_back(stateVal);
                v = v->getParent();
            }
            
            std::reverse(plan.begin(), plan.end());
            
            // snap to goal state if we didn't hit it exactly
            if (!(g->getValue() == wGoal))
            {
                plan.push_back(wGoal);
            }
            
            std::cout << "A* planner succeeded with " << plan.size() << " states" << std::endl;
            
            return true;
        }
        
        return false;
    }
    
    bool aStarSearch(SearchState<T> *startState, SearchState<T> *goalState)
    {
        CLOSED_.clear();
        exploredStates_.clear();
        Heap<T> OPEN;
        
        //startState->setParent(startState); ??
        startState->setPathCost(0.0f);
        OPEN.insertState(startState, 4.0f * heuristic_(startState->getValue(), goalState->getValue()));
        
        while(OPEN.size() > 0)
        {
            SearchState<T> *s = OPEN.remove().state;
            T stateVal = s->getValue();
            
            // Check if s is the goal state
            if(stateVal == goalState->getValue())
            {
                return true;
            }
            
            CLOSED_.insert(s);
            
            if(!this->environment_->stateValid(stateVal))
            {
                continue;
            }
            
            exploredStates_.push_back(stateVal);
            
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
                                         4.0f * heuristic_((*it)->getValue(), goalState->getValue()));
                    }
                    else
                    {
                        OPEN.decreaseKey(*it, (*it)->getPathCost() + 
                                         4.0f * heuristic_((*it)->getValue(), goalState->getValue()));
                    }
                }
            }
        }
        
        return false;
    }
    
    const std::vector<T>& getExploredStates() const { return this->exploredStates_; }
    
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
    
    std::vector<T> exploredStates_;
    
    int stateExpansions_;
    
};
    
}

#endif 
