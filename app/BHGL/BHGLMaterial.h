//
//  BHGLMaterial.h
//
//  Created by John Visentin on 12/18/13.
//  Copyright (c) 2013 Brockenhaus Studio. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "BHGLTypes.h"
#import "BHGLTexture.h"

@interface BHGLMaterial : NSObject

/** An identifier for use by your application. This could, for instance, be
    an index into a materials array. */
@property (nonatomic, assign) GLuint identifier;

/** An descriptive name for use by your application. */
@property (nonatomic, copy) NSString *name;

/** The difuse texture for the material. */
@property (nonatomic, strong) BHGLTexture *texture;

/** Color emitted by the surface. Default is BHGLColorClear*/
@property (nonatomic, assign) BHGLColor emissionColor;

/** Surface color. Default is BHGLColorBlack. */
@property (nonatomic, assign) BHGLColor surfaceColor;

/** Ambient light contribution. Default is BHGLColorBlack. */
@property (nonatomic, assign) BHGLColor ambientColor;

/** Diffuse light contribution. Default is BHGLColorBlack. */
@property (nonatomic, assign) BHGLColor diffuseColor;

/** Specular light contribution. Default is BHGLColorBlack. */
@property (nonatomic, assign) BHGLColor specularColor;

/** Exponent for sharpening specular highlights. Default is 0.0. */
@property (nonatomic, assign) GLfloat shininess;

/** Whether glBlend should be enabled for this material. Default is GL_FALSE. */
@property (nonatomic, assign, getter = isBlendEnabled) GLboolean blendEnabled;

/* Properties for the glBlend function. Only applied if blendEnabled is GL_TRUE. */
@property (nonatomic, assign) GLenum blendSrcRGB; /** Default is GL_ONE. */
@property (nonatomic, assign) GLenum blendDestRGB; /** Default is GL_ZERO. */

@property (nonatomic, assign) GLenum blendSrcAlpha; /** Default is GL_ONE. */
@property (nonatomic, assign) GLenum blendDestAlpha; /** Default is GL_ZERO. */

/** Calls the appropriate gl methods to bind the material for rendering. 
    May alter GL_ACTIVE_TEXTURE and glBlendFunc. */
- (void)bind;

/** Resets GL_ACTIVE_TEXTURE and glBlendFunc to defaults. */
- (void)unbind;

/** Extract color and shininess info into a material info struct. */
- (BHGLMaterialInfo)materialInfo;

@end
