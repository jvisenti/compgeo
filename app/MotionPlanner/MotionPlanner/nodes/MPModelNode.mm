//
//  MPModelNode.m
//  MotionPlanner
//
//  Created by John Visentin on 4/4/14.
//  Copyright (c) 2014 Brockenhaus Studio. All rights reserved.
//

#import "MPModelNode.h"
#import "ConversionUtils.h"
#import "BHGLPVRExt.h"

@implementation MPModelNode

#pragma mark - overrides

- (void)setPosition:(GLKVector3)position
{
    self.model.setPosition(GLKVector3ToMPVec3(position));
}

- (GLKVector3)position
{
    return MPVec3ToGLKVector3(self.model.getPosition());
}

- (void)setScale:(GLKVector3)scale
{
    self.model.setScale(GLKVector3ToMPVec3(scale));
}

- (GLKVector3)scale
{
    return MPVec3ToGLKVector3(self.model.getScale());
}

- (void)setRotation:(GLKQuaternion)rotation
{
    self.model.setRotation(GLKQuaternionToMPQuaternion(rotation));
}

- (GLKQuaternion)rotation
{
    return MPQuaternionToGLKQuaternion(self.model.getRotation());
}

- (GLKMatrix4)modelMatrix
{
    return MPMat4ToGLKMatrix4(self.model.getModelMatrix());
}

#pragma mark - public interface

- (id)initWithPODFileNamed:(NSString *)fileName
{
    if ((self = [super init]))
    {
        BHGLNode *rootNode = [BHGLScene rootNodeFromPODFileNamed:@"Sphere" nodeClass:[MPModelNode class]];
        
        if ([rootNode.children count])
        {
            BHGLModelNode *sphere = [rootNode.children objectAtIndex:0];
            
            self.name = sphere.name;
            self.mesh = sphere.mesh;
            self.material = sphere.material;
            
            for (BHGLNode *node in sphere.children)
            {
                [self addChild:node];
            }
        }
    }
    
    return self;
}

- (void)configureProgram:(BHGLProgram *)program
{
    [super configureProgram:program];
    
    [program setUniform:@"u_Material" withMaterial:self.material];
    
    // can use modelView instead if only uniform scaling occurs
    GLKMatrix3 normalMatrix = GLKMatrix3InvertAndTranspose(GLKMatrix4GetMatrix3(program.modelViewMatrix), NULL);
    glUniformMatrix3fv([program uniformPosition:@"u_NormalMatrix"], 1, GL_FALSE, normalMatrix.m);
    
    glUniformMatrix4fv([program uniformPosition:@"u_MVMatrix"], 1, GL_FALSE, program.modelViewMatrix.m);
}

@end
