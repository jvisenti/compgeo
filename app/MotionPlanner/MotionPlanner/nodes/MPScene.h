//
//  MPScene.h
//  MotionPlanner
//
//  Created by John Visentin on 4/4/14.
//  Copyright (c) 2014 Brockenhaus Studio. All rights reserved.
//

#import "BHGLScene.h"
#import "MPModelNode.h"

@interface MPScene : BHGLScene

@property (nonatomic, strong) BHGLNode *activeObject;

- (BOOL)transform:(MP::Transform3D &)transform validForModel:(MPModelNode *)model;

@end
