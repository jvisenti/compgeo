//
//  BHGLKeyframeAnimation.h
//
//  Created by John Visentin on 10/21/13.
//  Copyright (c) 2013 Brockenhaus Studio. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "BHGLAnimation.h"
#import "BHGLTypes.h"

typedef void (^BHGLAnimationTransform)(GLKMatrix4 *m);

@interface BHGLKeyframeAnimation : BHGLAnimation

/** The total number of frames in the animation, set upon initialization. */
@property (nonatomic, readonly) unsigned int numFrames;

/** The current animation frame. 
    If the animation repeats, the frame is taken modulo the number of frames. */
@property (nonatomic, readonly) float currentFrame;

/** The number of frames per second at which to execute the animation. */
@property (nonatomic, assign) unsigned int fps;

/** A transform to apply at the end of each animation frame. 
    See the modelMatrix property for how this transform is used. */
@property (nonatomic, copy) BHGLAnimationTransform transform;

/** The scale vector for the current animation frame. Fractional frames are interpolated. */
@property (nonatomic, readonly) GLKVector3 scale;
/** The rotation quaternion for the current animation frame. Fractional frames are interpolated. */
@property (nonatomic, readonly) GLKQuaternion rotation;
/** The translation vector for the current animation frame. Fractional frames are interpolated. */
@property (nonatomic, readonly) GLKVector3 translation;

+ (BHGLKeyframeAnimation *)animationWithFrames:(unsigned int)numFrames fps:(unsigned int)fps;
- (id)initWithAnimationInfo:(BHGLAnimationInfo)animationInfo;

- (GLKVector3)scaleForFrame:(unsigned int)frame;
- (GLKQuaternion)rotationForFrame:(unsigned int)frame;
- (GLKVector3)translationForFrame:(unsigned int)frame;

/** Set the scale vector for the given animation frame. */
- (void)setScale:(GLKVector3)scale forFrame:(unsigned int)frame;
/** Set the rotation quaternion for the given animation frame. */
- (void)setRotation:(GLKQuaternion)rotation forFrame:(unsigned int)frame;
/** Set the translation vector for the given animation frame. */
- (void)setTranslation:(GLKVector3)translation forFrame:(unsigned int)frame;

/** Set the precomputed matrix to use for the given frame. */
- (void)setMatrix:(GLKMatrix4)matrix forFrame:(unsigned int)frame;

/** Creates an animation that reverses the behavior of the receiver.
 The reversed animation runs frames in reverse order of the original animation.
 */
- (instancetype)reversedAnimation;

@end
