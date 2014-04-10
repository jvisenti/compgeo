//
//  BHGLLight.m
//
//  Created by John Visentin on 2/15/14.
//  Copyright (c) 2014 Brockenhaus Studio. All rights reserved.
//

#import "BHGLLight.h"

@implementation BHGLLight

- (id)init
{
    if ((self = [super init]))
    {
        self.type = BHGLLightTypeDirectional;
        self.enabled = YES;
    }
    return self;
}

- (GLKVector3)coneDirection
{
    return GLKQuaternionRotateVector3(self.rotation, _coneDirection);
}

- (BHGLLightInfo)lightInfo
{
    BHGLLightInfo lightInfo;
    
    lightInfo.identifier = self.identifier;
    lightInfo.type = self.type;
    lightInfo.enabled = self.isEnabled;
    lightInfo.ambient = self.ambientColor;
    lightInfo.color = self.color;
    lightInfo.constantAttenuation = self.constantAttenuation;
    lightInfo.linearAttenuation = self.linearAttenuation;
    lightInfo.quadraticAttentuation = self.quadraticAttenuation;
    lightInfo.spotCosCutoff = self.spotCosCutoff;
    lightInfo.spotExponent = self.spotExponent;
    
    memcpy(lightInfo.position, self.position.v, 3*sizeof(GLfloat));
    
    memcpy(lightInfo.halfVector, self.halfVector.v, 3*sizeof(GLfloat));
    
    GLKVector3 coneDir = self.coneDirection;
    memcpy(lightInfo.coneDirection, coneDir.v, 3*sizeof(GLfloat));
    
    return lightInfo;
}

@end
