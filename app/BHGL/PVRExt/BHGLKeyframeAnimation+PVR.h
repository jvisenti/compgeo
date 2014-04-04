//
//  BHGLKeyframeAnimation+PVR.h
//
//  Created by John Visentin on 10/21/13.
//  Copyright (c) 2013 Brockenhaus Studio. All rights reserved.
//

#import "BHGLKeyframeAnimation.h"

class SPODNode;

@interface BHGLKeyframeAnimation (PVR)

- (id)initWithSPODNode:(const SPODNode &)node numFrames:(unsigned int)numFrames fps:(unsigned int)fps;

@end
