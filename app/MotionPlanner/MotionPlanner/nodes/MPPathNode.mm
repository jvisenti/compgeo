//
//  MPPathNode.m
//  MotionPlanner
//
//  Created by John Visentin on 4/12/14.
//  Copyright (c) 2014 Brockenhaus Studio. All rights reserved.
//

#import "MPPathNode.h"
#import "BHGLCUtils.h"

@interface MPPathNode ()
{
    std::vector<MPVec3> _path;
}

- (void)updateMesh;
- (void)cleanup;

@end

@implementation MPPathNode

- (id)init
{
    if ((self = [super init]))
    {
//        BHGLProgram *program = [[BHGLProgram alloc] initWithVertexShaderNamed:kBHGLBasicVertexShader fragmentShaderNamed:kBHGLBasicFragmentShader mvpUniform:kBHGLMVPUniformName];
//        
//        [program setVertexAttribute:BHGLVertexAttribPosition forName:kBHGLPositionAttributeName];
//        
//        if ([program link])
//        {
//            self.program = program;
//        }
    }
    return self;
}

- (id)initWithPath:(const std::vector<MPVec3> &)path
{
    if ((self = [self init]))
    {
        [self setPath:path];
    }
    return self;
}

- (void)dealloc
{
    [self cleanup];
}

#pragma mark - property overrides

- (void)setMesh:(BHGLMesh *)mesh
{
    // does nothing. mesh generation is done elsewhere.
}

#pragma mark - public interface

- (const std::vector<MPVec3> &)path
{
    return _path;
}

- (void)setPath:(const std::vector<MPVec3> &)path
{
    _path = path;
    
    [self updateMesh];
}

#pragma mark private interface

- (void)updateMesh
{
    [self cleanup];
    
    size_t dataSize = 3* _path.size() * sizeof(GLfloat);
    GLfloat *vertices = (GLfloat *)malloc(dataSize);
    
    for (int i = 0; i < self.path.size(); ++i)
    {
        memcpy(&vertices[3*i], _path.at(i).v, 3 * sizeof(GLfloat));
    }
    
    BHGLVertexType vType = BHGLVertexTypeCreate(1);
    vType.attribs[0] = BHGLVertexAttribPosition;
    vType.types[0] = GL_FLOAT;
    vType.lengths[0] = 3;
    vType.normalized[0] = GL_FALSE;
    vType.offsets[0] = (GLvoid *)0;
    vType.stride = 3 * sizeof(GLfloat);
    
    BHGLMesh *mesh = [[BHGLMesh alloc] initWithVertexData:(const GLvoid *)vertices vertexDataSize:dataSize vertexType:&vType];
    mesh.primitiveMode = GL_LINE_STRIP;
    
    BHGLVertexTypeFree(vType);
    
    [super setMesh:mesh];
}

- (void)cleanup
{
    if (self.mesh)
    {
        // vertex data allocd locally, so node is responsible for freeing it
        free((void *)self.mesh.vertexData);
    }
}

@end
