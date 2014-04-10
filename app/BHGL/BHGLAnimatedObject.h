//
//  BHGLAnimatedObject.h
//
//  Created by John Visentin on 12/6/13.
//  Copyright (c) 2013 Brockenhaus Studio. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "BHGLAnimation.h"

@interface BHGLAnimatedObject : NSObject

/** The current translation of the object. Default is (0, 0, 0) */
@property (nonatomic, assign) GLKVector3 position;

/** The current rotation of the object. Default is the identity. */
@property (nonatomic, assign) GLKQuaternion rotation;

/** The current scale of the object. Default is (1, 1, 1). */
@property (nonatomic, assign) GLKVector3 scale;

/** The model matrix computed from current position, rotation, and scale. */
@property (nonatomic, readonly) GLKMatrix4 modelMatrix;

/** Sets the position, rotation, and scale properties by decomposing the given transform. 
    @warning NOT YET IMPLEMENTED! */
- (void)setTransform:(GLKMatrix4)transform;

/** All animations currently running on the object. */
- (NSArray *)activeAnimations;

/** Immediately adds the animation to a list of animations run by the receiver.
    Animations are processed on each update call. */
- (void)runAnimation:(BHGLAnimation *)animation;

/** Enqueues the animation to be run after all currently executing animations are complete.
    When the animation is ready to be run, the runAnimation method is called.
    @note The enqueued animation may never be run if currently executing animations repeat.
    Animations are processed on each update call. */
- (void)enqueueAnimation:(BHGLAnimation *)animation;

/** Removes the animation from the list of animations run by the receiver.
    The animation will not be processed on subsequent update calls. */
- (void)removeAnimation:(BHGLAnimation *)animation;

/** Removes all animations from the list of animations run by the receiver.
    The animations will not be processed on subsequent update calls. */
- (void)removeAllAnimations;

/** Advances animation time. Subclasses should call the super implementation. */
- (void)update:(NSTimeInterval)dt;

@end
