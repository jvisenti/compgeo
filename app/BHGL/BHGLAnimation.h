//
//  BHGLAnimation.h
//
//  Created by John Visentin on 12/1/13.
//  Copyright (c) 2013 Brockenhaus Studio. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <GLKit/GLKMath.h>

@class BHGLAnimation, BHGLAnimatedObject;

@interface BHGLAnimation : NSObject <NSCopying>
{
    NSTimeInterval _currentTime;
    NSTimeInterval _duration;
    
    struct
    {
        unsigned int started:1;
        unsigned int finished:1;
        unsigned int repeated:1;
        unsigned int reversed:1;
    } _animationFlags;
}

/** The current animation time.
    If the animation repeats, the current time is taken modulo the duration. */
@property (nonatomic, readonly) NSTimeInterval currentTime;

/** The total duration of the animation. May be 0 if model matrix is constant. */
@property (nonatomic, readonly) NSTimeInterval duration;

/** Whether the animation should repeat. Default NO. */
@property (nonatomic, assign) BOOL repeats;

/** Whether the animation has started running. */
@property (nonatomic, readonly, getter = isStarted) BOOL started;

/** Whether the animation has finished running. */
@property (nonatomic, readonly, getter = isFinished) BOOL finished;

/** Applies the effects of the animation at the current time to the given object. 
    The default implementaiton does nothing.
    @note If using BHGLAnimatedObject to manage animations, you should not call this method directly. */
- (void)applyToObject:(BHGLAnimatedObject *)object;

/** Advances the animation time by the given time interval. */
- (void)update:(NSTimeInterval)dt;

/** Creates an animation that reverses the behavior of the receiver.
    Not all animations are reversible. If the receiver is not reversible,
    this method returns an animation with the same behavior as the receiver.
    Refer to the individual method or subclass documentation to see if an animation is reversible.
 */
- (instancetype)reversedAnimation;

/** Creates an animation that runs a collection of animations sequentially.
    Each animation runs to completion before moving on to the next, therefore it is not recommended to add
    repeating animations to a sequence.
    The duration of the sequence animation is the sum of the durations of the animations in the sequence.
    The reverse of this animation is the sequence in reverse order with each animation reversed.
    @note Animations should not be updated once added to a sequence. Update the sequence animation instead.
*/
+ (BHGLAnimation *)sequence:(NSArray *)animations;

/** Creates an animation that runs a collection of animations concurrently.
    Each animation is applied in the order that the animations appear in the group.
    The duration of the group animation is the max of the durations of the animations in the group. 
    The reverse of this animation is a group with each animation reversed.
    @note Animations should not be updated once added to a group. Update the group animation instead.
*/
+ (BHGLAnimation *)group:(NSArray *)animations;

@end
