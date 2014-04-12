//
//  BHGLKeyframeAnimation.m
//
//  Created by John Visentin on 10/21/13.
//  Copyright (c) 2013 Brockenhaus Studio. All rights reserved.
//

#import "BHGLKeyframeAnimation.h"
#import "BHGLCUtils.h"
#import "BHGLNode.h"

@interface BHGLKeyframeAnimation ()
{
    BHGLAnimationInfo _animationInfo;
}

@end

@implementation BHGLKeyframeAnimation

#pragma mark - initialization

+ (BHGLKeyframeAnimation *)animationWithFrames:(unsigned int)numFrames fps:(unsigned int)fps
{
    BHGLAnimationInfo animInfo = BHGLAnimationInfoCreate(numFrames, fps);
    
    id animation = [[BHGLKeyframeAnimation alloc] initWithAnimationInfo:animInfo];
    
    BHGLAnimationInfoFree(animInfo);
    
    return animation;
}

- (id)initWithAnimationInfo:(BHGLAnimationInfo)animationInfo
{
    if ((self = [super init]))
    {
        BHGLAnimationInfoCopy(&_animationInfo, &animationInfo);
        
        if (animationInfo.transform)
        {
            void (*t)(GLfloat(*)[16]) = animationInfo.transform;
            self.transform = ^(GLKMatrix4 *m) {
                t(&m->m);
            };
        }
        
        float frames = MAX(1, animationInfo.numFrames) - 1;
        _duration = animationInfo.fps ? (frames / animationInfo.fps) : 0.0;
    }
    return self;
}

#pragma mark - NSCopying

- (id)copyWithZone:(NSZone *)zone
{
    BHGLKeyframeAnimation *copy = [super copyWithZone:zone];
    BHGLAnimationInfoCopy(&(copy->_animationInfo), &_animationInfo);
    copy.transform = self.transform;
    
    return copy;
}

#pragma mark - public interface

- (void)applyToObject:(BHGLAnimatedObject *)object
{
    if (self.isFinished)
    {
        return;
    }
    
    int frame = (int)self.currentFrame;
    
    GLKVector3 s;
    GLKQuaternion r;
    GLKVector3 t;
    
    GLKMatrix4 *matrix = NULL;
    if (_animationInfo.matrices[frame])
    {
        GLKMatrix4 modelMatrix = GLKMatrix4MakeWithArray(_animationInfo.matrices[frame]);
        
        if (self.transform)
        {
            self.transform(&modelMatrix);
        }
        
        matrix = &modelMatrix;
    }
    else
    {
        s = [self scale];
        
        r = [self rotation];
        
        t = [self translation];
        
        if (self.transform)
        {
            GLKMatrix4 scale = GLKMatrix4MakeScale(s.x, s.y, s.z);
            GLKMatrix4 rotation = GLKMatrix4MakeWithQuaternion(r);
            
            GLKMatrix4 modelMatrix = GLKMatrix4Multiply(rotation, scale);
            
            modelMatrix.m[12] += t.x;
            modelMatrix.m[13] += t.y;
            modelMatrix.m[14] += t.z;
            
            self.transform(&modelMatrix);
            
            matrix = &modelMatrix;
        }
    }
    
    if (matrix)
    {
        [object setTransform:*matrix];
    }
    else
    {
        object.scale = s;
        object.rotation = r;
        object.position = t;
    }
    
    _animationFlags.repeated = NO;
    _animationFlags.started = YES;
    
    if (self.currentTime >= self.duration && !self.repeats)
    {
        _animationFlags.finished = YES;
    }
}

- (instancetype)reversedAnimation
{    
    return [super reversedAnimation];
}

- (void)dealloc
{
    BHGLAnimationInfoFree(_animationInfo);
}

- (unsigned int)numFrames
{
    return _animationInfo.numFrames;
}

- (unsigned int)fps
{
    return _animationInfo.fps;
}

- (void)setFps:(unsigned int)fps
{
    _animationInfo.fps = fps;
    
    float frames = MAX(1, self.numFrames) - 1;
    _duration = fps ? (frames / (float)fps) : 0.0;
}

- (float)currentFrame
{
    if (self.duration == 0 || self.numFrames == 0)
    {
        return 0.0f;
    }
    
    float frame = (self.currentTime / self.duration) * (self.numFrames - 1);
    
    if (_animationFlags.reversed)
    {
        frame = (self.numFrames - 1) - frame;
    }
    
    return frame;
}

- (GLKVector3)scale
{
    unsigned int current = (unsigned int)self.currentFrame;
    unsigned int next = current < (int)self.numFrames - 1 ? current + 1 : 0;
    float partial = self.currentFrame - current;
    
    return GLKVector3Lerp([self scaleForFrame:current], [self scaleForFrame:next], partial);
}

- (GLKQuaternion)rotation
{
    unsigned int current = (unsigned int)self.currentFrame;
    unsigned int next = current < (int)self.numFrames - 1 ? current + 1 : 0;
    float partial = self.currentFrame - current;
    
    return GLKQuaternionSlerp([self rotationForFrame:current], [self rotationForFrame:next], partial);
}

- (GLKVector3)translation
{
    unsigned int current = (unsigned int)self.currentFrame;
    unsigned int next = current < (int)self.numFrames - 1 ? current + 1 : 0;
    float partial = self.currentFrame - current;
    
    return GLKVector3Lerp([self translationForFrame:current], [self translationForFrame:next], partial);
}

- (GLKVector3)scaleForFrame:(unsigned int)frame
{
    return GLKVector3Make(_animationInfo.scales[frame][0], _animationInfo.scales[frame][1], _animationInfo.scales[frame][2]);
}

- (GLKQuaternion)rotationForFrame:(unsigned int)frame
{
    return GLKQuaternionMake(_animationInfo.rotations[frame][0], _animationInfo.rotations[frame][1], _animationInfo.rotations[frame][2], _animationInfo.rotations[frame][3]);
}

- (GLKVector3)translationForFrame:(unsigned int)frame
{
    return GLKVector3Make(_animationInfo.translations[frame][0], _animationInfo.translations[frame][1], _animationInfo.translations[frame][2]);
}

- (void)setScale:(GLKVector3)scale forFrame:(unsigned int)frame
{
    if (frame < self.numFrames)
    {
        _animationInfo.scales[frame][0] = scale.x;
        _animationInfo.scales[frame][1] = scale.y;
        _animationInfo.scales[frame][2] = scale.z;
    }
}

- (void)setRotation:(GLKQuaternion)rotation forFrame:(unsigned int)frame
{
    if (frame < self.numFrames)
    {
        _animationInfo.rotations[frame][0] = rotation.x;
        _animationInfo.rotations[frame][1] = rotation.y;
        _animationInfo.rotations[frame][2] = rotation.z;
        _animationInfo.rotations[frame][3] = rotation.s;
    }
}

- (void)setTranslation:(GLKVector3)translation forFrame:(unsigned int)frame
{
    if (frame < self.numFrames)
    {
        _animationInfo.translations[frame][0] = translation.x;
        _animationInfo.translations[frame][1] = translation.y;
        _animationInfo.translations[frame][2] = translation.z;
    }
}

- (void)setMatrix:(GLKMatrix4)matrix forFrame:(unsigned int)frame
{
    if (frame < self.numFrames)
    {
        if (_animationInfo.matrices[frame] == NULL)
        {
            _animationInfo.matrices[frame] = malloc(16*sizeof(GLfloat));
        }
        
        memcpy(_animationInfo.matrices[frame], matrix.m, 16*sizeof(GLfloat));
    }
}

@end
