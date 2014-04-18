//
//  MPScene.h
//  MotionPlanner
//
//  Created by John Visentin on 4/4/14.
//  Copyright (c) 2014 Brockenhaus Studio. All rights reserved.
//

#import "BHGLScene.h"
#import "MPEnvironment3D.h"
#import "MPModelNode.h"

@interface MPScene : BHGLScene

@property (atomic, readonly, getter = isUserInteractionEnabled) BOOL userInteractionEnabled;
@property (nonatomic, readonly) BHGLNode *rootNode;

- (id)initWithEnvironment:(MP::Environment3D *)environment;

- (void)setEnvironment:(MP::Environment3D *)environment;
- (const MP::Environment3D *)getEnvironment;

- (BOOL)transform:(MP::Transform3D &)transform validForModel:(MP::Model *)model;

- (BOOL)plan;
- (BOOL)planTo:(const MP::Transform3D &)goal;
- (BOOL)planFrom:(const MP::Transform3D &)start to:(const MP::Transform3D &)goal;

- (void)executePlan;

- (void)animateActiveObject:(BHGLAnimation *)animation;
- (void)removeAnimationFromActiveObject:(BHGLAnimation *)animation;

- (void)animateShadow:(BHGLAnimation *)animation;
- (void)removeAnimationFromShadow:(BHGLAnimation *)animation;

@end
