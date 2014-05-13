//
//  MPBenchmarker.cpp
//  MotionPlanner
//
//  Created by Ellis Ratner on 5/9/14.
//

#include "MPBenchmarker.h"

namespace MP
{
    Benchmarker::Benchmarker()
    : environment_(nullptr), planner_(nullptr), startGoalPairs_(), planCounter_(0)
    {
        srand(static_cast<unsigned>(time(0)));
    }
    
    Benchmarker::~Benchmarker()
    {
        if(planner_)
        {
            delete planner_;
            planner_ = nullptr;
        }
        if(environment_)
        {
            delete environment_;
            environment_ = nullptr;
        }
    }
    
    void Benchmarker::loadEnvironment(const std::string &file)
    {
        Reader reader(file);
        
        if(environment_)
        {
            delete environment_;
            environment_ = nullptr;
        }
        
        environment_ = reader.generateEnvironment3D();
    }
    
    void Benchmarker::benchmark(int N, const Action6D::ActionSet &actionSet)
    {
        assert(environment_ != nullptr);
        
        std::cout << "\t *** BEGIN BENCHMARKING ***" << std::endl;
        
        MPVec3 halfSize = MPVec3MultiplyScalar(environment_->getSize(), 0.5f);
        
        MPVec3 min = MPVec3Subtract(environment_->getOrigin(), halfSize);
        MPVec3 max = MPVec3Add(environment_->getOrigin(), halfSize);
        
        MPAABox environmentBounds = MPAABoxMake(min, max);
        
        // Randomly generate N start/goal pairs within the bounds of the environment
        generateRandomStartGoalPairs3D(N, environmentBounds);
        
        if(planner_ != nullptr)
        {
            delete planner_;
            planner_ = nullptr;
        }
        
        // Set the action set to be in 3D
        Model *activeObject = environment_->getActiveObject();
        activeObject->setActionSet(actionSet);
        
        // Instantiate the planner
        planner_ = new AStarPlanner<Transform3D>(environment_, manhattanHeuristic);
        
        std::vector<std::vector<float> > planningTimes;
        std::vector<int> successes;
        
        // Test the planner with various weights
        for(int step = 0; step < 8; ++step)
        {
            int w;
            if(step < 2)
                w = step;
            else
                w = 2*step - 2;
            
            static_cast<AStarPlanner<Transform3D> *>(planner_)->setWeight(1.0f * w);
            
            std::cout << "\t *** WEIGHT = " << w << " ***" << std::endl;
            
            Timer timer;
            planningTimes.push_back(std::vector<float>());
            double planningTime = 0.0f;
            int success = 0, failure = 0;
            std::vector<Transform3D> plan;
            for(int i = 0; i < N; ++i)
            {
                planCounter_ = (step + 1) * i;
                plan.clear();
                std::cout << "Start: (" << startGoalPairs_.at(i).first.getPosition().x
                << ", " << startGoalPairs_.at(i).first.getPosition().y
                << ", " << startGoalPairs_.at(i).first.getPosition().z
                << "), Goal: (" << startGoalPairs_.at(i).second.getPosition().x
                << ", " << startGoalPairs_.at(i).second.getPosition().y
                << ", " << startGoalPairs_.at(i).second.getPosition().z
                << ")" << std::endl;
                
                // Reset the environment before each plan TODO
//                environment_->reset();
                
                timer.start();
                // Give the planner a timeout, and then force it to stop
                Later stopPlanner(PLANNER_TIMEOUT * 1000.0f, true, &Benchmarker::stopPlanning, this, (step + 1) * i);
                
                if(planner_->plan(startGoalPairs_.at(i).first, startGoalPairs_.at(i).second, plan))
                {
                    planningTime = (GET_ELAPSED_MICRO(timer) / 1000000.0f);
                    std::cout << "Success! Plan took " << planningTime << " seconds " << std::endl;
                    success++;
                }
                else
                {
                    std::cout << "Plan failed :(" << std::endl;
                    failure++;
                }
                
                planningTimes.at(step).push_back(planningTime);
            }
            
            std::cout << "\t *** TEST DONE ***" << std::endl;
            std::cout << success << " succeeded plans, " << failure << " failed plans" << std::endl;;
            std::cout << "Success rate: " << (1.0f * success / N) * 100.0f << "%" << std::endl;
            successes.push_back(success);
        }
        
        std::cout << "\t *** SUMMARY ***" << std::endl;
        for(int i = 0; i < planningTimes.size(); ++i)
        {
            float totalTime = 0.0f;
            int n = 0;
            for(auto time : planningTimes.at(i))
            {
                if(time <= PLANNER_TIMEOUT)
                {
                    totalTime += time;
                    n++;
                }
            }
            
            int weight = (i < 2 ? i : 2*i - 2);
            
            std::cout << "Weight " << weight << ": (average) " << totalTime / n  << " (success rate) " << ((1.0f * successes.at(i)) / N) * 100.0f << "%" << std::endl;
        }
    }
    
    void Benchmarker::generateRandomStartGoalPairs3D(int N, const MPAABox &region)
    {
        assert(environment_ != nullptr);
        
        startGoalPairs_.clear();
        
        for(int s = 0; s < N; ++s)
        {
            Transform3D start = randomTransform3D(region);
            Transform3D goal = randomTransform3D(region);
            
            startGoalPairs_.push_back(std::make_pair(start, goal));
        }
        
        std::cout << "Done adding " << N << " randomly generated start/goal pairs" << std::endl;
    }
    
    Transform3D Benchmarker::randomTransform3D(const MPAABox &region)
    {
        float x, y, z;
        bool valid = false;
        Transform3D transform;
        while(!valid)
        {
            x = region.min.x + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (region.max.x - region.min.x)));
            y = region.min.y + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (region.max.y - region.min.y)));
            z = region.min.z + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (region.max.z - region.min.z)));
            transform = Transform3D(MPVec3Make(x, y, z), MPVec3Make(1.0f, 1.0f, 1.0f), MPQuaternionIdentity);
            
            Transform3D plannerTransform(transform);
            environment_->worldToPlanner(plannerTransform);
            
            if(environment_->stateValid(plannerTransform))
            {
                valid = true;
                std::cout << "found one!" << std::endl;
            }
            else
            {
                std::cout << "bad :(" << std::endl;
            }
        }
        
        return transform;
    }
    
    void Benchmarker::stopPlanning(int plan)
    {
        if(planCounter_ == plan && planner_ != nullptr)
        {
            std::cout << "\t *** STOPPING PLANNER ***" << std::endl;
            
            planner_->stopPlanning();
        }
    }
    
}