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
#include <iostream>

#define DEFAULT_HASH_TABLE_SIZE 512
#define DEFAULT_MAX_LOAD_FACTOR 0.7

namespace MP
{

/* A hash table data structure with chaining */
template <typename T>
struct HashTableElement
{
  HashTableElement() { }
  HashTableElement(SearchState<T> *s, HashTableElement<T> *n)
  : state(s), next(n) { }

  SearchState<T> *state;
  HashTableElement<T> *next;
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
    // @todo If the load factor is two high, double the size of the hash table,
    // and re-insert all the elements
    // ...
    if(getLoadFactor() > getMaxLoadFactor())
    {
      std::cout << "Warning: hash table has exceeded the maximum load factor ("
		<< getLoadFactor() << ")" << std::endl;
      increaseTableSize();
    }

    int slot = hash_(s->getValue()) % slots_.size();
    std::cout << "Hashing state to slot " << slot << std::endl;

    HashTableElement<T> *e = slots_[slot];
    if(e == nullptr)
      slots_[slot] = new HashTableElement<T>(s, nullptr);
    else
    {
      while(e->next != nullptr)
	e = e->next;

      e->next = new HashTableElement<T>(s, nullptr);
    }
    
    // @todo should we check if it's already in the table?
    numElements_++;
  }

  bool remove(SearchState<T> *s)
  {
    // Find the bin where the state should be
    int slot = hash_(s->getValue());

    // @todo
  }

  SearchState<T> *get(T t)
  {
    int slot = hash_(t) % slots_.size();
    std::cout << "Checking slot " << slot << std::endl;

    // Search the slot for the appropriate entry
    HashTableElement<T> *e = slots_[slot];
    while(e != nullptr)
    {
      std::cout << "Comparing (" << e->state->getValue().x_ << ", " 
		<< e->state->getValue().y_ << ") to (" << t.x_
		<< ", " << t.y_ << ")" << std::endl;
      if(e->state->getValue() == t)
      {
	std::cout << "Found in the hash table" << std::endl;
	return e->state;
      }

      e = e->next;
    }

    return nullptr;
  }

  inline double getMaxLoadFactor() const { return maxLoadFactor_; }

  inline void setMaxLoadFactor(double a) { maxLoadFactor_ = a; }

  inline double getLoadFactor() const { return ((double)numElements_)/slots_.size(); }

  inline int size() const { return numElements_; }

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

private:
  void increaseTableSize()
  {
    // @todo
  }

  int numElements_;

  std::vector<HashTableElement<T> *> slots_;

  double maxLoadFactor_;

  int (*hash_)(T);
 
};

}

#endif
