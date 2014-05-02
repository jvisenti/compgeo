//
//  MPStaticScene.h
//  MotionPlanner
//
//  Created by John Visentin on 5/1/14.
//  Copyright (c) 2014 Brockenhaus Studio. All rights reserved.
//

#import "MPScene.h"

@interface MPStaticScene : MPScene

@property (atomic, readonly, getter = isPlanning) BOOL planning;

@property (atomic, assign) BOOL showExpandedStates;

@property (nonatomic, assign) double planningDelayMultiplier;
@property (nonatomic, assign) double planningWeight;

- (BOOL)plan;
- (BOOL)planTo:(const MP::Transform3D &)goal;
- (BOOL)planFrom:(const MP::Transform3D &)start to:(const MP::Transform3D &)goal;

- (void)executePlan;

@end
