//
//  MPModelNode.m
//  MotionPlanner
//
//  Created by John Visentin on 4/4/14.
//  Copyright (c) 2014 Brockenhaus Studio. All rights reserved.
//

#import "MPModelNode.h"
#import "ConversionUtils.h"
#import "BHGLCUtils.h"

@interface MPModelNode ()
{
    MP::Model _model;
}

@end

@implementation MPModelNode

#pragma mark - overrides

- (void)setPosition:(GLKVector3)position
{
    _model.setPosition(GLKVector3ToMPVec3(position));
}

- (GLKVector3)position
{
    return MPVec3ToGLKVector3(self.model.getPosition());
}

- (void)setScale:(GLKVector3)scale
{
    _model.setScale(GLKVector3ToMPVec3(scale));
}

- (GLKVector3)scale
{
    return MPVec3ToGLKVector3(self.model.getScale());
}

- (void)setRotation:(GLKQuaternion)rotation
{
    _model.setRotation(GLKQuaternionToMPQuaternion(rotation));
}

- (GLKQuaternion)rotation
{
    return MPQuaternionToGLKQuaternion(self.model.getRotation());
}

- (GLKMatrix4)modelMatrix
{
    return MPMat4ToGLKMatrix4(_model.getModelMatrix());
}

- (void)setMesh:(BHGLMesh *)mesh
{
    // does nothing. set new model to change the mesh.
}

- (void)setModel:(const MP::Model &)model
{
    _model = model;
    
    MPMesh *mesh = _model.getMesh();
    
    BHGLVertexType vType = BHGLVertexTypeCreate(2);
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
    
    [super setMesh:[[BHGLMesh alloc] initWithVertexData:(const GLvoid *)mesh->vertexData vertexDataSize:(mesh->numVertices * mesh->stride) vertexType:&vType indexData:(const GLvoid *)mesh->indexData indexDataSize:(mesh->numIndices * mesh->indexSize) indexType:GL_UNSIGNED_BYTE]];
    
    BHGLVertexTypeFree(vType);
}

- (const MP::Model &)model
{
    return _model;
}

#pragma mark - public interface

- (id)initWithModel:(const MP::Model &)model
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
