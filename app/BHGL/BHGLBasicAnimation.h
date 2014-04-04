//
//  BHGLBasicAnimation.h
//
//  Created by John Visentin on 12/1/13.
//  Copyright (c) 2013 Brockenhaus Studio. All rights reserved.
//

#import "BHGLAnimation.h"

@interface BHGLBasicAnimation : BHGLAnimation

/** An animation that has no effect for the given duration. 
    @note This animation is reversible, but the reversed animation has the same behavior. */
+ (BHGLBasicAnimation *)wait:(NSTimeInterval)duration;

/** An animation whose model matrix is computed using a custom block over the given duration.
    @note This animation is not reversible. The reversed animation simply does nothing. */
+ (BHGLBasicAnimation *)transformWithBlock:(void(^)(BHGLAnimatedObject *object, NSTimeInterval current, NSTimeInterval duration))animationBlock duration:(NSTimeInterval)duration;

/** An animation that runs the given block immediately.
    @note This animation is not reversible. The reversed animation simply does nothing. */
+ (BHGLBasicAnimation *)runBlock:(void(^)())block;

#pragma mark - scaling

/** An animation that immediately scales to the given vector.
    @note This animation is reversible, but the reversed animation does nothing (as expected). */
+ (BHGLBasicAnimation *)scaleTo:(GLKVector3)scale;
/** An animation that scales to the given vector over the given duration. 
    @note This animation is reversible, and scales from the given scale to the current scale. */
+ (BHGLBasicAnimation *)scaleTo:(GLKVector3)scale withDuration:(NSTimeInterval)duration;

/** An animation that immediately scales by the given vector.
    @note This animation is reversible, and scales by the inverse vector. */
+ (BHGLBasicAnimation *)scaleBy:(GLKVector3)scale;
/** An animation that scales by the given vector over the given duration. 
    @note This animation is reversible, and scales by the inverse vector. */
+ (BHGLBasicAnimation *)scaleBy:(GLKVector3)scale withDuration:(NSTimeInterval)duration;

#pragma mark - rotation

/** An animation that immediately rotates to the given quaternion.
    @note This animation is reversible, but the reversed animation does nothing (as expected). */
+ (BHGLBasicAnimation *)rotateTo:(GLKQuaternion)rotation;
/** An animation that rotates to the given quaternion over the given duration. 
    @note This animation is reversible, and rotates from the given rotation to the current rotation. */
+ (BHGLBasicAnimation *)rotateTo:(GLKQuaternion)rotation withDuration:(NSTimeInterval)duration;

/** An animation that immediately rotates by the given quaternion.
    @note This animation is reversible, and rotates by the inverse angle. */
+ (BHGLBasicAnimation *)rotateBy:(GLKQuaternion)rotation;
/** An animation that rotates by the given quaternion over the given duration. 
    @note This animation is reversible, and rotates by the inverse angle. */
+ (BHGLBasicAnimation *)rotateBy:(GLKQuaternion)rotation withDuration:(NSTimeInterval)duration;

#pragma mark - translation

/** An animation that immediately translates to the given position.
    @note This animation is reversible, but the reversed animation does nothing (as expected). */
+ (BHGLBasicAnimation *)translateTo:(GLKVector3)translation;
/** An animation that translates to the given position over the given duration. 
    @note This animation is reversible, and translates from the given position to the current position. */
+ (BHGLBasicAnimation *)translateTo:(GLKVector3)translation withDuration:(NSTimeInterval)duration;

/** An animation that immediately translates by the given position.
    @note This animation is reversible, and translates by the inverse vector. */
+ (BHGLBasicAnimation *)translateBy:(GLKVector3)translation;
/** An animation that translates by the given position over the given duration. 
    @note This animation is reversible, and translates by the inverse vector. */
+ (BHGLBasicAnimation *)translateBy:(GLKVector3)translation withDuration:(NSTimeInterval)duration;

@end
