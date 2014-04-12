//
//  MPModelNode.h
//  MotionPlanner
//
//  Created by John Visentin on 4/4/14.
//  Copyright (c) 2014 Brockenhaus Studio. All rights reserved.
//

#import "BHGLModelNode.h"
#import "MPModel.h"

@class  MPScene;

@interface MPModelNode : BHGLModelNode

@property (nonatomic, assign) MP::Model *model;

@property (nonatomic, weak) MPScene *scene;

- (id)initWithModel:(MP::Model *)model;

@end
