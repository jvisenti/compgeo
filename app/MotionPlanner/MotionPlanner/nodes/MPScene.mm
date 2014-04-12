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
#import "MPPathNode.h"
#import "MPDijkstra3D.h"
#import "MPUtils.h"

#define kMPPlanStatesPerSec 5

@interface MPScene ()
{
    MP::Environment3D *_environment;
}

@property (nonatomic, assign) MP::Dijkstra3D *planner;
@property (nonatomic, assign) std::vector<MP::Transform3D> &plan;

@property (nonatomic, strong) BHGLLight *light;

@property (nonatomic, weak) MPCube *boundingBox;
@property (nonatomic, weak) MPPathNode *pathNode;
@property (nonatomic, weak) MPModelNode *activeObject;

- (void)setupProgram;
- (void)setupCamera;
- (void)setupLights;

- (void)updateBoundingBox;

- (void)cleanup;

@end

@implementation MPScene

@synthesize rootNode = _rootNode;

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

- (void)setEnvironment:(MP::Environment3D *)environment
{
    if (environment != _environment)
    {
        [self.rootNode.children makeObjectsPerformSelector:@selector(removeFromParent)];
        
        MPPathNode *pathNode = [[MPPathNode alloc] init];
        [_rootNode addChild:pathNode];
        self.pathNode = pathNode;
        
        // free old environment/planner
        [self cleanup];
        
        if (environment != nullptr)
        {
            // create active object
            MPModelNode *activeNode = [[MPModelNode alloc] initWithModel:environment->getActiveObject()];
            
            activeNode.material.surfaceColor = BHGLColorRed;
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
                
                obstacleNode.material.surfaceColor = BHGLColorYellow;
                obstacleNode.material.ambientColor = BHGLColorWhite;
                obstacleNode.material.diffuseColor = BHGLColorWhite;
                obstacleNode.material.specularColor = BHGLColorMake(0.6f, 0.6f, 0.6f, 1.0f);
                obstacleNode.material.shininess = 10.0f;
                
                [self addChild:obstacleNode];
            }
            
            self.planner = new MP::Dijkstra3D(environment);
        }
        
        _environment = environment;
        
        [self updateBoundingBox];
    }
}

- (const MP::Environment3D *)getEnvironment
{
    return _environment;
}

- (BOOL)transform:(MP::Transform3D &)transform validForModel:(MP::Model *)model
{
    BOOL valid = YES;
    
    if (_environment != nullptr)
    {
        valid = _environment->isValidForModel(transform, model);
    }
    
    return valid;
}

- (BOOL)planTo:(const MP::Transform3D &)goal
{
    return [self planFrom:self.activeObject.model->getTransform() to:goal];
}

- (BOOL)planFrom:(const MP::Transform3D &)start to:(const MP::Transform3D &)goal
{
    self.plan.clear();
    
    if(!self.planner->plan(start, goal, self.plan))
    {
        printf("failed to find plan from ");
        MPVec3Print(start.getPosition());
        printf(" to ");
        MPVec3Print(goal.getPosition());
        printf("\n");
        
        return NO;
    }
    
    return YES;
}

- (void)executePlan
{
    std::vector<MPVec3> path;
    BHGLKeyframeAnimation *anim = [BHGLKeyframeAnimation animationWithFrames:(int)self.plan.size() fps:kMPPlanStatesPerSec];
    
    for (int i = 0; i < self.plan.size(); ++i)
    {
        MP::Transform3D &transform = self.plan.at(i);
        
        path.push_back(transform.getPosition());
        
        [anim setTranslation:MPVec3ToGLKVector3(transform.getPosition()) forFrame:i];
        [anim setScale:MPVec3ToGLKVector3(transform.getScale()) forFrame:i];
        [anim setRotation:MPQuaternionToGLKQuaternion(transform.getRotation()) forFrame:i];
    }
    
    [self.pathNode setPath:path];
    
    [self.activeObject runAnimation:anim];
}

- (void)animateActiveObject:(BHGLAnimation *)animation
{
    [self.activeObject runAnimation:animation];
}

- (void)removeAnimationFromActiveObject:(BHGLAnimation *)animation
{
    [self.activeObject removeAnimation:animation];
}

#pragma mark - private interface

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

- (void)updateBoundingBox
{
    [self.boundingBox removeFromParent];
    
    MPCube *boundingBox = [[MPCube alloc] init];
    boundingBox.scale = GLKVector3Make(2.0*_environment->getSize().w+0.01, 2.0*_environment->getSize().h+0.01, 2.0*_environment->getSize().d+0.01);
    
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

- (void)cleanup
{
    if (self.planner)
    {
        // TODO: anything else need to be done to free up memory?
        delete self.planner;
    }
    
    if (_environment)
    {
        if (_environment->getActiveObject())
        {
            //            TODO: need to free these somehow
            //            free((void *)_environment->getActiveObject()->getMesh()->vertexData);
            //            free((void *)_environment->getActiveObject()->getMesh()->indexData);
            
            delete _environment->getActiveObject();
        }
        
        const std::vector<MP::Model *> &obstacles = _environment->getObstacles();
        for(std::vector<MP::Model * const>::iterator it = obstacles.begin(); it != obstacles.end(); ++it)
        {
            MP::Model *otherModel = *it;
            
            //            TODO: need to free these somehow
            //            free((void *)otherModel->getMesh()->vertexData);
            //            free((void *)otherModel->getMesh()->indexData);
            
            delete otherModel;
        }
        
        delete _environment;
    }
}

@end
