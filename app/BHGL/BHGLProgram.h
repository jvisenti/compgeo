//
//  BHGLProgram.h
//
//  Created by John Visentin on 10/12/13.
//  Copyright (c) 2013 Brockenhaus Studio. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <GLKit/GLKMath.h>
#import "BHGLTypes.h"

@class BHGLTexture, BHGLMaterial, BHGLLight;

extern NSString* const kBHGLBasicVertexShader;
extern NSString* const kBHGLBasicFragmentShader;

extern NSString* const kBHGLMVPUniformName;
extern NSString* const kBHGLPositionAttributeName;
extern NSString* const kBHGLNormalAttributeName;
extern NSString* const kBHGLColorAttributeName;
extern NSString* const kBHGLTexCoord0AttributeName;
extern NSString* const kBHGLTexCoord1AttributeName;

@interface BHGLProgram : NSObject

/** An identifier for use by your application. */
@property (nonatomic, copy) NSString *identifier;

@property (nonatomic, readonly) GLuint name;
@property (nonatomic, readonly) GLuint vertexShader;
@property (nonatomic, readonly) GLuint fragmentShader;
@property (nonatomic, readonly, getter = isLinked) BOOL linked;

/** An array of NSNumbers containing each BHGLVertexAttrib that has been set to a name. */
@property (nonatomic, readonly) NSArray *boundAttributes;

@property (nonatomic, strong) BHGLTexture *texture0;
@property (nonatomic, assign) GLKMatrix4 modelViewMatrix;
@property (nonatomic, assign) GLKMatrix4 projectionMatrix;

/** The uniform variable name for the model view projection matrix. 
    Changing the vale of this property has no effect until the next time the program is linked. */
@property (nonatomic, copy) NSString *mvpUniformName;

/** Compiles and links a new program with the given shader names.
    @param vShaderName the name of the vertex shader file including extension.
    @param fShaderName the name of the fragment shader file including extension. */
- (id)initWithVertexShaderNamed:(NSString *)vShaderName fragmentShaderNamed:(NSString *)fShaderName mvpUniform:(NSString *)mvpName;

/** Associates an attribute with an attribute name from within the program.
    Program must be linked once attributes are set in order for the change to take effect. */
- (void)setVertexAttribute:(BHGLVertexAttrib)attribute forName:(NSString *)attributeName;

/** Returns attribute name from within the program for a given attribute.
    @note returns nil if attribute has not yet been associated with a name */
- (NSString *)nameForVertexAttribute:(BHGLVertexAttrib)attribute;

/** Sets the texture for the given texture slot. e.g. GL_TEXTURE1. Must be called after program is linked. In order to take effect.
    @note You may use the textureInfo0 property to set the texture at slot 0 instead. */
- (void)setTexture:(BHGLTexture *)texture forTextureSlot:(GLubyte)texSlot uniformName:(NSString *)uniformName;

/** Set the given uniform with data from the given material. The uniform's type must match BHGLMaterialInfo.
    @note Results are undefined if the receiver is not currently in use. */
- (void)setUniform:(NSString *)uniformName withMaterial:(BHGLMaterial *)material;

/** Set the given uniform (assumed to be an array) with data from the given materials. 
    The uniform's type must be an array of structs matching BHGLMaterialInfo.
    @note Results are undefined if the receiver is not currently in use. */
- (void)setUniform:(NSString *)uniformName withMaterials:(NSArray *)materials;

/** Set the given uniform with data from the given material. The uniform's type must match BHGLMaterialInfo.
 @note Results are undefined if the receiver is not currently in use. */
- (void)setUniform:(NSString *)uniformName withMaterialInfo:(BHGLMaterialInfo)materialInfo;

/** Set the given uniform (assumed to be an array) with data from the given n materials.
    The uniform's type must be an array of structs matching BHGLMaterialInfo.
    @note Results are undefined if the receiver is not currently in use. */
- (void)setUniform:(NSString *)uniformName withMaterialInfo:(BHGLMaterialInfo *)materialInfo num:(int)n;

/** Set the given uniform with data from the given light. The uniform's type must match BHGLLightInfo.
 @note Results are undefined if the receiver is not currently in use. */
- (void)setUniform:(NSString *)uniformName withLight:(BHGLLight *)light;

/** Set the given uniform (assumed to be an array) with data from the given lights.
 The uniform's type must be an array of structs matching BHGLLightInfo.
 @note Results are undefined if the receiver is not currently in use. */
- (void)setUniform:(NSString *)uniformName withLights:(NSArray *)lights;

/** Set the given uniform with data from the given light. The uniform's type must match BHGLLightInfo.
 @note Results are undefined if the receiver is not currently in use. */
- (void)setUniform:(NSString *)uniformName withLightInfo:(BHGLLightInfo)lightInfo;

/** Set the given uniform (assumed to be an array) with data from the given n lights.
 The uniform's type must be an array of structs matching BHGLLightInfo.
 @note Results are undefined if the receiver is not currently in use. */
- (void)setUniform:(NSString *)uniformName withLightInfo:(BHGLLightInfo *)lightInfo num:(int)n;

/** Returns the position of the given uniform. Results are undefined if the program is not linked. */
- (GLuint)uniformPosition:(NSString *)uniformName;

/** Sets the receiver as the current OpenGL program. */
- (void)use;

/** Link the receiver. Must be called before program can function.
    @return YES if link was successful, NO otherwise. */
- (BOOL)link;

/** Call before drawing to set appropriate uniforms. May change GL_CURRENT_PROGRAM and GL_TEXTURE_BINDING_2D.
    Subclasses should call super implementation.*/
- (void)prepareToDraw;

/** Compiles a new shader.
    @param shaderName the shader's filename, including extension.
    @return The newly compiled shader's name if successful. 0 if an error occurred. */
+ (GLuint)compileShaderNamed:(NSString *)shaderName type:(GLenum)shaderType;

@end
