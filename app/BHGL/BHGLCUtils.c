//
//  BHGLCUtils.c
//
//  Created by John Visentin on 10/11/13.
//  Copyright (c) 2013 Brockenhaus Studio. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "BHGLCUtils.h"

#pragma mark - type helpers

BHGLVertexType BHGLVertexTypeCreate(GLubyte numAttribs)
{
    BHGLVertexType v;
    v.numAttribs = numAttribs;
    v.attribs = malloc(numAttribs*sizeof(BHGLVertexAttrib));
    v.types = malloc(numAttribs*sizeof(GLenum));
    v.normalized = malloc(numAttribs*sizeof(GLboolean));
    v.lengths = malloc(numAttribs*sizeof(GLubyte));
    v.offsets = malloc(numAttribs*sizeof(GLvoid*));
    v.stride = 0;
    
    return v;
}

BHGLVertexType BHGLVertexTypeCreateWithType(BHGLEnum vType)
{
    BHGLVertexType v;
    
    switch (vType)
    {
        case BHGL_COLOR_VERTEX:
        {
            v = BHGLVertexTypeCreate(2);
            
            v.attribs[0] = BHGLVertexAttribPosition;
            v.attribs[1] = BHGLVertexAttribColor;
            
            v.types[0] = GL_FLOAT;
            v.types[1] = GL_UNSIGNED_BYTE;
            
            v.normalized[0] = GL_FALSE;
            v.normalized[1] = GL_TRUE;
            
            v.lengths[0] = 3;
            v.lengths[1] = 4;
            
            v.offsets[0] = (GLvoid *)offsetof(BHGLColorVertex, position);
            v.offsets[1] = (GLvoid *)offsetof(BHGLColorVertex, color);
            
            v.stride = sizeof(BHGLColorVertex);
            
            break;
        }
            
        case BHGL_NCOLOR_VERTEX:
        {
            v = BHGLVertexTypeCreate(3);
            
            v.attribs[0] = BHGLVertexAttribPosition;
            v.attribs[1] = BHGLVertexAttribNormal;
            v.attribs[2] = BHGLVertexAttribColor;
            
            v.types[0] = GL_FLOAT;
            v.types[1] = GL_FLOAT;
            v.types[2] = GL_UNSIGNED_BYTE;
            
            v.normalized[0] = GL_FALSE;
            v.normalized[1] = GL_FALSE;
            v.normalized[2] = GL_TRUE;
            
            v.lengths[0] = 3;
            v.lengths[1] = 3;
            v.lengths[2] = 4;
            
            v.offsets[0] = (GLvoid *)offsetof(BHGLNColorVertex, position);
            v.offsets[1] = (GLvoid *)offsetof(BHGLNColorVertex, normal);
            v.offsets[2] = (GLvoid *)offsetof(BHGLNColorVertex, color);
            
            v.stride = sizeof(BHGLNColorVertex);
            
            break;
        }
            
        case BHGL_TEXTURE_VERTEX:
        {
            v = BHGLVertexTypeCreate(2);
            
            v.attribs[0] = BHGLVertexAttribPosition;
            v.attribs[1] = BHGLVertexAttribTexCoord0;
            
            v.types[0] = GL_FLOAT;
            v.types[1] = GL_FLOAT;
            
            v.normalized[0] = GL_FALSE;
            v.normalized[1] = GL_FALSE;
            
            v.lengths[0] = 3;
            v.lengths[1] = 2;
            
            v.offsets[0] = (GLvoid *)offsetof(BHGLTextureVertex, position);
            v.offsets[1] = (GLvoid *)offsetof(BHGLTextureVertex, texCoord);
            
            v.stride = sizeof(BHGLTextureVertex);
            
            break;
        }
            
        case BHGL_NTEXTURE_VERTEX:
        {
            v = BHGLVertexTypeCreate(3);
            
            v.attribs[0] = BHGLVertexAttribPosition;
            v.attribs[1] = BHGLVertexAttribNormal;
            v.attribs[2] = BHGLVertexAttribTexCoord0;
            
            v.types[0] = GL_FLOAT;
            v.types[1] = GL_FLOAT;
            v.types[2] = GL_FLOAT;
            
            v.normalized[0] = GL_FALSE;
            v.normalized[1] = GL_FALSE;
            v.normalized[2] = GL_FALSE;
            
            v.lengths[0] = 3;
            v.lengths[1] = 3;
            v.lengths[2] = 2;
            
            v.offsets[0] = (GLvoid *)offsetof(BHGLNTextureVertex, position);
            v.offsets[1] = (GLvoid *)offsetof(BHGLNTextureVertex, normal);
            v.offsets[2] = (GLvoid *)offsetof(BHGLNTextureVertex, texCoord);
            
            v.stride = sizeof(BHGLNTextureVertex);
            
            break;
        }
            
        default:
            break;
    }
    
    return v;
}

void BHGLVertexTypeCopy(BHGLVertexType *dest, const BHGLVertexType *src)
{
    GLubyte n = src->numAttribs;
        
    *dest = BHGLVertexTypeCreate(n);
    memcpy(dest->attribs, src->attribs, n*sizeof(BHGLVertexAttrib));
    memcpy(dest->types, src->types, n*sizeof(GLenum));
    memcpy(dest->normalized, src->normalized, n*sizeof(GLboolean));
    memcpy(dest->lengths, src->lengths, n*sizeof(GLubyte));
    memcpy(dest->offsets, src->offsets, n*sizeof(GLint));
    dest->stride = src->stride;    
}

void BHGLVertexTypeFree(BHGLVertexType vType)
{
    free(vType.attribs);
    vType.attribs = NULL;
    
    free(vType.types);
    vType.types = NULL;
    
    free(vType.normalized);
    vType.normalized = NULL;
    
    free(vType.lengths);
    vType.lengths = NULL;
    
    free(vType.offsets);
    vType.offsets = NULL;
}

BHGLAnimationInfo BHGLAnimationInfoCreate(unsigned int numFrames, unsigned int fps)
{
    BHGLAnimationInfo animInfo;
    animInfo.numFrames = numFrames;
    animInfo.fps = fps;
    animInfo.transform = NULL;
    
    animInfo.scales = malloc(numFrames*sizeof(GLfloat[3]));
    animInfo.rotations = calloc(numFrames, sizeof(GLfloat[4]));
    animInfo.translations = calloc(numFrames, sizeof(GLfloat[3]));
    
    animInfo.matrices = malloc(numFrames*sizeof(GLfloat*));
    
    int i, j;
    for (i = 0; i < numFrames; i++)
    {
        for (j = 0; j < 3; j++)
        {
            animInfo.scales[i][j] = 1.0f;
        }
        
        animInfo.rotations[i][3] = 1.0f;
        
        animInfo.matrices[i] = NULL;
    }
    
    return animInfo;
}

void BHGLAnimationInfoCopy(BHGLAnimationInfo *dest, const BHGLAnimationInfo *src)
{
    GLuint n = src->numFrames;
    
    *dest = BHGLAnimationInfoCreate(n, src->fps);
    dest->transform = src->transform;
    memcpy(dest->scales, src->scales, n*sizeof(GLfloat[3]));
    memcpy(dest->rotations, src->rotations, n*sizeof(GLfloat[4]));
    memcpy(dest->translations, src->translations, n*sizeof(GLfloat[3]));
    
    int i;
    for (i = 0; i < n; i++)
    {
        if (src->matrices[i] != NULL)
        {
            dest->matrices[i] = malloc(16*sizeof(GLfloat));
            memcpy(dest->matrices[i], src->matrices[i], 16*sizeof(GLfloat));
        }
        else
        {
            dest->matrices[i] = NULL;
        }
    }
}

void BHGLAnimationInfoFree(BHGLAnimationInfo animInfo)
{
    free(animInfo.scales);
    animInfo.scales = NULL;
    
    free(animInfo.rotations);
    animInfo.scales = NULL;
    
    free(animInfo.translations);
    animInfo.translations = NULL;
    
    int i;
    for (i = 0; i < animInfo.numFrames; i++)
    {
        free(animInfo.matrices[i]);
        animInfo.matrices[i] = NULL;
    }
    
    free(animInfo.matrices);
    animInfo.matrices = NULL;
}

BHGLVertexAttribInfo* BHGLVertexAttributesForVertexType(const BHGLVertexType *vType)
{
    BHGLVertexAttribInfo *attribs = malloc(vType->numAttribs * sizeof(BHGLVertexAttribInfo));
    
    int i;
    for (i = 0; i < vType->numAttribs; i++)
    {        
        attribs[i] = BHGLVertexAttribInfoMake(vType->attribs[i], vType->attribs[i], vType->lengths[i], vType->types[i], vType->normalized[i], vType->stride, (const GLvoid *)vType->offsets[i]);
    }
    
    return attribs;
}

#pragma mark - binding

void BHGLBindDefault()
{
    BHGLBindVertexArray(0);
    BHGLBindBuffers(0, 0);
}

extern void BHGLBindBuffers(GLuint arrayBuffer, GLuint indexBuffer)
{
    glBindBuffer(GL_ARRAY_BUFFER, arrayBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
}

void BHGLBindBufferSet(BHGLBufferSet set)
{
    BHGLBindBuffers(set.vertexBuffer, set.indexBuffer);
}

void BHGLBindVertexArray(GLuint vertexArray)
{
#ifdef GL_ES_VERSION_2_0
    glBindVertexArrayOES(vertexArray);
#else
    glBindVertexArray(vertexArray);
#endif
}

void BHGLEnableAttribute(GLuint program, const GLchar *name, GLuint *index)
{
    *index = glGetAttribLocation(program, name);
    glEnableVertexAttribArray(*index);
}

#pragma mark - state capturing

static GLint _saved_prog;
void BHGLSaveProgram()
{
    glGetIntegerv(GL_CURRENT_PROGRAM, &_saved_prog);
}

void BHGLRestoreProgram()
{
    glUseProgram(_saved_prog);
}

static GLint _saved_vbo;
void BHGLSaveVertexBuffer()
{
    glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &_saved_vbo);
}

void BHGLRestoreVertexBuffer()
{
    glBindBuffer(GL_ARRAY_BUFFER, _saved_vbo);
}

static GLint _saved_ibo;
void BHGLSaveIndexBuffer()
{
    glGetIntegerv(GL_ELEMENT_ARRAY_BUFFER_BINDING, &_saved_ibo);
}

void BHGLRestoreIndexBuffer()
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _saved_ibo);
}

static GLint _saved_vao;
void BHGLSaveVertexArray()
{
    glGetIntegerv(BHGL_VERTEX_ARRAY_BINDING, &_saved_vao);
}

void BHGLRestoreVertexArray()
{
    BHGLBindVertexArray(_saved_vao);
}

#pragma mark - object creation

GLuint BHGLGenerateVertexArray()
{
    GLuint vertexArray;
#ifdef GL_ES_VERSION_2_0
    glGenVertexArraysOES(1, &vertexArray);
#else
    glGenVertexArrays(1, &vertexArray);
#endif
    
    BHGLBindVertexArray(vertexArray);
        
    return vertexArray;
}

GLuint BHGLGenerateVertexBuffer(GLsizeiptr size, GLvoid *vertices, GLenum usage)
{
    GLuint vertexBuffer;
    
    glGenBuffers(1, &vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, size, (const GLvoid *)vertices, usage);
    
    return vertexBuffer;
}

GLuint BHGLGenerateIndexBuffer(GLsizeiptr size, GLuint *indices, GLenum usage)
{
    GLuint indexBufer;
    
    glGenBuffers(1, &indexBufer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, (const GLvoid *)indices, usage);
    
    return indexBufer;
}

extern GLuint BHGLGenerateProgram(GLuint vertexShader, GLuint fragmentShader)
{
    GLuint program = glCreateProgram();
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
        
    return program;
}

GLuint BHGLGenerateRGBATextureDefault(GLsizei width, GLsizei height, GLubyte *data)
{
    return BHGLGenerateRGBATexture(width, height, data, GL_LINEAR, GL_LINEAR, GL_REPEAT, GL_REPEAT);
}

GLuint BHGLGenerateRGBATexture(GLsizei width, GLsizei height, GLubyte *data, GLenum minFilter, GLenum magFilter, GLenum sWrap, GLenum tWrap)
{
    GLuint tex;
    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_2D, tex);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, sWrap);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, tWrap);
    
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, (GLvoid *)data);
    
    return tex;
}


GLboolean BHGLLinkProgram(GLuint program)
{
    glLinkProgram(program);
    
    GLint success;
    glGetProgramiv(program, GL_LINK_STATUS, &success);
   
#ifdef DEBUG
    if (success == GL_FALSE)
    {
        BHGLprintProgramLog(program);
    }
#endif

    return success;
}

void BHGLDeleteVertexArrays(GLsizei n, GLuint *vertexArrays)
{
#ifdef GL_ES_VERSION_2_0
    glDeleteVertexArraysOES(n, vertexArrays);
#else
    glDeleteVertexArrays(n, vertexArrays);
#endif
}


#pragma mark - shader compiling

GLuint BHGLCompileShader(const GLchar *shaderString, GLenum type)
{
    GLuint shader = glCreateShader(type);
    GLint length = (GLuint)strlen(shaderString);
    
    glShaderSource(shader, 1, &shaderString, &length);
    glCompileShader(shader);
    
#ifdef DEBUG
    GLint success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    
    BHGLprintShaderLog(shader);
#endif
    
    return shader;
}

GLuint BHGLCompileShaderf(const char *filePath, GLenum type)
{
    FILE *fp = fopen(filePath, "r");
    
    if (fp)
    {
        fseek(fp, 0, SEEK_END);
        long fileSize = ftell(fp);
        fseek(fp, 0, SEEK_SET);
        
        char *shader = malloc(fileSize+1);
        
        fread(shader, 1, fileSize, fp);
        shader[fileSize] = '\0';
        fclose(fp);
                
        GLuint name = BHGLCompileShader(shader, type);
        
        free(shader);
        return name;
    }
    else
    {
#ifdef DEBUG
        fprintf(stderr, "failed to load shader from %s\n", filePath);
#endif
        fclose(fp);
        return 0;
    }
}

#pragma mark - print functions

void BHGLprintShaderLog(GLuint shader)
{
    GLint length;
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);
    
    GLchar *logText = malloc(length * sizeof(GLchar));
    glGetShaderInfoLog(shader, length, 0, logText);
    
    fprintf(stderr, "%s\n", logText);
    
    free(logText);
}

void BHGLprintProgramLog(GLuint program)
{
    GLint length;
    glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length);
    
    GLchar *logText = malloc(length * sizeof(GLchar));
    glGetProgramInfoLog(program, length, 0, logText);
    
    fprintf(stderr, "%s\n", logText);
    
    free(logText);
}

GLenum BHGLError()
{
    GLenum errCode;
    const GLchar *errString = NULL;
    
    switch(errCode = glGetError())
    {
        case GL_NO_ERROR:
            break;
            
        case GL_INVALID_ENUM:
			errString = "GL_INVALID_ENUM";
			break;
            
		case GL_INVALID_VALUE:
			errString = "GL_INVALID_VALUE";
			break;
            
		case GL_INVALID_OPERATION:
			errString = "GL_INVALID_OPERATION";
			break;
            
		case GL_INVALID_FRAMEBUFFER_OPERATION:
			errString = "GL_INVALID_FRAMEBUFFER_OPERATION";
			break;
            
        default:
            errString = "UNKNOWN GL ERROR";
    }
    
#ifdef DEBUG
    if (errString != NULL)
    {
        fprintf(stderr, "BHGL Error: %s\n", errString);
    }
#endif
    
    return errCode;
}