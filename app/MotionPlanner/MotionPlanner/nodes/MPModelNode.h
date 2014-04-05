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

- (void)setModel:(const MP::Model &)model;
- (const MP::Model &)model;

- (id)initWithModel:(const MP::Model &)model;

@end
