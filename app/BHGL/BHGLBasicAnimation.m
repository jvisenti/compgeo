//
//  BHGLBasicAnimation.m
//
//  Created by John Visentin on 12/1/13.
//  Copyright (c) 2013 Brockenhaus Studio. All rights reserved.
//

#import "BHGLBasicAnimation.h"
#import "BHGLNode.h"

typedef void (^BHGLAnimationBlock)(BHGLBasicAnimation *anim, BHGLAnimatedObject *object, NSTimeInterval currentTime, NSTimeInterval totalTime);

@interface BHGLBasicAnimation ()

@property (nonatomic, copy) BHGLAnimationBlock animationBlock;

@property (nonatomic, assign) GLKVector3 startScale;
@property (nonatomic, assign) GLKVector3 endScale;

@property (nonatomic, assign) GLKQuaternion startRotation;
@property (nonatomic, assign) GLKQuaternion endRotation;

@property (nonatomic, assign) GLKVector3 startTranslation;
@property (nonatomic, assign) GLKVector3 endTranslation;

- (BOOL)isReversed;
- (void)setDuration:(NSTimeInterval)duration;

@end

@implementation BHGLBasicAnimation

+ (BHGLBasicAnimation *)wait:(NSTimeInterval)duration
{
    BHGLBasicAnimation *animation = [[BHGLBasicAnimation alloc] init];
    [animation setDuration:duration];
    
    animation.animationBlock = ^(BHGLBasicAnimation *anim, BHGLAnimatedObject *object, NSTimeInterval currentTime, NSTimeInterval totalTime){};;
    
    return animation;
}

+ (BHGLBasicAnimation *)transformWithBlock:(void (^)(BHGLAnimatedObject *, NSTimeInterval, NSTimeInterval))animationBlock duration:(NSTimeInterval)duration
{
    BHGLBasicAnimation *animation = [[BHGLBasicAnimation alloc] init];
    [animation setDuration:duration];
    
    animation.animationBlock = ^(BHGLBasicAnimation *anim, BHGLAnimatedObject *object, NSTimeInterval currentTime, NSTimeInterval totalTime) {
        if (animationBlock && ![anim isReversed])
        {
            animationBlock(object, currentTime, totalTime);
        }
    };
    
    return animation;
}

+ (BHGLBasicAnimation *)runBlock:(void (^)())block
{
    BHGLBasicAnimation *animation = [[BHGLBasicAnimation alloc] init];
    [animation setDuration:0.0];
    
    animation.animationBlock = ^(BHGLBasicAnimation *anim, BHGLAnimatedObject *object, NSTimeInterval currentTime, NSTimeInterval totalTime)
    {
        if (block && ![anim isReversed])
        {
            block();
        }
    };
    
    return animation;
}

#pragma mark - scaling

+ (BHGLBasicAnimation *)scaleTo:(GLKVector3)scale
{
    BHGLBasicAnimation *animation = [[BHGLBasicAnimation alloc] init];
    
    animation.animationBlock = ^(BHGLBasicAnimation *anim, BHGLAnimatedObject *object, NSTimeInterval currentTime, NSTimeInterval totalTime)
    {
        if (![anim isReversed])
        {
            object.scale = scale;
        }
    };
    
    return animation;
}

+ (BHGLBasicAnimation *)scaleTo:(GLKVector3)scale withDuration:(NSTimeInterval)duration
{
    BHGLBasicAnimation *animation = [[BHGLBasicAnimation alloc] init];
    [animation setDuration:duration];
    
    animation.animationBlock = ^(BHGLBasicAnimation *anim, BHGLAnimatedObject *object, NSTimeInterval currentTime, NSTimeInterval totalTime)
    {
        if (currentTime > totalTime)
        {
            return;
        }
        
        if (!anim.isStarted)
        {
            if ([anim isReversed])
            {
                anim.startScale = scale;
                anim.endScale = object.scale;
            }
            else
            {
                anim.startScale = object.scale;
                anim.endScale = scale;
            }
        }
        
        float partial = totalTime ? currentTime / totalTime : 1.0f;
        
        GLKVector3 vec = GLKVector3Lerp(anim.startScale, anim.endScale, partial);
        
        object.scale = vec;
    };
    
    return animation;
}

+ (BHGLBasicAnimation *)scaleBy:(GLKVector3)scale
{
    BHGLBasicAnimation *animation = [[BHGLBasicAnimation alloc] init];
    
    animation.animationBlock = ^(BHGLBasicAnimation *anim, BHGLAnimatedObject *object, NSTimeInterval currentTime, NSTimeInterval totalTime)
    {
        GLKVector3 vec = [anim isReversed] ?  GLKVector3Make(1/scale.x, 1/scale.y, 1/scale.z) : scale;
        
        object.scale = GLKVector3Multiply(vec, object.scale);
    };
    
    return animation;
}

+ (BHGLBasicAnimation *)scaleBy:(GLKVector3)scale withDuration:(NSTimeInterval)duration
{
    BHGLBasicAnimation *animation = [[BHGLBasicAnimation alloc] init];
    [animation setDuration:duration];
    
    animation.animationBlock = ^(BHGLBasicAnimation *anim, BHGLAnimatedObject *object, NSTimeInterval currentTime, NSTimeInterval totalTime)
    {
        if (currentTime > totalTime)
        {
            return;
        }
        
        if (!anim.isStarted)
        {
            anim.startScale = object.scale;
            
            GLKVector3 vec = [anim isReversed] ?  GLKVector3Make(1/scale.x, 1/scale.y, 1/scale.z) : scale;
            anim.endScale = GLKVector3Multiply(vec, object.scale);
        }
        
        float partial = totalTime ? currentTime / totalTime : 1.0f;
        
        object.scale = GLKVector3Lerp(anim.startScale, anim.endScale, partial);
    };
    
    return animation;
}

#pragma mark - rotation

+ (BHGLBasicAnimation *)rotateTo:(GLKQuaternion)rotation
{
    BHGLBasicAnimation *animation = [[BHGLBasicAnimation alloc] init];
    
    animation.animationBlock = ^(BHGLBasicAnimation *anim, BHGLAnimatedObject *object, NSTimeInterval currentTime, NSTimeInterval totalTime)
    {
        if (![anim isReversed])
        {
            object.rotation = rotation;
        }
    };
    
    return animation;
}

+ (BHGLBasicAnimation *)rotateTo:(GLKQuaternion)rotation withDuration:(NSTimeInterval)duration
{
    BHGLBasicAnimation *animation = [[BHGLBasicAnimation alloc] init];
    [animation setDuration:duration];
    
    animation.animationBlock = ^(BHGLBasicAnimation *anim, BHGLAnimatedObject *object, NSTimeInterval currentTime, NSTimeInterval totalTime)
    {
        if (currentTime > totalTime)
        {
            return;
        }
        
        if (!anim.isStarted)
        {
            if ([anim isReversed])
            {
                anim.startRotation = rotation;
                anim.endRotation = object.rotation;
            }
            else
            {
                anim.startRotation = object.rotation;
                anim.endRotation = rotation;
            }
        }
        
        float partial = totalTime ? currentTime / totalTime : 1.0f;
        
        GLKQuaternion quat = GLKQuaternionSlerp(anim.startRotation, anim.endRotation, partial);
        
        object.rotation = quat;
    };
    
    return animation;
}

+ (BHGLBasicAnimation *)rotateBy:(GLKQuaternion)rotation
{
    BHGLBasicAnimation *animation = [[BHGLBasicAnimation alloc] init];
    
    animation.animationBlock = ^(BHGLBasicAnimation *anim, BHGLAnimatedObject *object, NSTimeInterval currentTime, NSTimeInterval totalTime)
    {
        GLKQuaternion quat = [anim isReversed] ?  GLKQuaternionInvert(rotation) : rotation;
        
        object.rotation = GLKQuaternionMultiply(quat, object.rotation);
    };
    
    return animation;
}

+ (BHGLBasicAnimation *)rotateBy:(GLKQuaternion)rotation withDuration:(NSTimeInterval)duration
{
    BHGLBasicAnimation *animation = [[BHGLBasicAnimation alloc] init];
    [animation setDuration:duration];
    
    animation.animationBlock = ^(BHGLBasicAnimation *anim, BHGLAnimatedObject *object, NSTimeInterval currentTime, NSTimeInterval totalTime)
    {
        if (currentTime > totalTime)
        {
            return;
        }
        
        if (!anim.isStarted)
        {
            anim.startRotation = object.rotation;
            
            GLKQuaternion quat = [anim isReversed] ?  GLKQuaternionInvert(rotation) : rotation;
            anim.endRotation = GLKQuaternionMultiply(quat, object.rotation);
        }
        
        float partial = totalTime ? currentTime / totalTime : 1.0f;
        
        object.rotation = GLKQuaternionSlerp(anim.startRotation, anim.endRotation, partial);
    };
    
    return animation;
}

#pragma mark - translation

+ (BHGLBasicAnimation *)translateTo:(GLKVector3)translation
{
    BHGLBasicAnimation *animation = [[BHGLBasicAnimation alloc] init];
    
    animation.animationBlock = ^(BHGLBasicAnimation *anim, BHGLAnimatedObject *object, NSTimeInterval currentTime, NSTimeInterval totalTime)
    {
        if (![anim isReversed])
        {
            object.position = translation;
        }
    };
    
    return animation;
}

+ (BHGLBasicAnimation *)translateTo:(GLKVector3)translation withDuration:(NSTimeInterval)duration
{
    BHGLBasicAnimation *animation = [[BHGLBasicAnimation alloc] init];
    [animation setDuration:duration];
    
    animation.animationBlock = ^(BHGLBasicAnimation *anim, BHGLAnimatedObject *object, NSTimeInterval currentTime, NSTimeInterval totalTime)
    {
        if (currentTime > totalTime)
        {
            return;
        }
        
        if (!anim.isStarted)
        {
            if ([anim isReversed])
            {
                anim.startTranslation = translation;
                anim.endTranslation = object.position;
            }
            else
            {
                anim.startTranslation = object.position;
                anim.endTranslation = translation;
            }
        }
        
        float partial = totalTime ? currentTime / totalTime : 1.0f;
        
        GLKVector3 vec = GLKVector3Lerp(anim.startTranslation, anim.endTranslation, partial);
        
        object.position = vec;
    };
    
    return animation;
}

+ (BHGLBasicAnimation *)translateBy:(GLKVector3)translation
{
    BHGLBasicAnimation *animation = [[BHGLBasicAnimation alloc] init];
    
    animation.animationBlock = ^(BHGLBasicAnimation *anim, BHGLAnimatedObject *object, NSTimeInterval currentTime, NSTimeInterval totalTime)
    {
        GLKVector3 vec = [anim isReversed] ?  GLKVector3Make(-translation.x, -translation.y, -translation.z) : translation;
        
        object.position = GLKVector3Add(vec, object.position);
    };
    
    return animation;
}

+ (BHGLBasicAnimation *)translateBy:(GLKVector3)translation withDuration:(NSTimeInterval)duration
{
    BHGLBasicAnimation *animation = [[BHGLBasicAnimation alloc] init];
    [animation setDuration:duration];
    
    animation.animationBlock = ^(BHGLBasicAnimation *anim, BHGLAnimatedObject *object, NSTimeInterval currentTime, NSTimeInterval totalTime)
    {
        if (currentTime > totalTime)
        {
            return;
        }
        
        if (!anim.isStarted)
        {
            anim.startTranslation = object.position;
            
            GLKVector3 vec = [anim isReversed] ?  GLKVector3Make(-translation.x, -translation.y, -translation.z) : translation;
            anim.endTranslation = GLKVector3Add(vec, object.position);
        }
        
        float partial = totalTime ? currentTime / totalTime : 1.0f;
        
        object.position = GLKVector3Lerp(anim.startTranslation, anim.endTranslation, partial);
    };
    
    return animation;
}

#pragma mark - public interface

- (void)applyToObject:(BHGLAnimatedObject *)object
{
    if (self.animationBlock && !self.isFinished)
    {
        if (_animationFlags.repeated)
        {
            self.animationBlock(self, object, self.duration, self.duration);
            _animationFlags.repeated = NO;
            _animationFlags.started = NO;
        }
        
        self.animationBlock(self, object, self.currentTime, self.duration);
        
        if (self.currentTime >= self.duration && !self.repeats)
        {
            _animationFlags.finished = YES;
        }
    }
    
    _animationFlags.started = YES;
}

#pragma mark - NSCopying

- (id)copyWithZone:(NSZone *)zone
{
    BHGLBasicAnimation *copy = [super copyWithZone:zone];
    copy.animationBlock = self.animationBlock;
    
    return copy;
}

#pragma mark - private interface

- (BOOL)isReversed
{
    return _animationFlags.reversed;
}

- (void)setDuration:(NSTimeInterval)duration
{
    _duration = MAX(0, duration);
}

@end
