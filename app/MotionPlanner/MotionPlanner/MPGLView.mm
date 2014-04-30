//
//  MPGLView.m
//  MotionPlanner
//
//  Created by John Visentin on 4/4/14.
//  Copyright (c) 2014 Brockenhaus Studio. All rights reserved.
//

#import "MPGLView.h"
#import "BHGL.h"
#import "MPReader.h"
#import "MPDijkstra3D.h"
#import <Carbon/Carbon.h>

#define kMPSceneScaleFactor 0.2f

#define kMPObjectMotionIncrement 0.04f

@interface MPGLView ()

@property (nonatomic, strong) NSMutableDictionary *movementAnimations;

@property (nonatomic, weak) IBOutlet NSSlider *xSlider;
@property (nonatomic, weak) IBOutlet NSSlider *ySlider;

- (IBAction)xSliderChanged:(NSSlider *)sender;
- (IBAction)ySliderChanged:(NSSlider *)sender;

- (IBAction)openFile:(NSMenuItem *)sender;

@end

@implementation MPGLView

@dynamic scene;

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
        NSOpenGLPFASupersample,
        NSOpenGLPFASampleBuffers, 1,
        NSOpenGLPFASamples, 4,
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

- (BOOL)acceptsFirstResponder
{
    return YES;
}

- (void)mouseDragged:(NSEvent *)theEvent
{
    CGFloat dx = [theEvent deltaX] / self.bounds.size.width;
    CGFloat dy = [theEvent deltaY] / self.bounds.size.height;
    
    [self.xSlider setFloatValue:[self.xSlider floatValue] + dx];
    [self xSliderChanged:self.xSlider];
    
    [self.ySlider setFloatValue:[self.ySlider floatValue] + dy];
    [self ySliderChanged:self.ySlider];
}

- (void)scrollWheel:(NSEvent *)theEvent
{
    CGFloat dx = [theEvent scrollingDeltaY];
    
    float scale = dx < 0 ? 1.0f - kMPSceneScaleFactor : 1.0f + kMPSceneScaleFactor;
    
    BHGLBasicAnimation *scaleAnim = [BHGLBasicAnimation scaleBy:GLKVector3Make(scale, scale, scale) withDuration:0.1];
    
    [self.scene.rootNode runAnimation:scaleAnim];
}

- (void)keyDown:(NSEvent *)theEvent
{    
    unsigned short keyCode = [theEvent keyCode];
    
    GLKVector3 dp = GLKVector3Make(0.0f, 0.0f, 0.0f);
    
    int key = 0;
    
    switch (keyCode)
    {
        case kVK_LeftArrow:
        case kVK_ANSI_A:
            key = kVK_LeftArrow;
            dp.x -= kMPObjectMotionIncrement;
            break;
            
        case kVK_RightArrow:
        case kVK_ANSI_D:
            key = kVK_RightArrow;
            dp.x += kMPObjectMotionIncrement;
            break;
            
        case kVK_DownArrow:
        case kVK_ANSI_S:
            key = kVK_DownArrow;
            dp.z += kMPObjectMotionIncrement;
            break;
            
        case kVK_UpArrow:
        case kVK_ANSI_W:
            key = kVK_UpArrow;
            dp.z -= kMPObjectMotionIncrement;
            break;
            
        case kVK_Space:
            key = kVK_Space;
            dp.y += kMPObjectMotionIncrement;
            break;
            
        case kVK_ANSI_V:
            key = kVK_ANSI_V;
            dp.y -= kMPObjectMotionIncrement;
            break;
            
        default:
            break;
    }
    
    if (key && ![self.movementAnimations objectForKey:@(key)])
    {
        __weak MPGLView *wself = self;
        BHGLBasicAnimation *trans = [BHGLBasicAnimation transformWithBlock:^(BHGLAnimatedObject *object, NSTimeInterval current, NSTimeInterval duration) {
            
            GLKQuaternion rotateY = GLKQuaternionMakeWithAngleAndAxis(M_PI*[wself.xSlider floatValue], 0.0f, 1.0f, 0.0f);
            
            GLKVector3 t = GLKQuaternionRotateVector3(GLKQuaternionInvert(rotateY), dp);
            object.position = GLKVector3Add(object.position, t);

        } duration:0.0];
        trans.repeats = YES;
        
        [self.scene animateShadow:trans];
        
        [self.movementAnimations setObject:trans forKey:@(key)];
    }
}

- (void)keyUp:(NSEvent *)theEvent
{
    unsigned short keyCode = [theEvent keyCode];

    int key = 0;
    
    switch (keyCode)
    {
        case kVK_LeftArrow:
        case kVK_ANSI_A:
            key = kVK_LeftArrow;
            break;
            
        case kVK_RightArrow:
        case kVK_ANSI_D:
            key = kVK_RightArrow;
            break;
            
        case kVK_DownArrow:
        case kVK_ANSI_S:
            key = kVK_DownArrow;
            break;
            
        case kVK_UpArrow:
        case kVK_ANSI_W:
            key = kVK_UpArrow;
            break;
            
        case kVK_ANSI_V:
            key = kVK_ANSI_V;
            break;
            
        case kVK_Space:
            key = kVK_Space;
            break;
            
        case kVK_ANSI_P:
        {
            dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0), ^{
                if ([self.scene plan])
                {
                    dispatch_async(dispatch_get_main_queue(), ^{
                        [self.scene executePlan];
                    });
                }
            });
        }
            
        default:
            break;
    }
    
    BHGLAnimation *anim = [self.movementAnimations objectForKey:@(key)];
    
    if (anim)
    {
        [self.scene removeAnimationFromShadow:anim];
        [self.movementAnimations removeObjectForKey:@(key)];
    }
}

- (void)prepareOpenGL
{
    [super prepareOpenGL];
    
    self.scene = [[MPScene alloc] init];
        
    glEnable(GL_MULTISAMPLE);
    
    glEnable(GL_LINE_SMOOTH);
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
    
    glEnable(GL_POLYGON_SMOOTH);
    glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
    
    glEnable(GL_DEPTH_TEST);
    
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glClearColor(0.2f, 0.6f, 0.4f, 0.0f);
}

#pragma mark - private interface

- (NSMutableDictionary *)movementAnimations
{
    if (!_movementAnimations)
    {
        _movementAnimations = [NSMutableDictionary dictionaryWithCapacity:6];
    }
    return _movementAnimations;
}

- (IBAction)xSliderChanged:(NSSlider *)sender
{
    GLKQuaternion xRotation = GLKQuaternionMakeWithAngleAndAxis(M_PI*[self.ySlider floatValue], 1.0f, 0.0f, 0.0f);
    GLKQuaternion yRotation = GLKQuaternionMakeWithAngleAndAxis(M_PI*[sender floatValue], 0.0f, 1.0f, 0.0f);
    
    self.scene.rootNode.rotation = GLKQuaternionMultiply(xRotation, yRotation);
}

- (IBAction)ySliderChanged:(NSSlider *)sender
{
    GLKQuaternion xRotation = GLKQuaternionMakeWithAngleAndAxis(M_PI*[sender floatValue], 1.0f, 0.0f, 0.0f);
    GLKQuaternion yRotation = GLKQuaternionMakeWithAngleAndAxis(M_PI*[self.xSlider floatValue], 0.0f, 1.0f, 0.0f);
    
    self.scene.rootNode.rotation = GLKQuaternionMultiply(xRotation, yRotation);
}

- (void)openFile:(NSMenuItem *)sender
{
    NSOpenPanel *openPanel = [NSOpenPanel openPanel];
    
    [openPanel setCanChooseFiles:YES];
    [openPanel setCanChooseDirectories:NO];
    [openPanel setAllowsMultipleSelection:NO];
    
    [openPanel beginWithCompletionHandler:^(NSInteger result) {
        if (result == NSFileHandlingPanelOKButton)
        {
            NSString *chosenFile = [[openPanel URL] path];
            
            std::string filePath = std::string([chosenFile cStringUsingEncoding:NSUTF8StringEncoding]);
            
            MP::Reader reader(filePath);
            MP::Environment3D *environment = reader.generateEnvironment3D();
            
            // TODO: Generate some default actions for the active object
            MP::Model *activeObject = environment->getActiveObject();
            activeObject->setActionSet(MP::Action6D::generate6DActions(environment->getStepSize(), environment->getRotationStepSize()));
//            activeObject->setActionSet(MP::Action6D::generate3DActions(environmnet->getStepSize()));
//            activeObject->setActionSet(MP::Action6D::generateFalconActions(environment->getStepSize(), environment->getRotationStepSize()));
            
            if (environment)
            {                
                [self.xSlider setFloatValue:0.0f];
                [self.ySlider setFloatValue:0.0f];
                
                self.scene.environment = environment;
            }
        }
    }];
}

@end
