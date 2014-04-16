//
//  MPTimer.h
//
//  Created by Ellis Ratner on 4/16/14.
//  Copyright (c) 2014 Ellis Ratner. All rights reserved.
//

#ifndef _MPTimer_h
#define _MPTimer_h

#include <chrono>
#include <ostream>

using namespace std::chrono;

#define GET_ELAPSED_MILLI(timer) duration_cast<milliseconds>(timer.getTimeElapsed()).count()
#define GET_ELAPSED_MICRO(timer) duration_cast<microseconds>(timer.getTimeElapsed()).count()

namespace MP
{

class Timer 
{
public:
  void start() { start_ = high_resolution_clock::now(); }

  high_resolution_clock::duration getTimeElapsed() const { return high_resolution_clock::now() - start_; }

private:
  high_resolution_clock::time_point start_;

};

}

#endif
