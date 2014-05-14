//
//  MPGLView.h
//  MotionPlanner
//
//  Created by John Visentin on 4/4/14.
//  Copyright (c) 2014 Brockenhaus Studio. All rights reserved.
//

#import "BHGLView.h"
#import "MPScene.h"

@interface MPGLView : BHGLView

@property (nonatomic, strong) MPScene *scene;

- (void)stopPlanning;

@end
