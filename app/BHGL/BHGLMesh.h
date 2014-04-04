//
//  BHGLMesh.h
//
//  Created by John Visentin on 10/11/13.
//  Copyright (c) 2013 Brockenhaus Studio. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "BHGLTypes.h"
#import "BHGLRenderedObject.h"

@interface BHGLMesh : NSObject <BHGLRenderedObject>

/** An identifier for use by your application. */
@property (nonatomic, copy) NSString *identifier;

@property (nonatomic, readonly) GLuint vertexArray;

/** The primitive mode to use in the glDraw*() function. Can be one of
    GL_POINTS, GL_LINES, GL_LINE_STRIP, GL_LINE_LOOP, GL_TRIANGLES, GL_TRIANGLE_STRIP, GL_TRIANGLE_FAN. 
    Default value is GL_POINTS. */
@property (nonatomic, assign) GLenum primitiveMode;

/* NOTE: not all properties are guaranteed to be set. 0 indicates an unknown value. */
@property (nonatomic, readonly) BHGLVertexType vertexType;
@property (nonatomic, readonly) GLenum indexType;
@property (nonatomic, readonly) BHGLBufferSet bufferSet;
@property (nonatomic, readonly) GLuint vertexCount;
@property (nonatomic, readonly) GLuint indexCount;
@property (nonatomic, readonly) const GLvoid *vertexData;
@property (nonatomic, readonly) const GLvoid *indexData;

/** Initialize a new mesh with the data needed for a glDrawElements call.
    @param vertexData the array of vertices in the mesh
    @param vertexDataSize the size of the entire vertex data array
    @param vertexType information about the type of vertices in the mesh. This parameter is copied into the vertexType property of the mesh
    @param indexData the array of indices
    @param indexDataSize the size of the entire index data array
    @param indexType index data type. Should be one of GL_UNSIGNED_BYTE, GL_UNSIGNED_SHORT, or GL_UNSIGNED_INT */
- (id)initWithVertexData:(const GLvoid *)vertexData vertexDataSize:(GLsizeiptr)vertexDataSize vertexType:(BHGLVertexType *)vertexType indexData:(const GLvoid *)indexData indexDataSize:(GLsizeiptr)indexDataSize indexType:(GLenum)indexType;

/** Initialize a new mesh with the data needed for a glDrawArrays call.
    @param vertexData the array of vertices in the mesh
    @param vertexDataSize the size of the entire vertex data array
    @param vertexType information about the type of vertices in the mesh. This parameter is copied into the vertexType property of the mesh
    @param faceCount the number of faces in the mesh */
- (id)initWithVertexData:(const GLvoid *)vertexData vertexDataSize:(GLsizeiptr)vertexDataSize vertexType:(BHGLVertexType *)vertexType;

@end
