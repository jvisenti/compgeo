//
//  MPEnviroment.h
//
//  Created by Ellis Ratner on 4/4/14.
//  Copyright (c) 2014 Ellis Ratner. All rights reserved.
//

#ifndef _MPEnvironment_h
#define _MPEnvironment_h

#include "MPSearchState.h"
#include "MPHashTable.h"
#include <vector>

namespace MP
{
    
template <typename T>
class Environment
{
public:
    typedef int (*hfptr)(const T&);
    
    Environment(hfptr THash) : states_(THash), invalidStates_(THash), hashFunction_(THash) { }
    
    virtual ~Environment()
    {
        clear();
    }
    
    virtual void getSuccessors(SearchState<T> *s,
                               std::vector<SearchState<T> *> &successors,
                               std::vector<double> &costs) = 0;
    
    inline int getNumStates() const { return states_.size(); }
    
    SearchState<T> *addState(T p)
    {
        SearchState<T> *s = states_.get(p);
        if(s == nullptr)
        {
            s = new SearchState<T>();
            s->setValue(p);
            states_.insert(s);
        }
        return s;
    }
    
    virtual bool getCost(SearchState<T> *s, SearchState<T> *t, double &cost) = 0;
    
    inline hfptr getHashFunction() const { return hashFunction_; }
    
    virtual bool stateValid(const T &state)
    {
        return invalidStates_.get(state) == nullptr;
    }
    
protected:
    void clear()
    {
        // The environment is responsible for allocating and deallocating states
        for(int i = 0; i < states_.getNumSlots(); ++i)
        {
            HashTableElement<T> *it = states_.getSlot(i);
            while(it != nullptr)
            {
                delete it->state;
                it->state = nullptr;
                it = it->next;
            }
        }
        
        for(int i = 0; i < invalidStates_.getNumSlots(); ++i)
        {
            HashTableElement<T> *it = invalidStates_.getSlot(i);
            while(it != nullptr)
            {
                delete it->state;
                it->state = nullptr;
                it = it->next;
            }
        }
    }
    
    HashTable<T> states_;
    HashTable<T> invalidStates_;
    hfptr hashFunction_;
    
};
    
}

#endif
