//
//  BHDCGLView.m
//
//  Created by John Visentin on 1/23/14.
//  Copyright (c) 2014 Brockenhaus Studio. All rights reserved.
//

#import <QuartzCore/QuartzCore.h>
#import "BHGLView.h"

@interface BHGLView ()
{
    CVDisplayLinkRef _displayLink;
    int64_t _lastUpdate;
}

- (CVReturn)getFrameForTime:(const CVTimeStamp *)outputTime;

@end

static CVReturn DisplayLinkCallback(CVDisplayLinkRef displayLink, const CVTimeStamp *now, const CVTimeStamp *outputTime, CVOptionFlags flagsIn, CVOptionFlags *flagsOut, void *displayLinkContext);

@implementation BHGLView

/* from Apple's GLEssentials sample project */
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
    
    NSOpenGLContext *context = [[NSOpenGLContext alloc] initWithFormat:pf shareContext:nil];
    
    [self setPixelFormat:pf];
    
    [self setOpenGLContext:context];
}

- (void)prepareOpenGL
{
    // vertical sync
    GLint swapInt = 1;
    [[self openGLContext] setValues:&swapInt forParameter:NSOpenGLCPSwapInterval];
    
    // Create a display link capable of being used with all active displays
    CVDisplayLinkCreateWithActiveCGDisplays(&_displayLink);
    
    // Set the renderer output callback function
    CVDisplayLinkSetOutputCallback(_displayLink, &DisplayLinkCallback, (__bridge void*)self);
    
    // Set the display link for the current renderer
    CGLContextObj cglContext = [[self openGLContext] CGLContextObj];
    CGLPixelFormatObj cglPixelFormat = [[self pixelFormat] CGLPixelFormatObj];
    CVDisplayLinkSetCurrentCGDisplayFromOpenGLContext(_displayLink, cglContext, cglPixelFormat);
    
    // Activate the display link
    CVDisplayLinkStart(_displayLink);
}

- (void)dealloc
{
    CVDisplayLinkStop(_displayLink);
    CVDisplayLinkRelease(_displayLink);
}

- (void)updateGL:(NSTimeInterval)dt
{
    if(![[NSApplication sharedApplication] isHidden])
    {
        [self.scene updateRecursive:dt];
        
        [self setNeedsDisplay:YES];
    }
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
    
    [self.openGLContext flushBuffer];
}

#pragma mark - CVDisplayLink methods

- (CVReturn)getFrameForTime:(const CVTimeStamp *)outputTime
{
    if (_lastUpdate)
    {
        int64_t dt = (outputTime->hostTime - _lastUpdate) / CLOCKS_PER_SEC;
        
        [self updateGL:dt / 1000.0];
    }
    
    _lastUpdate = outputTime->hostTime;
    
    return kCVReturnSuccess;
}

// This is the renderer output callback function
static CVReturn DisplayLinkCallback(CVDisplayLinkRef displayLink, const CVTimeStamp *now, const CVTimeStamp *outputTime, CVOptionFlags flagsIn, CVOptionFlags *flagsOut, void *displayLinkContext)
{
    CVReturn result = [(__bridge id)displayLinkContext getFrameForTime:outputTime];
    return result;
}

@end