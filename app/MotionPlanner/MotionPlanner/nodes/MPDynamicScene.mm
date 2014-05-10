//
//  MPDynamicScene.m
//  MotionPlanner
//
//  Created by John Visentin on 5/1/14.
//  Copyright (c) 2014 Brockenhaus Studio. All rights reserved.
//

#import "MPDynamicScene.h"
#import "MPPotentialFieldController.h"

#define kMPDynamicSceneVoxelSize 0.25f

#define kMPPotentialGradStep 0.01f
#define kMPPotentialAttractiveMultiplier 4.0f
#define kMPPotentialRepulsiveMultiplier 1.0f

@interface MPDynamicScene ()

@property (nonatomic, assign) MP::Transform3D *goal;
@property (nonatomic, assign) MP::PotentialFieldController *controller;

@end

@implementation MPDynamicScene

#pragma mark - property overrides

- (BOOL)isUserInteractionEnabled
{
    return YES;
}

- (void)setEnvironment:(MP::Environment3D *)environment
{
    [super setEnvironment:environment];
    
    delete self.goal;
    delete self.controller;
    
    if (self.environment)
    {
        self.controller = new MP::PotentialFieldController(self.environment->getObstacles(), self.environment->getActiveObject(), kMPDynamicSceneVoxelSize);
        
        self.controller->setGradStep(kMPPotentialGradStep);
        self.controller->setAttractiveMultiplier(kMPPotentialAttractiveMultiplier);
        self.controller->setRepulsiveMultiplier(kMPPotentialRepulsiveMultiplier);
    }
}

#pragma mark - public interface

- (void)move
{
    [self moveTo:self.shadow.model->getTransform()];
}

- (void)moveTo:(const MP::Transform3D &)goal
{
    delete self.goal;
    
    self.goal = new MP::Transform3D(goal.getPosition(), goal.getScale(), goal.getRotation());
    
    if (self.controller)
    {
        self.controller->setGoal(goal);
        self.controller->move();
    }
}

- (void)update:(NSTimeInterval)dt
{
    [super update:dt];
    
    if (self.controller && self.activeObject.model && self.goal)
    {
        self.controller->move();
    }
}

- (void)cleanup
{
    [super cleanup];
    
    delete self.goal;
    
    if (self.controller)
    {
        delete self.controller;
    }
}

@end
