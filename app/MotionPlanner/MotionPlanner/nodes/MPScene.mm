//
//  MPScene.m
//  MotionPlanner
//
//  Created by John Visentin on 4/4/14.
//  Copyright (c) 2014 Brockenhaus Studio. All rights reserved.
//

#import "MPScene.h"
#import "MPCube.h"

#define kMPSceneMaxSize 4.0f

@interface MPScene ()
{
    __weak BHGLNode *_rootNode;
    __weak MPCube *_boundingBox;
    __weak MPModelNode *_activeObject;
    __weak MPModelNode *_shadow;
}

- (void)setupProgram;
- (void)setupCamera;
- (void)setupLights;

- (void)updateBoundingBox;
- (void)updateShadow;

@end

@implementation MPScene

@synthesize rootNode = _rootNode;
@synthesize boundingBox = _boundingBox;
@synthesize activeObject = _activeObject;
@synthesize shadow = _shadow;

- (id)init
{
    if ((self = [super init]))
    {
        [self setupProgram];
        [self setupCamera];
        [self setupLights];
    }
    return self;
}

- (id)initWithEnvironment:(MP::Environment3D *)environment
{
    if ((self = [self init]))
    {
        _environment = environment;
    }
    return self;
}

- (void)dealloc
{
    [self cleanup];
}

#pragma mark - property overrides

- (void)addChild:(BHGLNode *)node
{
    [self.rootNode addChild:node];
}

- (void)insertChild:(BHGLNode *)node atIndex:(NSUInteger)index
{
    [self.rootNode insertChild:node atIndex:index];
}

- (BHGLNode *)rootNode
{
    if (!_rootNode)
    {
        BHGLNode *root = [[BHGLNode alloc] init];
        [super addChild:root];
        _rootNode = root;
    }
    
    return _rootNode;
}

- (void)setEnvironment:(MP::Environment3D *)environment
{
    [self.rootNode.children makeObjectsPerformSelector:@selector(removeFromParent)];
    self.rootNode.position = GLKVector3Make(0.0f, 0.0f, 0.0f);
    self.rootNode.scale = GLKVector3Make(1.0f, 1.0f, 1.0f);
    self.rootNode.rotation = GLKQuaternionIdentity;
    
    // free old environment/planner
    [self cleanup];
    
    if (environment != nullptr)
    {
        // create active object
        MPModelNode *activeNode = [[MPModelNode alloc] initWithModel:environment->getActiveObject()];
        
        activeNode.material.surfaceColor = BHGLColorOrange;
        activeNode.material.ambientColor = BHGLColorWhite;
        activeNode.material.diffuseColor = BHGLColorWhite;
        activeNode.material.specularColor = BHGLColorMake(0.6f, 0.6f, 0.6f, 1.0f);
        activeNode.material.shininess = 10.0f;
        
        [self addChild:activeNode];
        
        self.activeObject = activeNode;
        
        // create obstacles
        const std::vector<MP::Model *> &obstacles = environment->getObstacles();
        for(auto it = obstacles.begin(); it != obstacles.end(); ++it)
        {
            MP::Model *obstacle = *it;
            
            MPModelNode *obstacleNode = [[MPModelNode alloc] initWithModel:obstacle];
            
            obstacleNode.material.surfaceColor = obstacleNode.material.texture ? BHGLColorBlack : BHGLColorTeal;
            obstacleNode.material.ambientColor = BHGLColorWhite;
            obstacleNode.material.diffuseColor = BHGLColorWhite;
            obstacleNode.material.specularColor = BHGLColorMake(0.6f, 0.6f, 0.6f, 1.0f);
            obstacleNode.material.shininess = 10.0f;
            
            [self addChild:obstacleNode];
        }
    }
    
    _environment = environment;
    
    [self updateShadow];
    [self updateBoundingBox];
    
    float maxAxis = fmaxf(fmaxf(environment->getSize().w, environment->getSize().h), environment->getSize().d);
    float scale = kMPSceneMaxSize / maxAxis;
    
    self.rootNode.scale = GLKVector3Make(scale, scale, scale);
}

#pragma mark - public interface

- (void)configureProgram:(BHGLProgram *)program
{
    [super configureProgram:program];
    
    GLKVector3 eye = GLKVector3Make(0.0f, 0.0f, 1.0f);
    
    glUniform3fv([self.program uniformPosition:@"u_EyeDirection"], 1, eye.v);
}

- (BOOL)transform:(MP::Transform3D &)transform validForModel:(MP::Model *)model
{
    BOOL valid = YES;
    
    if (_environment != nullptr)
    {        
        if (model == self.shadow.model)
        {
            valid = _environment->inBoundsForModel(transform, self.shadow.model);
        }
        else
        {
            valid = _environment->isValidForModel(transform, model);
        }
    }
    
    return valid;
}

#pragma mark - private interface

- (void)setupProgram
{
    BHGLProgram *program = [[BHGLProgram alloc] initWithVertexShaderNamed:@"lighting.vsh" fragmentShaderNamed:@"lighting.fsh" mvpUniform:kBHGLMVPUniformName];
    
    [program setVertexAttribute:BHGLVertexAttribPosition forName:kBHGLPositionAttributeName];
    [program setVertexAttribute:BHGLVertexAttribNormal forName:kBHGLNormalAttributeName];
    [program setVertexAttribute:BHGLVertexAttribTexCoord0 forName:kBHGLTexCoord0AttributeName];
    
    if ([program link])
    {
        self.program = program;
    }
}

- (void)setupCamera
{
    [self addCamera:[[BHGLCamera alloc] initWithFieldOfView:GLKMathDegreesToRadians(35) aspectRatio:1.0 nearClippingPlane:0.01 farClippingPlane:40.0]];
    
    self.activeCamera.position = GLKVector3Make(0.0f, 2.0f, 10.0f);
    self.activeCamera.target = self.rootNode;
}

- (void)setupLights
{
    BHGLLight *light = [[BHGLLight alloc] init];
    light.type = BHGLLightTypePoint;
    light.ambientColor = BHGLColorMake(0.8f, 0.8f, 0.8f, 1.0f);
    light.color = BHGLColorWhite;
    light.position = GLKVector3Make(1.0f, 2.0f, 2.0f);
    light.constantAttenuation = 0.0f;
    light.linearAttenuation = 0.02f;
    light.quadraticAttenuation = 0.017f;
    
    [self addLight:light];
    
    self.lightUniform = @"u_Lights";
}

- (void)updateBoundingBox
{
    [self.boundingBox removeFromParent];
    
    MPCube *boundingBox = [[MPCube alloc] init];
    boundingBox.scale = GLKVector3Make(_environment->getSize().w+0.01, _environment->getSize().h+0.01, _environment->getSize().d+0.01);
    
    boundingBox.material.surfaceColor = BHGLColorMake(1.0f, 1.0f, 1.0f, 0.1f);
    boundingBox.material.ambientColor = BHGLColorWhite;
    boundingBox.material.diffuseColor = BHGLColorWhite;
    boundingBox.material.specularColor = BHGLColorMake(0.6f, 0.6f, 0.6f, 1.0f);
    boundingBox.material.blendEnabled = GL_TRUE;
    boundingBox.material.blendSrcRGB = GL_SRC_ALPHA;
    boundingBox.material.blendDestRGB = GL_ONE_MINUS_SRC_ALPHA;
    
    [self addChild:boundingBox];
    
    self.boundingBox = boundingBox;
}

- (void)updateShadow
{
    [self.shadow removeFromParent];
    
    MP::Model *shadowModel = new MP::Model(self.activeObject.model->getMesh());
    shadowModel->setTransform(self.activeObject.model->getTransform());
    
    MPModelNode *shadow = [[MPModelNode alloc] initWithModel:shadowModel];
    
    BHGLColor shadowColor = self.activeObject.material.surfaceColor;
    shadowColor.a = 0.2f;
    
    shadow.material.surfaceColor = shadowColor;
    shadow.material.emissionColor = shadow.material.surfaceColor;
    shadow.material.ambientColor = BHGLColorWhite;
    shadow.material.diffuseColor = BHGLColorWhite;
    shadow.material.specularColor = BHGLColorMake(0.6f, 0.6f, 0.6f, 1.0f);
    shadow.material.blendEnabled = GL_TRUE;
    shadow.material.blendSrcRGB = GL_SRC_ALPHA;
    shadow.material.blendDestRGB = GL_ONE_MINUS_SRC_ALPHA;
    
    [self addChild:shadow];
    
    _shadow = shadow;
}

- (void)cleanup
{    
    if (self.environment)
    {
        if (self.environment->getActiveObject())
        {
            
            if (MPMeshGetRefCount(self.environment->getActiveObject()->getMesh()) <= 1)
            {
                // free allocated vertex and index data if we're about to free the mesh
                free((void *)_environment->getActiveObject()->getMesh()->vertexData);
                free((void *)_environment->getActiveObject()->getMesh()->indexData);
            }
            
            delete self.environment->getActiveObject();
        }
        
        const std::vector<MP::Model *> &obstacles = _environment->getObstacles();
        for(std::vector<MP::Model * const>::iterator it = obstacles.begin(); it != obstacles.end(); ++it)
        {
            MP::Model *otherModel = *it;
            
            if (MPMeshGetRefCount((*it)->getMesh()) <= 1)
            {
                // free allocated vertex and index data if we're about to free the mesh
                free((void *)(*it)->getMesh()->vertexData);
                free((void *)(*it)->getMesh()->indexData);
            }
            
            delete otherModel;
        }
        
        delete self.environment;
    }
}

@end
