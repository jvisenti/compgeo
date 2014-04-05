//
//  MPGLView.m
//  MotionPlanner
//
//  Created by John Visentin on 4/4/14.
//  Copyright (c) 2014 Brockenhaus Studio. All rights reserved.
//

#import "MPGLView.h"
#import "BHGL.h"

@interface MPGLView ()

- (void)updateGL:(NSTimer *)timer;

@end

@implementation MPGLView

#pragma mark - public interface

- (void)awakeFromNib
{
    [super awakeFromNib];
    
    NSOpenGLPixelFormatAttribute attrs[] = {
        NSOpenGLPFADoubleBuffer,
        NSOpenGLPFADepthSize, 24,
        // Must specify the 3.2 Core Profile to use OpenGL 3.2
        NSOpenGLPFAOpenGLProfile,
        NSOpenGLProfileVersion3_2Core,
        0
    };
    
    NSOpenGLPixelFormat *pf = [[NSOpenGLPixelFormat alloc] initWithAttributes:attrs];
    
    if (!pf)
    {
        NSLog(@"No OpenGL pixel format");
    }
    
    NSOpenGLContext* context = [[NSOpenGLContext alloc] initWithFormat:pf shareContext:nil];
    
    [self setPixelFormat:pf];
    
    [self setOpenGLContext:context];
}

- (void)viewDidMoveToWindow
{
    if (![self window])
    {
        [self clearGLContext];
        
        [[NSApplication sharedApplication] terminate:self];
    }
}

- (void)prepareOpenGL
{
    self.scene = [[MPScene alloc] init];
    
    NSTimer *timer = [NSTimer timerWithTimeInterval:1.0/60.0 target:self selector:@selector(updateGL:) userInfo:nil repeats:YES];
    [[NSRunLoop currentRunLoop] addTimer:timer forMode:NSRunLoopCommonModes];
    
    glEnable(GL_LINE_SMOOTH);
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
    
    glEnable(GL_POLYGON_SMOOTH);
    glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
    
    glEnable(GL_DEPTH_TEST);
    
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glClearColor(0.6f, 0.8f, 1.0f, 0.0f);
}

/** Simply sets the new viewport */
- (void)reshape
{
    [super reshape];
    
    glViewport(0, 0, self.bounds.size.width, self.bounds.size.height);
}

- (void)drawRect:(NSRect)dirtyRect
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    [self.scene render];
    
    [[self openGLContext] flushBuffer];
}

#pragma mark - private interface

- (void)updateGL:(NSTimer *)timer
{
    if(![[NSApplication sharedApplication] isHidden])
    {
        [self.scene updateRecursive:[timer timeInterval]];
        
        [self setNeedsDisplay:YES];
    }
}

@end
