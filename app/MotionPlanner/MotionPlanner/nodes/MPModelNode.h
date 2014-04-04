//
//  MPModelNode.h
//  MotionPlanner
//
//  Created by John Visentin on 4/4/14.
//  Copyright (c) 2014 Brockenhaus Studio. All rights reserved.
//

#import "BHGLModelNode.h"
#import "MPModel.h"

@interface MPModelNode : BHGLModelNode

@property (nonatomic, assign) MPModel &model;

- (id)initWithPODFileNamed:(NSString *)fileName;

@end
