//
//  MPPathNode.h
//  MotionPlanner
//
//  Created by John Visentin on 4/12/14.
//  Copyright (c) 2014 Brockenhaus Studio. All rights reserved.
//

#import "BHGLModelNode.h"
#import "MPMath.h"
#import <vector>

@interface MPPathNode : BHGLModelNode

- (id)initWithPath:(const std::vector<MPVec3> &)path;

- (void)setPath:(const std::vector<MPVec3> &)path;
- (const std::vector<MPVec3> &)path;

@end
