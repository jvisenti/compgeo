//
//  MPScene.m
//  MotionPlanner
//
//  Created by John Visentin on 4/4/14.
//  Copyright (c) 2014 Brockenhaus Studio. All rights reserved.
//

#import "MPScene.h"
#import "BHGL.h"

#import "MPCube.h"
#import "MPPyramid.h"

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
        
        MP::Model *cube = new MP::Model(mesh);
        cube->setPosition(MPVec3Make(0.0f, 0.0f, -8.0f));
        
        MPModelNode *activeNode = [[MPModelNode alloc] initWithModel:cube];
        
        activeNode.material.surfaceColor = BHGLColorRed;
        activeNode.material.ambientColor = BHGLColorWhite;
        activeNode.material.diffuseColor = BHGLColorWhite;
        activeNode.material.specularColor = BHGLColorMake(0.6f, 0.6f, 0.6f, 1.0f);
        activeNode.material.shininess = 10.0f;
        
        BHGLBasicAnimation *rotate = [BHGLBasicAnimation rotateBy:GLKQuaternionMakeWithAngleAndAxis(M_PI, 0.0f, 1.0f, 0.0f) withDuration:2.0f];
        rotate.repeats = YES;
        
        [activeNode runAnimation:rotate];
        
        [self addChild:activeNode];
    
        self.activeObject = activeNode;
    
        MP::Model *pyramid = new MP::Model(mesh);
        pyramid->setPosition(MPVec3Make(3.0f, 0.0f, -8.0f));
        
        MPModelNode *pyramidNode = [[MPModelNode alloc] initWithModel:pyramid];
        
        pyramidNode.material.surfaceColor = BHGLColorYellow;
        pyramidNode.material.ambientColor = BHGLColorWhite;
        pyramidNode.material.diffuseColor = BHGLColorWhite;
        pyramidNode.material.specularColor = BHGLColorMake(0.6f, 0.6f, 0.6f, 1.0f);
        pyramidNode.material.shininess = 10.0f;
        
        [self addChild:pyramidNode];
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
    light.position = GLKVector3Make(1.0f, 2.0f, -4.0f);
    light.constantAttenuation = 0.6f;
    light.linearAttenuation = 0.02f;
    light.quadraticAttenuation = 0.08f;
    
    [self addLight:light];
    
    self.lightUniform = @"u_Lights";
}

#pragma mark - public interface

- (void)addChild:(BHGLNode *)node
{
    [super addChild:node];
    
    if ([node isKindOfClass:[MPModelNode class]])
    {
        ((MPModelNode *)node).scene = self;
    }
}

- (BOOL)transform:(MP::Transform3D &)transform validForModel:(MPModelNode *)model
{
    __block BOOL valid = YES;
    
    [self.children enumerateObjectsUsingBlock:^(BHGLNode *child, NSUInteger idx, BOOL *stop) {
        if (child != model && [child isKindOfClass:[MPModelNode class]])
        {
            MP::Model *otherModel = ((MPModelNode *)child).model;
            if (model.model->wouldCollideWithModel(transform, *otherModel))
            {
                valid = NO;
                *stop = YES;
            }
        }
    }];
    
    return valid;
}

@end