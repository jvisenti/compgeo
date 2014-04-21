//
//  BHGLTypes.h
//
//  Created by John Visentin on 10/12/13.
//  Copyright (c) 2013 Brockenhaus Studio. All rights reserved.
//

#ifdef __APPLE__
#include <TargetConditionals.h>

#if TARGET_OS_IPHONE
#include <OpenGLES/ES2/gl.h>
#include <OpenGLES/ES2/glext.h>
#else
#include <OpenGL/gl3.h>
#include <OpenGL/gl3ext.h>
#endif

#else
#include <GL/gl.h>
#include <GL/glx.h>
#endif

#ifndef BHGLTypes_h
#define BHGLTypes_h

#if defined(__cplusplus)
extern "C" {
#endif

#ifdef GL_ES_VERSION_2_0
#define BHGL_VERTEX_ARRAY_BINDING GL_VERTEX_ARRAY_BINDING_OES
#else
#define BHGL_VERTEX_ARRAY_BINDING GL_VERTEX_ARRAY_BINDING
#endif

#define BHGL_COLOR_VERTEX       0x10
#define BHGL_TEXTURE_VERTEX     0x14
#define BHGL_NCOLOR_VERTEX      0x1C
#define BHGL_NTEXTURE_VERTEX    0x20

typedef GLenum BHGLEnum;

/* indexes for shader attributes */
typedef enum
{
    BHGLVertexAttribPosition,
    BHGLVertexAttribNormal,
    BHGLVertexAttribColor,
    BHGLVertexAttribTexCoord0,
    BHGLVertexAttribTexCoord1
} BHGLVertexAttrib;
    
typedef enum
{
    BHGLLightTypeUnknown = 0,
    BHGLLightTypeDirectional,
    BHGLLightTypePoint,
    BHGLLightTypeSpot
} BHGLLightType;

typedef struct _BHGLBufferSet
{
    GLuint vertexBuffer;
    GLuint indexBuffer;
} BHGLBufferSet;

static inline BHGLBufferSet BHGLBufferSetMake(GLuint vertexBuffer, GLuint indexBuffer)
{
    return (BHGLBufferSet){vertexBuffer, indexBuffer};
}

typedef struct _BHGLTextureInfo
{
    GLuint name;
    GLsizei width;
    GLsizei height;
} BHGLTextureInfo;

static inline BHGLTextureInfo BHGLTextureInfoMake(GLuint name, GLsizei width, GLsizei height)
{
    return (BHGLTextureInfo){name, width, height};
}

static inline int BHGLTextureInfoIsNull(BHGLTextureInfo info)
{
    return (info.name == 0);
}

typedef struct _BHGLVertexAttribInfo
{
    BHGLVertexAttrib attribute;
    GLuint index;
    GLint length;
    GLenum type;
    GLboolean normalized;
    GLsizei stride;
    const GLvoid *ptr;
} BHGLVertexAttribInfo;

static inline BHGLVertexAttribInfo BHGLVertexAttribInfoMake(BHGLVertexAttrib attrib, GLuint index, GLint length, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid *ptr)
{
    return (BHGLVertexAttribInfo){attrib, index, length, type, normalized, stride, ptr};
}
    
typedef union _BHGLColor
{
    struct {GLfloat r, g, b, a;}; // rgba values should be 0.0 to 1.0
    GLfloat c[4];
} BHGLColor;
    
static inline BHGLColor BHGLColorMake(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha)
{
    BHGLColor c;
    c.r = red;
    c.g = green;
    c.b = blue;
    c.a = alpha;
    
    return c;
}
    
/* color constants */
static const BHGLColor BHGLColorClear = {0.0f, 0.0f, 0.0f, 0.0f};
static const BHGLColor BHGLColorBlack = {0.0f, 0.0f, 0.0f, 1.0f};
static const BHGLColor BHGLColorWhite = {1.0f, 1.0f, 1.0f, 1.0f};
static const BHGLColor BHGLColorRed = {1.0f, 0.0f, 0.0f, 1.0f};
static const BHGLColor BHGLColorGreen = {0.0f, 1.0f, 0.0f, 1.0f};
static const BHGLColor BHGLColorBlue = {0.0f, 0.0f, 1.0f, 1.0f};
static const BHGLColor BHGLColorCyan = {0.0f, 1.0f, 1.0f, 1.0f};
static const BHGLColor BHGLColorYellow = {1.0f, 1.0f, 0.0f, 1.0f};
static const BHGLColor BHGLColorMagenta = {1.0f, 0.0f, 1.0f, 1.0f};
static const BHGLColor BHGLColorOrange = {1.0f, 0.5f, 0.0f, 1.0f};
static const BHGLColor BHGLColorPurple = {0.5f, 0.0f, 0.5f, 1.0f};
static const BHGLColor BHGLColorBrown = {0.6f, 0.4f, 0.2f, 1.0f};
static const BHGLColor BHGLColorTeal = {0.0f, 0.5f, 0.5f, 1.0f};

typedef struct _BHGLColorVertex
{
    GLfloat position[3];
    GLubyte color[4];
} BHGLColorVertex;

typedef struct _BHGLNColorVertex
{
    GLfloat position[3];
    GLfloat normal[3];
    GLubyte color[4];
} BHGLNColorVertex;

typedef struct _BHGLTextureVertex
{
    GLfloat position[3];
    GLfloat texCoord[2];
} BHGLTextureVertex;

typedef struct _BHGLNTextureVertex
{
    GLfloat position[3];
    GLfloat normal[3];
    GLfloat texCoord[2];
} BHGLNTextureVertex;
    
typedef struct _BHGLVertexType
{
    GLubyte numAttribs;
    BHGLVertexAttrib *attribs;
    GLenum *types;
    GLubyte *lengths;
    GLboolean *normalized;
    GLvoid **offsets;
    GLsizei stride;
} BHGLVertexType;
        
typedef struct _BHGLAnimationInfo
{
    unsigned int numFrames;
    unsigned int fps;
    GLfloat (*scales)[3];
    GLfloat (*rotations)[4]; // quaternions
    GLfloat (*translations)[3];
    GLfloat **matrices; // precomputed 4x4 matrices (GLfloat[16]) for each frame. May be null.
    void (*transform)(GLfloat(*)[16]); // additional transform function to apply each frame. May be null.
} BHGLAnimationInfo;
    
typedef struct _BHGLMaterialInfo
{
    GLuint identifier; // for use by your application, but otherwise unused
    BHGLColor emission;
    BHGLColor surface;
    BHGLColor ambient;
    BHGLColor diffuse;
    BHGLColor specular;
    GLfloat shininess;
} BHGLMaterialInfo;
    
typedef struct _BHGLLightInfo
{
    GLuint identifier; // for use by your application, but otherwise unused
    BHGLLightType type;
    GLboolean enabled;
    BHGLColor ambient;
    BHGLColor color;
    GLfloat position[3];
    GLfloat constantAttenuation;
    GLfloat linearAttenuation;
    GLfloat quadraticAttentuation;
    GLfloat halfVector[3];          // only for directional
    GLfloat coneDirection[3];       // only for spot
    GLfloat spotCosCutoff;          // only for spot
    GLfloat spotExponent;           // only for spot
} BHGLLightInfo;
    
#if defined(__cplusplus)
}
#endif

#endif
