//
//  MPMotion.h
//
//  Created by John Visentin on 5/10/14.
//  Copyright (c) 2014 Brockenhaus Studio. All rights reserved.
//

#ifndef __MPMotion__
#define __MPMotion__

#include <vector>
#include "MPMath.h"

namespace MP
{
    
class Motion
{
public:
    Motion() : repeats_(true), loops_(true), duration_(0.0f) {}
    
    void setPath(const std::vector<MPVec3>& path) { path_ = path; }
    std::vector<MPVec3>& path() { return path_; }
    const std::vector<MPVec3>& path() const { return path_; }
    
    void setRepeats(bool repeats) { repeats_ = repeats; }
    bool repeats() const { return repeats_; }
    
    void setLoops(bool loops) { loops_ = loops; }
    bool loops() const { return loops_; }
    
    void setDuration(double duration) { duration_ = duration; }
    double duration() const { return duration_; }
    
private:
    std::vector<MPVec3> path_;
    
    bool repeats_;
    bool loops_;
    
    double duration_;
};
    
}


#endif
