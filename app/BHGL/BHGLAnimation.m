//
//  BHGLAnimation.m
//
//  Created by John Visentin on 12/1/13.
//  Copyright (c) 2013 Brockenhaus Studio. All rights reserved.
//

#import "BHGLAnimation.h"

@interface BHGLAnimation ()

- (void)setCurrentTime:(NSTimeInterval)currentTime;
- (void)setStarted:(BOOL)started;
- (void)setFinished:(BOOL)finished;

@end

/** PRIVATE CLASSES */

@interface BHGLSequenceAnimation : BHGLAnimation

@property (nonatomic, copy) NSArray *animations;

- (id)initWithAnimations:(NSArray *)animations;

@end

@implementation BHGLSequenceAnimation

- (id)initWithAnimations:(NSArray *)animations
{
    if ((self = [super init]))
    {
        self.animations = animations;
    }
    return self;
}

- (void)setAnimations:(NSArray *)animations
{
    _animations = [animations copy];
    
    _duration = -1;
}

- (id)copyWithZone:(NSZone *)zone
{
    BHGLSequenceAnimation *copy = [super copyWithZone:zone];
    
    NSMutableArray *deepCopy = [NSMutableArray arrayWithCapacity:[self.animations count]];
    [self.animations enumerateObjectsUsingBlock:^(BHGLAnimation *anim, NSUInteger idx, BOOL *stop) {
        [deepCopy addObject:[anim copy]];
    }];
    
    copy.animations = deepCopy;
    
    return copy;
}

- (NSTimeInterval)duration
{
    if (_duration < 0)
    {
        __block NSTimeInterval duration = 0.0f;
        
        [self.animations enumerateObjectsUsingBlock:^(BHGLAnimation *anim, NSUInteger idx, BOOL *stop) {
            duration += anim.duration;
        }];
        
        _duration = duration;
    }
    
    
    return _duration;
}

- (void)setStarted:(BOOL)started
{
    [super setStarted:started];
    
    if (!started)
    {
        [self.animations enumerateObjectsUsingBlock:^(BHGLAnimation *anim, NSUInteger idx, BOOL *stop) {
            anim.started = NO;
            anim.finished = NO;
        }];
    }
}

- (instancetype)reversedAnimation
{
    BHGLSequenceAnimation *reverse = [[BHGLSequenceAnimation alloc] init];
    reverse.repeats = self.repeats;
    reverse->_animationFlags.reversed = ~_animationFlags.reversed;
    
    NSMutableArray *reversedAnimations = [NSMutableArray arrayWithCapacity:[self.animations count]];
    
    for (int i = (int)[self.animations count]-1; i >= 0; i--)
    {
        [reversedAnimations addObject:[[self.animations objectAtIndex:i] reversedAnimation]];
    }
        
    reverse.animations = reversedAnimations;
    
    return reverse;
}

- (void)applyToObject:(BHGLAnimatedObject *)object
{
    if (self.isFinished)
    {
        return;
    }
    
    if (_animationFlags.repeated)
    {
        // apply the effects of ending the animation if they were skipped
        BHGLAnimation *last = [self.animations lastObject];
        last.currentTime = last.duration;
        [last applyToObject:object];
        
        self.started = NO;
        _animationFlags.repeated = NO;
    }
    
    __block NSTimeInterval time = self.currentTime;
    
    [self.animations enumerateObjectsUsingBlock:^(BHGLAnimation *anim, NSUInteger idx, BOOL *stop) {
        if (anim.repeats || anim.duration >= time)
        {
            anim.currentTime = time;
            [anim applyToObject:object];
            
            *stop = YES;
        }
        else if (anim.isFinished)
        {
            time -= anim.duration;
        }
        else
        {
            anim.currentTime = anim.duration;
            [anim applyToObject:object];
            
            time -= anim.duration;
        }
    }];
    
    self.started = YES;
    
    if (self.currentTime >= self.duration && !self.repeats)
    {
        // apply the effects of ending the animation if they were skipped
        BHGLAnimation *last = [self.animations lastObject];
        last.currentTime = last.duration;
        [last applyToObject:object];
        
        self.finished = YES;
    }
}

@end

@interface BHGLGroupAnimation : BHGLAnimation

@property (nonatomic, copy) NSArray *animations;

- (id)initWithAnimations:(NSArray *)animations;

@end

@implementation BHGLGroupAnimation

- (id)initWithAnimations:(NSArray *)animations
{
    if ((self = [super init]))
    {
        self.animations = animations;
    }
    return self;
}

- (void)setAnimations:(NSArray *)animations
{
    _animations = [animations copy];
    
    _duration = -1;
}

- (id)copyWithZone:(NSZone *)zone
{
    BHGLGroupAnimation *copy = [super copyWithZone:zone];
    
    NSMutableArray *deepCopy = [NSMutableArray arrayWithCapacity:[self.animations count]];
    [self.animations enumerateObjectsUsingBlock:^(BHGLAnimation *anim, NSUInteger idx, BOOL *stop) {
        [deepCopy addObject:[anim copy]];
    }];
    
    copy.animations = deepCopy;
    
    return copy;
}

- (NSTimeInterval)duration
{
    if (_duration < 0)
    {
        __block NSTimeInterval duration = 0.0f;
        
        [self.animations enumerateObjectsUsingBlock:^(BHGLAnimation *anim, NSUInteger idx, BOOL *stop) {
            duration = MAX(duration, anim.duration);
        }];
        
        _duration = duration;
    }
    

    return _duration;
}

- (void)setStarted:(BOOL)started
{
    [super setStarted:started];
    
    if (!started)
    {
        [self.animations enumerateObjectsUsingBlock:^(BHGLAnimation *anim, NSUInteger idx, BOOL *stop) {
            anim.started = NO;
            anim.finished = NO;
        }];
    }
}

- (instancetype)reversedAnimation
{
    BHGLGroupAnimation *reverse = [[BHGLGroupAnimation alloc] init];
    reverse.repeats = self.repeats;
    reverse->_animationFlags.reversed = ~_animationFlags.reversed;
    
    NSMutableArray *reversedAnimations = [NSMutableArray arrayWithCapacity:[self.animations count]];
    
    [self.animations enumerateObjectsUsingBlock:^(BHGLAnimation *anim, NSUInteger idx, BOOL *stop) {
        [reversedAnimations addObject:[anim reversedAnimation]];
    }];
    
    reverse.animations = reversedAnimations;
    
    return reverse;
}

- (void)applyToObject:(BHGLAnimatedObject *)object
{
    if (self.isFinished)
    {
        return;
    }
    
    if (_animationFlags.repeated)
    {
        self.started = NO;
        _animationFlags.repeated = NO;
    }
    
    [self.animations enumerateObjectsUsingBlock:^(BHGLAnimation *anim, NSUInteger idx, BOOL *stop) {
        if (!anim.isFinished)
        {
            anim.currentTime = self.currentTime;
            [anim applyToObject:object];
        }
    }];
    
    self.started = YES;
    
    if (self.currentTime >= self.duration && !self.repeats)
    {
        self.finished = YES;
    }
}

@end

/** END PRIVATE CLASSES */

@implementation BHGLAnimation

#pragma mark - initialization

+ (BHGLAnimation *)sequence:(NSArray *)animations
{
    return [[BHGLSequenceAnimation alloc] initWithAnimations:animations];
}

+ (BHGLAnimation *)group:(NSArray *)animations
{
    return [[BHGLGroupAnimation alloc] initWithAnimations:animations];
}

- (id)init
{
    if ((self = [super init]))
    {
        self.repeats = NO;
    }
    return self;
}

#pragma mark - NSCopying

- (id)copyWithZone:(NSZone *)zone
{
    BHGLAnimation *copy = [[[self class] alloc] init];
    copy->_duration = self.duration;
    copy.repeats = self.repeats;
    copy->_animationFlags.reversed = _animationFlags.reversed;
        
    return copy;
}

#pragma mark - public interface

- (BOOL)isStarted
{
    return _animationFlags.started;
}

-(BOOL)isFinished
{
    return _animationFlags.finished;
}

- (void)applyToObject:(BHGLAnimatedObject *)object
{
    return;
}

- (void)update:(NSTimeInterval)dt
{
    self.currentTime += dt;
}

- (instancetype)reversedAnimation
{
    BHGLAnimation *reverse = [self copy];
    reverse->_animationFlags.reversed = ~_animationFlags.reversed;
    
    return reverse;
}

#pragma mark - private interface

- (void)setCurrentTime:(NSTimeInterval)currentTime
{
    currentTime = MAX(0, currentTime);
    
    if (self.repeats && self.duration > 0.0)
    {
        if (currentTime >= self.duration)
        {
            _animationFlags.repeated = YES;
        }
        
        currentTime = fmodf(currentTime, self.duration);
    }
    else
    {
        if (currentTime < self.duration)
        {
            self.finished = NO;
        }
        
        currentTime = fminf(currentTime, self.duration);
    }
    
    _currentTime = currentTime;
}

- (void)setStarted:(BOOL)started
{
    _animationFlags.started = started;
}

- (void)setFinished:(BOOL)finished
{
    _animationFlags.finished = finished;
}

@end
