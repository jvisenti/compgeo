//
//  BHGLKeyframeAnimation+PVR.m
//
//  Created by John Visentin on 10/21/13.
//  Copyright (c) 2013 Brockenhaus Studio. All rights reserved.
//

#import "BHGLKeyframeAnimation+PVR.h"
#import "PVRTModelPOD.h"
#import "BHGLCUtils.h"

@implementation BHGLKeyframeAnimation (PVR)

- (id)initWithSPODNode:(const SPODNode &)node numFrames:(unsigned int)numFrames fps:(unsigned int)fps
{
    if (node.nAnimFlags == 0)
    {
        // no animation happens, so just need 1 frame after all
        numFrames = 1;
    }
    
    BHGLAnimationInfo animInfo = BHGLAnimationInfoCreate(numFrames, fps);
    
    int scaleIdx = 0, rotationIdx = 0, translationIdx = 0, matrixIdx = 0;
    
    for (int i = 0; i < numFrames; i++)
    {
        if (node.pfAnimScale)
        {
            memcpy(animInfo.scales[i], &node.pfAnimScale[scaleIdx], 3*sizeof(GLfloat));
            
            if (node.nAnimFlags & ePODHasScaleAni)
            {
                scaleIdx += 7;
            }
        }
        
        if (node.pfAnimRotation)
        {
            // convert to CCW-positive angles
            GLKQuaternion q;
            memcpy(q.q, &node.pfAnimRotation[rotationIdx], 4*sizeof(float));
            q = GLKQuaternionInvert(q);
            
            memcpy(animInfo.rotations[i], q.q, 4*sizeof(float));
            
            if (node.nAnimFlags & ePODHasRotationAni)
            {
                rotationIdx += 4;
            }
        }
        
        if (node.pfAnimPosition)
        {
            memcpy(animInfo.translations[i], &node.pfAnimPosition[translationIdx], 3*sizeof(GLfloat));
            
            if (node.nAnimFlags & ePODHasPositionAni)
            {
                translationIdx += 3;
            }
        }
        
        if (node.pfAnimMatrix)
        {
            GLfloat *mat = (GLfloat *)malloc(16*sizeof(GLfloat));
            memcpy(mat, &(node.pfAnimMatrix[matrixIdx]), 16*sizeof(GLfloat));
            animInfo.matrices[i] = mat;
            
            if (node.nAnimFlags & ePODHasMatrixAni)
            {
                matrixIdx += 16;
            }
        }
    }
    
    self = [self initWithAnimationInfo:animInfo];
    
    BHGLAnimationInfoFree(animInfo);
    
    return self;
}

@end
