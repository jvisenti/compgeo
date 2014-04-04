//
//  BHGLCUtils.h
//
//  Created by John Visentin on 10/11/13.
//  Copyright (c) 2013 Brockenhaus Studio. All rights reserved.
//

#ifndef BHGLCUtils_h
#define BHGLCUtils_h

#ifdef __cplusplus
extern "C" {
#endif
    
#include <stddef.h>
#include "BHGLTypes.h"
    
/* type helpers */
extern BHGLVertexType BHGLVertexTypeCreate(GLubyte numAttribs);
extern BHGLVertexType BHGLVertexTypeCreateWithType(BHGLEnum vType);
extern void BHGLVertexTypeCopy(BHGLVertexType *dest, const BHGLVertexType *src);
extern void BHGLVertexTypeFree(BHGLVertexType vType);
    
extern BHGLAnimationInfo BHGLAnimationInfoCreate(unsigned int numFrames, unsigned int fps);
extern void BHGLAnimationInfoCopy(BHGLAnimationInfo *dest, const BHGLAnimationInfo *src);
extern void BHGLAnimationInfoFree(BHGLAnimationInfo animInfo);
    
extern BHGLVertexAttribInfo* BHGLVertexAttributesForVertexType(const BHGLVertexType *vType);

/* binding */
extern void BHGLBindDefault();
extern void BHGLBindBuffers(GLuint arrayBuffer, GLuint indexBuffer);
extern void BHGLBindBufferSet(BHGLBufferSet set);
extern void BHGLBindVertexArray(GLuint vertexArray);

extern void BHGLEnableAttribute(GLuint program, const GLchar *name, GLuint *index);

/* state capturing */
extern void BHGLSaveProgram();
extern void BHGLRestoreProgram();
extern void BHGLSaveVertexBuffer();
extern void BHGLRestoreVertexBuffer();
extern void BHGLSaveIndexBuffer();
extern void BHGLRestoreIndexBuffer();
extern void BHGLSaveVertexArray();
extern void BHGLRestoreVertexArray();

/* NOTE: Names are bound before returning!! */
extern GLuint BHGLGenerateVertexArray();
extern GLuint BHGLGenerateVertexBuffer(GLsizeiptr size, GLvoid *vertices, GLenum usage);
extern GLuint BHGLGenerateIndexBuffer(GLsizeiptr size, GLuint *indices, GLenum usage);
extern GLuint BHGLGenerateRGBATextureDefault(GLsizei width, GLsizei height, GLubyte *data);
extern GLuint BHGLGenerateRGBATexture(GLsizei width, GLsizei height, GLubyte *data, GLenum minFilter, GLenum magFilter, GLenum sWrap, GLenum tWrap);

/* creates but does not link or use program */
extern GLuint BHGLGenerateProgram(GLuint vertexShader, GLuint fragmentShader);
extern GLboolean BHGLLinkProgram(GLuint program);

extern void BHGLDeleteVertexArrays(GLsizei n, GLuint *vertexArrays);

/* shader compiling. Shader string must be null terminated. */
extern GLuint BHGLCompileShader(const GLchar *shaderString, GLenum type);
extern GLuint BHGLCompileShaderf(const char *filePath, GLenum type);

/* print functions */
extern void BHGLprintShaderLog(GLuint shader);
extern void BHGLprintProgramLog(GLuint program);
extern GLenum BHGLError();
    
#ifdef __cplusplus
}
#endif

#endif
