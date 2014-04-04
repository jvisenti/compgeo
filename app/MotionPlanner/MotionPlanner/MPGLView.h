//
//  MPGLView.h
//  MotionPlanner
//
//  Created by John Visentin on 4/4/14.
//  Copyright (c) 2014 Brockenhaus Studio. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import "MPScene.h"

@interface MPGLView : NSOpenGLView

@property (nonatomic, strong) MPScene *scene;

@end
