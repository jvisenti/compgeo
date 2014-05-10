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

@property (nonatomic, readonly) NSArray *obstacles;

@property (nonatomic, weak) MPModelNode *activeObject;
@property (nonatomic, weak) MPModelNode *shadow;

@property (nonatomic, assign) MP::Environment3D *environment;

- (id)initWithEnvironment:(MP::Environment3D *)environment;

- (BOOL)transform:(MP::Transform3D &)transform validForModel:(MP::Model *)model;

- (void)cleanup;

@end
