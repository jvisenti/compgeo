//
//  BHGLAnimatedObject.m
//
//  Created by John Visentin on 12/6/13.
//  Copyright (c) 2013 Brockenhaus Studio. All rights reserved.
//

#import "BHGLAnimatedObject.h"
#import "BHGLMath.h"

@interface BHGLAnimatedObject ()
{
    GLKMatrix4 *_cachedModelMatrix;
}

@property (nonatomic, strong) NSMutableArray *animationQueue;
@property (nonatomic, strong) NSMutableArray *animations;

- (void)invalidateModelMatrixCache;

@end

@implementation BHGLAnimatedObject

- (id)init
{
    if ((self = [super init]))
    {
        self.position = GLKVector3Make(0.0f, 0.0f, 0.0f);
        self.rotation = GLKQuaternionIdentity;
        self.scale = GLKVector3Make(1.0f, 1.0f, 1.0f);
        
        _cachedModelMatrix = NULL;
    }
    
    return self;
}

- (GLKMatrix4)modelMatrix
{
    if (!_cachedModelMatrix)
    {
        GLKMatrix4 scale = GLKMatrix4MakeScale(self.scale.x, self.scale.y, self.scale.z);
        GLKMatrix4 rotation = GLKMatrix4MakeWithQuaternion(self.rotation);
        
        GLKMatrix4 mat = GLKMatrix4Multiply(rotation, scale);
        
        // shortcut to apply a translation transformation
        mat.m[12] += self.position.x;
        mat.m[13] += self.position.y;
        mat.m[14] += self.position.z;
    
        _cachedModelMatrix = (GLKMatrix4 *)malloc(sizeof(GLKMatrix4));
        memcpy(_cachedModelMatrix, &mat, sizeof(GLKMatrix4));
    }
    
    return *_cachedModelMatrix;
}

- (void)setPosition:(GLKVector3)position
{
    _position = position;
    [self invalidateModelMatrixCache];
}

- (void)setScale:(GLKVector3)scale
{
    _scale = scale;
    [self invalidateModelMatrixCache];
}

- (void)setRotation:(GLKQuaternion)rotation
{
    _rotation = rotation;
    [self invalidateModelMatrixCache];
}

- (void)setTransform:(GLKMatrix4)transform
{
//    BHGLMatrix4Decompose(transform, &_position, &_rotation, &_scale);
//    [self invalidateMatrixCache];
}

- (NSArray *)activeAnimations
{
    return [self.animations copy];
}

- (void)runAnimation:(BHGLAnimation *)animation
{
    [self.animations addObject:animation];
}

- (void)enqueueAnimation:(BHGLAnimation *)animation
{
    [self.animationQueue addObject:animation];
}

- (void)removeAnimation:(BHGLAnimation *)animation
{
    [self.animationQueue removeObject:animation];
    [self.animations removeObject:animation];
}

- (void)removeAllAnimations
{
    [self.animationQueue removeAllObjects];
    [self.animations removeAllObjects];
}

- (void)update:(NSTimeInterval)dt
{
    [[self.animations copy] enumerateObjectsUsingBlock:^(BHGLAnimation *anim, NSUInteger idx, BOOL *stop) {
        [anim applyToObject:self];
        
        if (anim.isFinished)
        {
            [self.animations removeObject:anim];
        }
        
        [anim update:dt];
    }];
    
    if ([self.animations count] == 0 && [self.animationQueue count] > 0)
    {
        [self runAnimation:[self.animationQueue objectAtIndex:0]];
        [self.animationQueue removeObjectAtIndex:0];
    }
}

#pragma mark - private interface

- (NSMutableArray *)animationQueue
{
    if (!_animationQueue)
    {
        _animationQueue = [NSMutableArray array];
    }
    return _animationQueue;
}

- (NSMutableArray *)animations
{
    if (!_animations)
    {
        _animations = [NSMutableArray array];
    }
    return _animations;
}

- (void)invalidateModelMatrixCache
{
    if (_cachedModelMatrix)
    {
        free(_cachedModelMatrix);
        _cachedModelMatrix = NULL;
    }
}

@end
