//
//  BHGLScene.m
//
//  Created by John Visentin on 10/19/13.
//  Copyright (c) 2013 Brockenhaus Studio. All rights reserved.
//

#import "BHGLScene.h"

@interface BHGLScene ()

@property (nonatomic, strong) NSMutableArray *mutableCameras;
@property (nonatomic, strong) NSMutableArray *mutableLights;

@end

@implementation BHGLScene

#pragma mark - property overrides

- (NSArray *)cameras
{
    return [self.mutableCameras copy];
}

- (NSArray *)lights
{
    return [self.mutableLights copy];
}

- (void)setLights:(NSArray *)lights
{
    self.mutableLights = [lights mutableCopy];
}

- (GLKMatrix4)viewMatrix
{
    GLKMatrix4 viewMatrix = self.activeCamera ? self.activeCamera.viewMatrix : GLKMatrix4Identity;
    
    return viewMatrix;
}

- (GLKMatrix4)projectionMatrix
{
    GLKMatrix4 projectionMatrix = self.activeCamera ? self.activeCamera.projectionMatrix : GLKMatrix4Identity;
    
    return projectionMatrix;
}

#pragma mark public interface

- (void)setActiveCamera:(BHGLCamera *)activeCamera
{
    if (!activeCamera || [self.cameras containsObject:activeCamera])
    {
        _activeCamera = activeCamera;
    }
}

- (void)addCamera:(BHGLCamera *)camera
{
    [self.mutableCameras addObject:camera];
    
    if (!self.activeCamera)
    {
        _activeCamera = camera;
    }
}

- (void)removeCamera:(BHGLCamera *)camera
{
    if (self.activeCamera == camera)
    {
        _activeCamera = nil;
    }
    
    [self.mutableCameras removeObject:camera];
}

- (BHGLCamera *)cameraNamed:(NSString *)name
{
    __block BHGLCamera *c = nil;
    [self.cameras enumerateObjectsUsingBlock:^(BHGLCamera *cam, NSUInteger idx, BOOL *stop)
     {
         if ([cam.name isEqualToString:name])
         {
             c = cam;
             *stop = YES;
         }
     }];
    
    return c;
}

- (void)addLight:(BHGLLight *)light
{
    [self.mutableLights addObject:light];
}

- (void)removeLight:(BHGLLight *)light
{
    [self.mutableLights removeObject:light];
}

- (BHGLLight *)lightNamed:(NSString *)name
{
    __block BHGLLight *l = nil;
    [self.cameras enumerateObjectsUsingBlock:^(BHGLLight *light, NSUInteger idx, BOOL *stop)
     {
         if ([light.name isEqualToString:name])
         {
             l = light;
             *stop = YES;
         }
     }];
    
    return l;
}

- (void)configureProgram:(BHGLProgram *)program
{
    [super configureProgram:program];
    
    if (self.lightUniform)
    {
        NSUInteger numLights = [self.mutableLights count];
        GLKMatrix4 camMatrix = self.viewMatrix;
        
        BHGLLightInfo *lights = malloc(numLights * sizeof(BHGLLightInfo));
        
        for (int i = 0; i < numLights; i++)
        {
            BHGLLight *light = [self.mutableLights objectAtIndex:i];
            GLKVector3 lightPos = GLKMatrix4MultiplyVector3(camMatrix, light.position); // transform to eye space
            
            lights[i] = [light lightInfo];
            memcpy(lights[i].position, lightPos.v, 3*sizeof(GLfloat));
            
            // TODO: compute half vector for directional lights
            if (light.type == BHGLLightTypeDirectional)
            {
                GLKVector3 halfVector = GLKVector3Add(GLKVector3Make(0.0f, 0.0f, 1.0f), lightPos);
                halfVector = GLKVector3Normalize(halfVector);
                memcpy(lights[i].halfVector, halfVector.v, 3*sizeof(GLfloat));
            }
        }
        
        [program setUniform:self.lightUniform withLightInfo:lights num:(int)numLights];
        
        free(lights);
    }
}

#pragma mark - BHGLRenderedObject

- (void)update:(NSTimeInterval)dt
{
    [self.cameras enumerateObjectsUsingBlock:^(BHGLCamera *cam, NSUInteger idx, BOOL *stop)
     {
         [cam update:dt];
     }];
    
    [self.lights enumerateObjectsUsingBlock:^(BHGLLight *light, NSUInteger idx, BOOL *stop) {
        [light update:dt];
    }];
    
    [super update:dt];
}

- (void)render
{
    [self configureProgram:self.program];
    
    [super render];
}

- (void)renderWithProgram:(BHGLProgram *)program
{
    [self configureProgram:program];
    
    [super renderWithProgram:program];
}

#pragma mark - private interface

- (NSMutableArray *)mutableCameras
{
    if (!_mutableCameras)
    {
        _mutableCameras = [NSMutableArray array];
    }
    return _mutableCameras;
}

- (NSMutableArray *)mutableLights
{
    if (!_mutableLights)
    {
        _mutableLights = [NSMutableArray array];
    }
    return _mutableLights;
}

@end
