//
//  MPDynamicScene.m
//  MotionPlanner
//
//  Created by John Visentin on 5/1/14.
//  Copyright (c) 2014 Brockenhaus Studio. All rights reserved.
//

#import "MPDynamicScene.h"
#import "MPPotentialFieldController.h"

#import "BHGLKeyframeAnimation.h"

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
        for (MPModelNode *obstacleNode in self.obstacles)
        {
            MP::Motion *motion = obstacleNode.model->getMotion();
            
            if (motion)
            {
                std::vector<MPVec3> path = motion->path();
                
                if (motion->loops())
                {
                    path.push_back(path.at(0));
                }
                
                BHGLAnimation *anim = [BHGLKeyframeAnimation animationWithFrames:(int)path.size() fps:(int)(path.size() / motion->duration())];
                
                for (int i = 0; i < path.size(); ++i)
                {
                    // bit of a hack... assumes obstacles can't scale or rotate in the future
                    [(BHGLKeyframeAnimation *)anim setScale:MPVec3ToGLKVector3(obstacleNode.model->getScale()) forFrame:i];
                    [(BHGLKeyframeAnimation *)anim setRotation:MPQuaternionToGLKQuaternion(obstacleNode.model->getRotation()) forFrame:i];
                    
                    [(BHGLKeyframeAnimation *)anim setTranslation:MPVec3ToGLKVector3(path.at(i)) forFrame:i];
                }
                
                if (!motion->loops())
                {
                    anim = [BHGLAnimation sequence:@[anim, [anim reversedAnimation]]];
                }
                
                anim.repeats = motion->repeats();
                
                [obstacleNode runAnimation:anim];
            }
        }
        
        self.controller = new MP::PotentialFieldController(self.environment->getObstacles(), self.environment->getActiveObject(), kMPDynamicSceneVoxelSize);
        
        self.controller->setGradStep(kMPPotentialGradStep);
        self.controller->setAttractiveMultiplier(kMPPotentialAttractiveMultiplier);
        self.controller->setRepulsiveMultiplier(kMPPotentialRepulsiveMultiplier);
    }
}

#pragma mark - public interface

- (BOOL)transform:(MP::Transform3D &)transform validForModel:(MP::Model *)model
{
    // no true collision detection for dynamic scenes
    return YES;
}

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
