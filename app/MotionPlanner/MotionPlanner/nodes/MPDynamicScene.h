//
//  MPDynamicScene.h
//  MotionPlanner
//
//  Created by John Visentin on 5/1/14.
//  Copyright (c) 2014 Brockenhaus Studio. All rights reserved.
//

#import "MPScene.h"

@interface MPDynamicScene : MPScene

- (void)move;
- (void)moveTo:(const MP::Transform3D &)goal;

@end
