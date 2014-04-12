//
//  BHGLLight.h
//
//  Created by John Visentin on 2/15/14.
//  Copyright (c) 2014 Brockenhaus Studio. All rights reserved.
//

#import "BHGLAnimatedObject.h"
#import <GLKit/GLKMath.h>
#import "BHGLTypes.h"

@interface BHGLLight : BHGLAnimatedObject

/** An identifier for use by your application. This could, for instance, be
 an index into a light array. */
@property (nonatomic, assign) GLuint identifier;

/** An descriptive name for use by your application. */
@property (nonatomic, copy) NSString *name;

/** The type of light. Default is BHGLLightTypeDirectional. */
@property (nonatomic, assign) BHGLLightType type;

/** Whether the light is enabled. Default is YES. */
@property (nonatomic, assign, getter = isEnabled) BOOL enabled;

/** The light's contribution to ambient light. */
@property (nonatomic, assign) BHGLColor ambientColor;

/** Color of emitted light. */
@property (nonatomic, assign) BHGLColor color;

/** The constant coefficient of attentuation. Unused by directional lights. */
@property (nonatomic, assign) float constantAttenuation;

/** The linear coefficient of attentuation. Unused by directional lights. */
@property (nonatomic, assign) float linearAttenuation;

/** The quadratic coefficient of attentuation. Unused by directional lights. */
@property (nonatomic, assign) float quadraticAttenuation;

/** The pre-computed direction of highlights. Only used by directional lights. */
@property (nonatomic, assign) GLKVector3 halfVector;

/** The cone direction for spot lights. Only used by spot lights.
    @note The getter returns the cone direction rotated by the light's rotation property. */
@property (nonatomic, assign) GLKVector3 coneDirection;

/** The cosine cutoff value. Only used by spot lights. */
@property (nonatomic, assign) float spotCosCutoff;

/** The exponent used for additional spot light attenuation. Only used by spot lights. */
@property (nonatomic, assign) float spotExponent;

/** Extract properties into a light info struct. */
- (BHGLLightInfo)lightInfo;

@end
