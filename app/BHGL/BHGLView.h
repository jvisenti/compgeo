//
//  BHGLView.h
//
//  Created by John Visentin on 4/8/14.
//  Copyright (c) 2014 Brockenhaus Studio. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import "BHGLScene.h"


@interface BHGLView : NSOpenGLView

@property (nonatomic, strong) BHGLScene *scene;

- (void)updateGL:(NSTimeInterval)dt;

@end
