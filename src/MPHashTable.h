//
//  MPHashTable.h
//
//  Created by Ellis Ratner on 4/3/14.
//  Copyright (c) 2014 Ellis Ratner. All rights reserved.
//

#ifndef _MPHashTable_h
#define _MPHashTable_h

#include "MPSearchState.h"
#include <vector>

#define DEFAULT_HASH_TABLE_SIZE 256
#define DEFAULT_MAX_LOAD_FACTOR 0.7

namespace MP
{

/* A hash table data structure with chaining */
template <typename T>
struct HashTableElement
{
  SearchState<T> *state;
  HashTableElement *next;
};

template <typename T>
class HashTable
{
public:
  /* Needs a has function that hashes objects of type T */
  HashTable(int (*hash)(T),
	    int initialSize = DEFAULT_HASH_TABLE_SIZE, 
	    double maxLoadFactor = DEFAULT_MAX_LOAD_FACTOR)
    : numElements_(0), slots_(initialSize), maxLoadFactor_(maxLoadFactor), hash_(hash)
  {
  }

  ~HashTable()
  {
    clear();
  }

  void insert(SearchState<T> *s)
  {
    int slot = hash_(s->getValue());

    // @todo
  }

  void remove(SearchState<T> *s)
  {
    // @todo
  }

  SearchState<T> *get(T t)
  {
    int slot = hash_(t) % slots_.size();

    // Search the slot for the appropriate entry
    HashTableElement<T> *e = slots_[slot];
    while(e != nullptr)
    {
      if(e->state->getValue() == t)
	return e;

      e = e->next;
    }

    return nullptr;
  }

  inline double getMaxLoadFactor() const { return maxLoadFactor_; }

  inline void setMaxLoadFactor(double a) { maxLoadFactor_ = a; }

private:
  inline double getLoadFactor() const { return ((double)numElements_)/slots_.size(); }

  void clear()
  {
    for(auto it = slots_.begin(); it != slots_.end(); ++it)
    {
      HashTableElement<T> *hit = *it;
      while(hit != nullptr)
      {
	HashTableElement<T> *next = hit->next;
	delete hit;
	hit = 0;
	hit = next;
      }
    }
  }

  int numElements_;

  std::vector<HashTableElement<T> *> slots_;

  double maxLoadFactor_;

  int (*hash_)(T);
 
};

}

#endif
