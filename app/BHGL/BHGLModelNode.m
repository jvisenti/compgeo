//
//  BHGLModelNode.m
//
//  Created by John Visentin on 10/16/13.
//  Copyright (c) 2013 Brockenhaus Studio. All rights reserved.
//

#import "BHGLModelNode.h"

@implementation BHGLModelNode

- (id)initWithMesh:(BHGLMesh *)mesh material:(BHGLMaterial *)material
{
    if ((self = [super init]))
    {
        self.mesh = mesh;
        self.material = material;
    }
    return self;
}

- (BHGLMaterial *)material
{
    if (!_material)
    {
        _material = [[BHGLMaterial alloc] init];
    }
    return _material;
}

- (void)renderWithProgram:(BHGLProgram *)program
{
    [self configureProgram:program];
    [program prepareToDraw];
    
    [self.material bind];
    
    [self.mesh render];
    
    [self.material unbind];
    
    [super renderWithProgram:program];
}

- (void)configureProgram:(BHGLProgram *)program
{
    [super configureProgram:program];
    
    program.projectionMatrix = [self computeProjectionMatrixRecursive:YES];
    program.modelViewMatrix = GLKMatrix4Multiply([self computeViewMatrixRecursive:YES], [self computeModelMatrixRecursive:YES]);    
}

#pragma mark - BHGLRenderedObject

- (void)render
{
    [self configureProgram:self.program];
    [self.program prepareToDraw];
    
    [self.material bind];
    
    [self.mesh render];
    
    [self.material unbind];
    
    [super render];
}

@end
