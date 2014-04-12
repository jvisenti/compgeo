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

@property (nonatomic, assign) const MP::Environment3D *environment;

@property (nonatomic, readonly) BHGLNode *rootNode;
@property (nonatomic, readonly) MPModelNode *activeObject;

- (id)initWithEnvironment:(const MP::Environment3D *)environment;

- (BOOL)transform:(MP::Transform3D &)transform validForModel:(MP::Model *)model;

@end
