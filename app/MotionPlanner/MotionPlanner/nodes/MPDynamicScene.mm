//
//  MPDynamicScene.m
//  MotionPlanner
//
//  Created by John Visentin on 5/1/14.
//  Copyright (c) 2014 Brockenhaus Studio. All rights reserved.
//

#import "MPDynamicScene.h"
#import "MPPotentialFieldController.h"

@interface MPDynamicScene ()

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
    
    delete self.controller;
    
    if (self.environment)
    {
        self.controller = new MP::PotentialFieldController(self.environment->getObstacles(), self.environment->getActiveObject());
    }
}

#pragma mark - public interface

- (void)move
{
    [self moveTo:self.shadow.model->getTransform()];
}

- (void)moveTo:(const MP::Transform3D &)goal
{
    self.controller->setGoal(goal);
    self.controller->move();
}

- (void)update:(NSTimeInterval)dt
{
    [super update:dt];
    
//    if (self.controller && self.activeObject.model)
//    {
//        self.controller->move();
//    }
}

- (void)cleanup
{
    [super cleanup];
    
    if (self.controller)
    {
        delete self.controller;
    }
}

@end
