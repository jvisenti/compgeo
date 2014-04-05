//
//  MPScene.m
//  MotionPlanner
//
//  Created by John Visentin on 4/4/14.
//  Copyright (c) 2014 Brockenhaus Studio. All rights reserved.
//

#import "MPScene.h"
#import "MPModelNode.h"
#import "BHGL.h"

#import "MPCube.h"

@interface MPScene ()

- (void)setupProgram;
- (void)setupCamera;
- (void)setupLights;

@end

@implementation MPScene

#pragma mark - initialization

- (id)init
{
    if ((self = [super init]))
    {
        [self setupProgram];
        [self setupCamera];
        [self setupLights];
        
#pragma mark - TESTING ONLY -- REMOVE
{
        MPMesh *mesh = MPMeshCreate((const MPVec3 *)CubeVertices, sizeof(CubeVertices[0]), sizeof(CubeVertices) / sizeof(CubeVertices[0]), (const void *)CubeIndices, sizeof(CubeIndices[0]), sizeof(CubeIndices) / sizeof(CubeIndices[0]));
        
        MP::Model model(mesh);
        model.setPosition(MPVec3Make(0.0f, 0.0f, -5.0f));
        
        MPModelNode *node = [[MPModelNode alloc] initWithModel:model];
        
        node.material.surfaceColor = BHGLColorRed;
        node.material.ambientColor = BHGLColorWhite;
        node.material.diffuseColor = BHGLColorWhite;
        node.material.specularColor = BHGLColorMake(0.6f, 0.6f, 0.6f, 1.0f);
        node.material.shininess = 10.0f;
        
        BHGLBasicAnimation *rotate = [BHGLBasicAnimation rotateBy:GLKQuaternionMakeWithAngleAndAxis(M_PI, 0.0f, 1.0f, 0.0f) withDuration:2.0f];
        rotate.repeats = YES;
        
        [node runAnimation:rotate];
        
        [self addChild:node];
    
        self.activeObject = node;
}
    }
    return self;
}

- (void)setupProgram
{
    BHGLProgram *program = [[BHGLProgram alloc] initWithVertexShaderNamed:@"lighting.vsh" fragmentShaderNamed:@"lighting.fsh" mvpUniform:kBHGLMVPUniformName];
    
    [program setVertexAttribute:BHGLVertexAttribPosition forName:kBHGLPositionAttributeName];
    [program setVertexAttribute:BHGLVertexAttribNormal forName:kBHGLNormalAttributeName];
    
    if ([program link])
    {
        self.program = program;
    }
}

- (void)setupCamera
{
    // TODO: decide on projective or orthographic camera
    [self addCamera:[[BHGLCamera alloc] initWithFieldOfView:GLKMathDegreesToRadians(35) aspectRatio:1.0 nearClippingPlane:0.01 farClippingPlane:10]];
    
    // uncomment for ortho camera
    // [self addCamera:[[BHGLCamera alloc] initWithLeft:-2.0f right:2.0f top:2.0f bottom:-2.0f nearClippingPlane:0.1f farClippingPlane:10.0f]];
}

- (void)setupLights
{
    BHGLLight *light = [[BHGLLight alloc] init];
    light.type = BHGLLightTypePoint;
    light.ambientColor = BHGLColorMake(0.6f, 0.6f, 0.6f, 1.0f);
    light.color = BHGLColorWhite;
    light.position = GLKVector3Make(1.0f, 2.0f, -2.0f);
    light.constantAttenuation = 0.6f;
    light.linearAttenuation = 0.02f;
    light.quadraticAttenuation = 0.08f;
    
    [self addLight:light];
    
    self.lightUniform = @"u_Lights";
}

@end
