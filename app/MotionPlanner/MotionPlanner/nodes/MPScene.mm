//
//  MPScene.m
//  MotionPlanner
//
//  Created by John Visentin on 4/4/14.
//  Copyright (c) 2014 Brockenhaus Studio. All rights reserved.
//

#import "MPScene.h"
#import "BHGL.h"

@interface MPScene ()

@property (nonatomic, strong) BHGLLight *light;

- (void)setupProgram;
- (void)setupCamera;
- (void)setupLights;

@end

@implementation MPScene

@synthesize rootNode = _rootNode;

#pragma mark - initialization

- (id)init
{
    if ((self = [super init]))
    {        
        [self setupProgram];
        [self setupCamera];
        [self setupLights];
        
        self.rootNode.position = GLKVector3Make(0.0f, 0.0f, -8.0f);
    }
    return self;
}

- (id)initWithEnvironment:(const MP::Environment3D *)environment
{
    if ((self = [self init]))
    {
        self.environment = environment;
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
    [self addCamera:[[BHGLCamera alloc] initWithFieldOfView:GLKMathDegreesToRadians(35) aspectRatio:1.0 nearClippingPlane:0.01 farClippingPlane:15]];
    
    self.activeCamera.target = self.rootNode;
}

- (void)setupLights
{
    self.light = [[BHGLLight alloc] init];
    self.light.type = BHGLLightTypePoint;
    self.light.ambientColor = BHGLColorMake(0.6f, 0.6f, 0.6f, 1.0f);
    self.light.color = BHGLColorWhite;
    self.light.position = GLKVector3Make(1.0f, 2.0f, -4.0f);
    self.light.constantAttenuation = 0.6f;
    self.light.linearAttenuation = 0.08f;
    self.light.quadraticAttenuation = 0.06f;
    
    [self addLight:self.light];
    
    self.lightUniform = @"u_Lights";
}

#pragma mark - property overrides

- (void)addChild:(BHGLNode *)node
{
    [self.rootNode addChild:node];
}

- (BHGLNode *)rootNode
{
    if (!_rootNode)
    {
        _rootNode = [[BHGLNode alloc] init];
        [super addChild:_rootNode];
    }
    
    return _rootNode;
}

#pragma mark - public interface

- (void)setEnvironment:(const MP::Environment3D *)environment
{
    if (environment != _environment)
    {
        [self.rootNode.children makeObjectsPerformSelector:@selector(removeFromParent)];
        
        if (environment != nullptr)
        {
            MPModelNode *activeNode = [[MPModelNode alloc] initWithModel:environment->getActiveObject()];
            
            activeNode.material.surfaceColor = BHGLColorRed;
            activeNode.material.ambientColor = BHGLColorWhite;
            activeNode.material.diffuseColor = BHGLColorWhite;
            activeNode.material.specularColor = BHGLColorMake(0.6f, 0.6f, 0.6f, 1.0f);
            activeNode.material.shininess = 10.0f;
            
            [self addChild:activeNode];
            
            _activeObject = activeNode;
            
            /* set obstacles */
            const std::vector<MP::Model *> &obstacles = environment->getObstacles();
            for(std::vector<MP::Model * const>::iterator it = obstacles.begin(); it != obstacles.end(); ++it)
            {
                MP::Model *obstacle = *it;
                
                MPModelNode *obstacleNode = [[MPModelNode alloc] initWithModel:obstacle];
                
                obstacleNode.material.surfaceColor = BHGLColorYellow;
                obstacleNode.material.ambientColor = BHGLColorWhite;
                obstacleNode.material.diffuseColor = BHGLColorWhite;
                obstacleNode.material.specularColor = BHGLColorMake(0.6f, 0.6f, 0.6f, 1.0f);
                obstacleNode.material.shininess = 10.0f;
                
                [self addChild:obstacleNode];
            }
        }
        else
        {
            _activeObject = nil;
        }
        
        _environment = environment;
    }
}

- (BOOL)transform:(MP::Transform3D &)transform validForModel:(MP::Model *)model
{
    BOOL valid = YES;
    
    if (self.environment != nullptr)
    {
        const std::vector<MP::Model *> &obstacles = self.environment->getObstacles();
        
        for(std::vector<MP::Model * const>::iterator it = obstacles.begin(); it != obstacles.end(); ++it)
        {
            MP::Model *otherModel = *it;
            
            if (model->wouldCollideWithModel(transform, *otherModel))
            {
                valid = NO;
                break;
            }
        }
    }
    
    return valid;
}

@end
