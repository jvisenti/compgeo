//
//  MPModelNode.m
//  MotionPlanner
//
//  Created by John Visentin on 4/4/14.
//  Copyright (c) 2014 Brockenhaus Studio. All rights reserved.
//

#import "MPModelNode.h"
#import "MPScene.h"
#import "BHGLCUtils.h"

@interface MPModelNode ()
{
    MP::Model *_model;
}

@end

@implementation MPModelNode

@dynamic scene;

#pragma mark - overrides

- (void)setPosition:(GLKVector3)position
{
    if (!_model) return;
    
    MPVec3 mpPos = GLKVector3ToMPVec3(position);
    MP::Transform3D next(mpPos, self.model->getScale(), self.model->getRotation());
    
    if (!self.scene || [self.scene transform:next validForModel:_model])
    {
        _model->setPosition(mpPos);
    }
}

- (GLKVector3)position
{
    if (!_model) return GLKVector3Make(0.0f, 0.0f, 0.0f);
    
    return MPVec3ToGLKVector3(self.model->getPosition());
}

- (void)setScale:(GLKVector3)scale
{
    if (!_model) return;
    
    MPVec3 mpScale = GLKVector3ToMPVec3(scale);
    MP::Transform3D next(self.model->getPosition(), mpScale, self.model->getRotation());
    
    if (!self.scene || [self.scene transform:next validForModel:_model])
    {
        _model->setScale(mpScale);
    }
}

- (GLKVector3)scale
{
    if (!_model) return GLKVector3Make(1.0f, 1.0f, 1.0f);
    
    return MPVec3ToGLKVector3(self.model->getScale());
}

- (void)setRotation:(GLKQuaternion)rotation
{
    if (!_model) return;
    
    MPQuaternion mpRot = GLKQuaternionToMPQuaternion(rotation);
    MP::Transform3D next(self.model->getPosition(), self.model->getScale(), mpRot);
    
    if (!self.scene || [self.scene transform:next validForModel:_model])
    {
        _model->setRotation(mpRot);
    }    
}

- (GLKQuaternion)rotation
{
    if (!_model) return GLKQuaternionIdentity;
    
    return MPQuaternionToGLKQuaternion(self.model->getRotation());
}

- (GLKMatrix4)modelMatrix
{
    if (!_model) return GLKMatrix4Identity;
    
    return MPMat4ToGLKMatrix4(_model->getModelMatrix());
}

- (void)setMesh:(BHGLMesh *)mesh
{
    // does nothing. set new model to change the mesh.
}

- (void)setModel:(MP::Model *)model
{
    _model = model;
    
    if (!model) return;
    
    MPMesh *mesh = _model->getMesh();
    
    if (mesh)
    {
        BHGLVertexType vType;
        
        if (mesh->texName)
        {
            vType = BHGLVertexTypeCreateWithType(BHGL_NTEXTURE_VERTEX);
            
            NSString *texName = [NSString stringWithUTF8String:mesh->texName];
            self.material.texture = [[BHGLTexture alloc] initWithImageNamed:texName];
        }
        else
        {
            vType = BHGLVertexTypeCreate(2);
            vType.attribs[0] = BHGLVertexAttribPosition;
            vType.types[0] = GL_FLOAT;
            vType.lengths[0] = 3;
            vType.normalized[0] = GL_FALSE;
            vType.offsets[0] = (GLvoid *)0;
            
            // NOTE: this assumes the mesh has normal data, which MPMesh knows nothing about. We should think about this.
            vType.attribs[1] = BHGLVertexAttribNormal;
            vType.types[1] = GL_FLOAT;
            vType.lengths[1] = 3;
            vType.normalized[1] = GL_FALSE;
            vType.offsets[1] = (GLvoid *)(3*sizeof(GLfloat));
            
            vType.stride = (GLsizei)mesh->stride;
        }
        
        [super setMesh:[[BHGLMesh alloc] initWithVertexData:(const GLvoid *)mesh->vertexData vertexDataSize:(mesh->numVertices * mesh->stride) vertexType:&vType indexData:(const GLvoid *)mesh->indexData indexDataSize:(mesh->numIndices * mesh->indexSize) indexType:GL_UNSIGNED_INT]];
        
        BHGLVertexTypeFree(vType);
    }
    else
    {
        [super setMesh:nil];
    }
}

- (MP::Model *)model
{
    return _model;
}

#pragma mark - public interface

- (id)initWithModel:(MP::Model *)model
{
    if ((self = [super init]))
    {
        [self setModel:model];
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
