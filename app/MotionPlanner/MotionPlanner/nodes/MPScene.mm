//
//  MPScene.m
//  MotionPlanner
//
//  Created by John Visentin on 4/4/14.
//  Copyright (c) 2014 Brockenhaus Studio. All rights reserved.
//

#import "MPScene.h"
#import "MPModelNode.h"
#import "BHGLPVRExt.h"

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
        
        // TODO: remove this, testing only
        CPVRTModelPOD scene;

        NSString *path = [[NSBundle mainBundle] pathForResource:@"Sphere" ofType:@"pod"];
        if (scene.ReadFromFile([path UTF8String]) == PVR_SUCCESS)
        {
            BHGLMesh *podMesh = [[BHGLMesh alloc] initWithSPODMesh:scene.pMesh[0]];
            
            MPMesh *mesh = MPMeshCreate((const MPVec3 *)podMesh.vertexData, (size_t)podMesh.vertexType.stride, (size_t)podMesh.vertexCount, (const void *)podMesh.indexData, sizeof(GL_UNSIGNED_INT), (size_t)podMesh.indexCount);
            
            MPModel model;
            model.setMesh(mesh);
            model.setPosition(MPVec3Make(0.0f, 0.0f, -5.0f));
            
            MPModelNode *modelNode = [[MPModelNode alloc] init];
            modelNode.model = model;
            modelNode.mesh = podMesh;
            
            modelNode.material.surfaceColor = BHGLColorRed;
            modelNode.material.ambientColor = BHGLColorWhite;
            modelNode.material.diffuseColor = BHGLColorWhite;
            modelNode.material.specularColor = BHGLColorWhite;
            modelNode.material.shininess = 20.0f;
            
            [self addChild:modelNode];
        }
        // end remove
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
    [self addCamera:[[BHGLCamera alloc] initWithFieldOfView:GLKMathDegreesToRadians(40) aspectRatio:1.0 nearClippingPlane:0.01 farClippingPlane:20]];
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
