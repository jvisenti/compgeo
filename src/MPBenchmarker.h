//
//  MPBenchmarker.h
//
//  Created by Ellis Ratner on 5/9/14.
//

#ifndef __MPBenchmarker__
#define __MPBenchmarker__

#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <thread>
#include <functional>
#include <future>
#include <chrono>
#include <cstdio>
#include "MPEnvironment3D.h"
#include "MPPlanner.h"
#include "MPReader.h"
#include "MPAStarPlanner.h"
#include "MPAction.h"

#define PLANNER_TIMEOUT 30.0f

namespace MP
{
    // Taken from http://stackoverflow.com/questions/14650885/how-to-create-timer-events-using-c-11
    class Later
    {
    public:
        template <class callable, class... arguments>
        Later(int after, bool async, callable &&f, arguments &&... args)
        {
            std::function<typename std::result_of<callable(arguments...)>::type()> task(std::bind(std::forward<callable>(f), std::forward<arguments>(args)...));
            
            if(async)
            {
                std::thread([after, task]()
                {
                    std::this_thread::sleep_for(std::chrono::milliseconds(after));
                    task();
                }).detach();
            }
            else
            {
                std::this_thread::sleep_for(std::chrono::milliseconds(after));
                task();
            }
        }
        
    };
    
    class Benchmarker
    {
    public:
        Benchmarker();
        ~Benchmarker();
        
        void loadEnvironment(const std::string &file);
        
        void benchmark(int N, const Action6D::ActionSet &actionSet);
        
        float getEnvStepSize() const { return environment_->getStepSize(); }
        
        float getEnvRotationStepSize() const { return environment_->getRotationStepSize(); }
        
    private:
        void generateRandomStartGoalPairs3D(int N, const MPAABox &region);
        
        Transform3D randomTransform3D(const MPAABox &region);
        
        void stopPlanning(int plan);
        
        Environment3D *environment_;
        Planner<Transform3D> *planner_;
        
        std::vector<std::pair<Transform3D, Transform3D> > startGoalPairs_;
        
        int planCounter_;
        
    };
}

#endif
