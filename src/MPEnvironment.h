//
//  MPEnviroment.h
//
//  Created by Ellis Ratner on 4/4/14.
//  Copyright (c) 2014 Ellis Ratner. All rights reserved.
//

#ifndef _MPEnvironment_h
#define _MPEnvironment_h

#include "MPSearchState.h"
#include <vector>

namespace MP
{

template <typename T>
class Environment
{
public:
  Environment() { }

  virtual ~Environment() { }

  virtual void getSuccessors(SearchState<T> *s, 
			     std::vector<SearchState<T> *> &successors, 
			     std::vector<double> &costs) = 0;
  
};

}

#endif
