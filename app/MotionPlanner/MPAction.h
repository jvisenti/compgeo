//
//  MPAction.h
//  MotionPlanner
//
//  Created by Ellis M. Ratner on 4/21/14.
//

#ifndef __MPAction__
#define __MPAction__

#include <iostream>

namespace MP
{

template <typename T>
class Action
{
public:
    Action(double cost)
    : cost_(cost)
    {
    }
    
    virtual ~Action()
    {
    }
    
    /* Map from T to T under this action */
    virtual T operator()(const T &) = 0;
    
    void setCost(double c)
    {
        cost_ = c;
    }
    
    double getCost() const
    {
        return cost_;
    }
    
protected:
    double cost_;
    
};
    
}

#endif
