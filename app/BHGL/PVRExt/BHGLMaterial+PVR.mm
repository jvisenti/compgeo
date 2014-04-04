//
//  BHGLMaterial+PVR.m
//
//  Created by John Visentin on 12/18/13.
//  Copyright (c) 2013 Brockenhaus Studio. All rights reserved.
//

#import "BHGLMaterial+PVR.h"
#import "PVRTModelPOD.h"

@implementation BHGLMaterial (PVR)

- (id)initWithSPODMaterial:(const SPODMaterial &)material
{
    if ((self = [super init]))
    {
        if (material.pfMatAmbient)
        {
            self.ambientColor = BHGLColorMake(material.pfMatAmbient[0], material.pfMatAmbient[1], material.pfMatAmbient[2], material.fMatOpacity);
        }
        
        if (material.pfMatDiffuse)
        {
            self.diffuseColor = BHGLColorMake(material.pfMatDiffuse[0], material.pfMatDiffuse[1], material.pfMatDiffuse[2], material.fMatOpacity);
        }
        
        if (material.pfMatSpecular)
        {
            self.specularColor = BHGLColorMake(material.pfMatSpecular[0], material.pfMatSpecular[1], material.pfMatSpecular[2], material.fMatOpacity);
        }
        
        self.shininess = material.fMatShininess;
        
        if (material.nFlags & ePODEnableBlending)
        {
            self.blendEnabled = GL_TRUE;
        }
        else
        {
            self.blendEnabled = GL_FALSE;
        }
        
        self.blendSrcRGB = BHGLBlendFuncFromPODBlendFunc(material.eBlendSrcRGB);
        self.blendDestRGB = BHGLBlendFuncFromPODBlendFunc(material.eBlendDstRGB);
        self.blendSrcAlpha = BHGLBlendFuncFromPODBlendFunc(material.eBlendSrcA);
        self.blendDestAlpha = BHGLBlendFuncFromPODBlendFunc(material.eBlendDstA);
    }
    return self;
}

static inline GLenum BHGLBlendFuncFromPODBlendFunc(EPODBlendFunc func)
{
    GLenum f = GL_ZERO;
    switch (func)
    {
        case ePODBlendFunc_ONE:
            f = GL_ONE;
            break;
            
        case ePODBlendFunc_SRC_COLOR:
            f = GL_SRC_COLOR;
            break;
            
        case ePODBlendFunc_ONE_MINUS_SRC_COLOR:
            f = GL_ONE_MINUS_SRC_COLOR;
            break;
            
        case ePODBlendFunc_SRC_ALPHA:
            f = GL_SRC_ALPHA;
            break;
            
        case ePODBlendFunc_ONE_MINUS_SRC_ALPHA:
            f = GL_ONE_MINUS_SRC_ALPHA;
            break;
            
        case ePODBlendFunc_DST_COLOR:
            f = GL_DST_ALPHA;
            break;
            
        case ePODBlendFunc_ONE_MINUS_DST_COLOR:
            f = GL_ONE_MINUS_DST_COLOR;
            break;
            
        case ePODBlendFunc_DST_ALPHA:
            f = GL_DST_ALPHA;
            break;
            
        case ePODBlendFunc_ONE_MINUS_DST_ALPHA:
            f = GL_ONE_MINUS_DST_ALPHA;
            break;
            
        case ePODBlendFunc_CONSTANT_COLOR:
            f = GL_CONSTANT_COLOR;
            break;
            
        case ePODBlendFunc_ONE_MINUS_CONSTANT_COLOR:
            f = GL_ONE_MINUS_CONSTANT_COLOR;
            break;
            
        case ePODBlendFunc_CONSTANT_ALPHA:
            f = GL_ONE_MINUS_CONSTANT_ALPHA;
            break;
            
        case ePODBlendFunc_ONE_MINUS_CONSTANT_ALPHA:
            f = GL_ONE_MINUS_CONSTANT_ALPHA;
            break;
            
        default:
            break;
    };
    
    return f;
}

@end
