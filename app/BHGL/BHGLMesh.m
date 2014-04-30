//
//  BHGLMesh.m
//
//  Created by John Visentin on 10/11/13.
//  Copyright (c) 2013 Brockenhaus Studio. All rights reserved.
//

#import "BHGLMesh.h"
#import "BHGLCUtils.h"

@implementation BHGLMesh

- (id)initWithVertexData:(const GLvoid *)vertexData vertexDataSize:(GLsizeiptr)vertexDataSize vertexType:(BHGLVertexType *)vertexType indexData:(const GLvoid *)indexData indexDataSize:(GLsizeiptr)indexDataSize indexType:(GLenum)indexType
{
    if ((self = [self initWithVertexData:vertexData vertexDataSize:vertexDataSize vertexType:vertexType]))
    {
        BHGLSaveIndexBuffer();
        
        _bufferSet.indexBuffer = BHGLGenerateIndexBuffer(indexDataSize, (GLuint *)indexData, GL_STATIC_DRAW);
        
        BHGLRestoreIndexBuffer();
        
        size_t indexSize;
        
        switch (indexType)
        {
            case GL_UNSIGNED_BYTE:
                indexSize = sizeof(GLubyte);
                break;
                
            case GL_UNSIGNED_SHORT:
                indexSize = sizeof(GLushort);
                break;
                
            default:
                indexSize = sizeof(GLuint);
                break;
        }
        
        _indexType = indexType;
        _indexCount = (GLuint)indexDataSize / indexSize;
        _indexData = indexData;
        
        BHGLError();
    }
    return self;
}

- (id)initWithVertexData:(const GLvoid *)vertexData vertexDataSize:(GLsizeiptr)vertexDataSize vertexType:(BHGLVertexType *)vertexType
{
    if ((self = [super init]))
    {        
        BHGLSaveVertexArray();
        BHGLSaveVertexBuffer();
        
        _vertexArray = BHGLGenerateVertexArray();

        _bufferSet.vertexBuffer = BHGLGenerateVertexBuffer(vertexDataSize, (GLvoid *)vertexData, GL_STATIC_DRAW);

        BHGLVertexAttribInfo *attribs = BHGLVertexAttributesForVertexType(vertexType);
        for (int i = 0; i < vertexType->numAttribs; i++)
        {
            BHGLVertexAttribInfo info = attribs[i];
            glEnableVertexAttribArray(info.index);
            glVertexAttribPointer(info.index, info.length, info.type, info.normalized, info.stride, info.ptr);
        }
        free(attribs);
        
        BHGLRestoreVertexArray();
        BHGLRestoreVertexBuffer();
        
        BHGLVertexTypeCopy(&_vertexType, vertexType);
        
        _vertexCount = (GLuint)vertexDataSize / vertexType->stride;
        _vertexData = vertexData;
        
        self.primitiveMode = GL_TRIANGLES;
        
        BHGLError();
    }

    return self;
}

- (void)dealloc
{
    BHGLVertexTypeFree(_vertexType);
//    glDeleteBuffers(2, &_bufferSet.vertexBuffer);
}

#pragma mark - BHGLRenderedObject

- (void)render
{
    BHGLBindVertexArray(self.vertexArray);
    BHGLBindBufferSet(self.bufferSet);
        
    if (self.indexCount == 0)
    {
        glDrawArrays(self.primitiveMode, 0, self.vertexCount);
    }
    else
    {
        glDrawElements(self.primitiveMode, self.indexCount, self.indexType, 0);
    }
    
    BHGLBindDefault();
}

- (void)renderWithProgram:(BHGLProgram *)program
{
    [program prepareToDraw];
    
    [self render];
}

- (void)update:(NSTimeInterval)dt {}

@end
