//
//  MPStaticScene.m
//  MotionPlanner
//
//  Created by John Visentin on 5/1/14.
//  Copyright (c) 2014 Brockenhaus Studio. All rights reserved.
//

#import "MPStaticScene.h"
#import "BHGL.h"
#import "MPPathNode.h"
#import "MPAStarPlanner.h"
#import "MPUtils.h"

#define kMPPlanStatesPerSec 5

#define kMPPlanMaxDelay 100000

@interface MPStaticScene ()
{
    __weak MPPathNode *_pathNode;
}

@property (nonatomic, assign) MP::AStarPlanner<MP::Transform3D> *planner;
@property (nonatomic, assign) std::vector<MP::Transform3D> &planStates;

@property (nonatomic, weak) MPPathNode *pathNode;

@end

@implementation MPStaticScene

@synthesize pathNode = _pathNode;

#pragma mark - property overrides

- (BOOL)isUserInteractionEnabled
{
    return !self.isPlanning;
}

- (void)setPlanningDelayMultiplier:(double)planningDelayMultiplier
{
    _planningDelayMultiplier = planningDelayMultiplier;
    
    if (self.planner)
    {
        self.planner->setDelay((int)(planningDelayMultiplier * kMPPlanMaxDelay));
    }
}

- (void)setPlanningWeight:(double)planningWeight
{
    _planningWeight = planningWeight;
    
    if (self.planner)
    {
        self.planner->setWeight(planningWeight);
    }
}

- (void)setEnvironment:(MP::Environment3D *)environment
{
    [super setEnvironment:environment];
    
    MPPathNode *pathNode = [[MPPathNode alloc] init];
    [self insertChild:pathNode atIndex:0];
    self.pathNode = pathNode;
    
    delete self.planner;
    
    if (self.environment)
    {
        self.planner = new MP::AStarPlanner<MP::Transform3D>(environment, MP::manhattanHeuristic);
        self.planner->setDelay((int)(self.planningDelayMultiplier * kMPPlanMaxDelay));
        self.planner->setWeight(self.planningWeight);
    }
}

#pragma mark - public interface

- (void)render
{
    if (!self.showExpandedStates)
    {
        [super render];
        return;
    }
    
    [self configureProgram:self.program];
    
    [self.rootNode.children enumerateObjectsUsingBlock:^(BHGLNode *child, NSUInteger idx, BOOL *stop) {
        if (![child isEqual:self.boundingBox])
        {
            [child render];
        }
    }];
    
    if (self.environment && self.planner && self.shadow.model)
    {
        MP::Transform3D current = self.shadow.model->getTransform();
        BHGLColor currentColor = self.shadow.material.surfaceColor;
        
        std::vector<MP::Transform3D> states = self.planner->getExploredStates();
        
        self.shadow.material.surfaceColor = BHGLColorMake(0.0f, 0.0f, 0.0f, 0.15f);
        self.shadow.material.emissionColor = self.shadow.material.surfaceColor;
        
        for (int i = 0; i < states.size() && i < 100; ++i)
        {
            MP::Transform3D t = states.at(states.size() - 1 - i);
            self.environment->plannerToWorld(t);
            self.shadow.model->setTransform(t);
            
            [self.shadow render];
        }
        
        self.shadow.material.surfaceColor = currentColor;
        self.shadow.material.emissionColor = self.shadow.material.surfaceColor;
        self.shadow.model->setTransform(current);
    }
    
    [self.boundingBox render];
}

- (BOOL)plan
{
    return [self planTo:self.shadow.model->getTransform()];
}

- (BOOL)planTo:(const MP::Transform3D &)goal
{
    return [self planFrom:self.activeObject.model->getTransform() to:goal];
}

- (BOOL)planFrom:(const MP::Transform3D &)start to:(const MP::Transform3D &)goal
{
    if (self.isPlanning)
    {
        NSLog(@"planner is already planning!");
        return NO;
    }
    
    if (self.planner)
    {
        _planning = YES;
        
        self.planStates.clear();
        
        [self.pathNode clearPath];
        
        if(!self.planner->plan(start, goal, self.planStates))
        {
            printf("failed to find plan from ");
            MPVec3Print(start.getPosition());
            printf(" to ");
            MPVec3Print(goal.getPosition());
            printf("\n");
            
            self.planner->reset();
            _planning = NO;
            
            return NO;
        }
        
        self.planner->reset();
        _planning = NO;
        
        return YES;
    }
    
    return NO;
}

- (void)stopPlanning
{
    if (self.planner)
    {
        self.planner->stopPlanning();
    }
}

- (void)executePlan
{
    std::vector<MPVec3> path;
    BHGLKeyframeAnimation *anim = [BHGLKeyframeAnimation animationWithFrames:(int)self.planStates.size() fps:kMPPlanStatesPerSec];
    
    for (int i = 0; i < self.planStates.size(); ++i)
    {
        MP::Transform3D &transform = self.planStates.at(i);
        
        path.push_back(transform.getPosition());
        
        [anim setTranslation:MPVec3ToGLKVector3(transform.getPosition()) forFrame:i];
        [anim setScale:MPVec3ToGLKVector3(transform.getScale()) forFrame:i];
        [anim setRotation:MPQuaternionToGLKQuaternion(transform.getRotation()) forFrame:i];
    }
    
    [self.pathNode setPath:path];
    
    [self.activeObject removeAllAnimations];
    [self.activeObject runAnimation:anim];
}

- (void)cleanup
{
    [super cleanup];
    
    if (self.planner)
    {
        delete self.planner;
    }
}

@end
