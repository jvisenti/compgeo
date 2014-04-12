//
//  BHGLCamera.m
//
//  Created by John Visentin on 10/18/13.
//  Copyright (c) 2013 Brockenhaus Studio. All rights reserved.
//

#import "BHGLCamera.h"
#import "BHGLMath.h"
#import "BHGLBasicAnimation.h"

@interface BHGLCamera ()
{
    GLKMatrix4 *_cachedProjectionMatrix;
}

- (void)invalidateProjectionMatrixCache;

@end

@implementation BHGLCamera

- (id)init
{
    if ((self = [super init]))
    {
        _type = BHGLCameraTypePerspective;
        self.up = GLKVector3Make(0.0f, 1.0f, 0.0f);
        
        _cachedProjectionMatrix = NULL;
    }
    return self;
}

- (id)initWithFieldOfView:(float)fov aspectRatio:(float)aspectRatio nearClippingPlane:(float)near farClippingPlane:(float)far
{
    if ((self = [self init]))
    {
        _fieldOfView = fov;
        _aspectRatio = aspectRatio;
        _near = near;
        _far = far;
    }
    return self;
}

- (id)initWithLeft:(float)left right:(float)right top:(float)top bottom:(float)bottom nearClippingPlane:(float)near farClippingPlane:(float)far
{
    if ((self = [self init]))
    {
        _type = BHGLCameraTypeOrthographic;
        
        _left = left;
        _right = right;
        _top = top;
        _bottom = bottom;
        
        _near = near;
        _far = far;
    }
    return self;
}

#pragma mark - property overrides

- (void)setType:(BHGLCameraType)type
{
    _type = type;
    [self invalidateProjectionMatrixCache];
}

- (void)setUp:(GLKVector3)up
{
    _up = GLKVector3Normalize(up);
}

- (void)setFieldOfView:(float)fieldOfView
{
    _fieldOfView = fieldOfView;
    [self invalidateProjectionMatrixCache];
}

- (void)setAspectRatio:(float)aspectRatio
{
    _aspectRatio = aspectRatio;
    [self invalidateProjectionMatrixCache];
}

- (void)setLeft:(float)left
{
    _left = left;
    [self invalidateProjectionMatrixCache];
}

- (void)setRight:(float)right
{
    _right = right;
    [self invalidateProjectionMatrixCache];
}

- (void)setBottom:(float)bottom
{
    _bottom = bottom;
    [self invalidateProjectionMatrixCache];
}

- (void)setTop:(float)top
{
    _top = top;
    [self invalidateProjectionMatrixCache];
}

- (void)setNear:(float)near
{
    _near = near;
    [self invalidateProjectionMatrixCache];
}

- (void)setFar:(float)far
{
    _far = far;
    [self invalidateProjectionMatrixCache];
}

- (GLKMatrix4)projectionMatrix
{
    if (!_cachedProjectionMatrix)
    {
        GLKMatrix4 proj = GLKMatrix4Identity;
        
        if (self.type == BHGLCameraTypePerspective)
        {
            proj = GLKMatrix4MakePerspective(self.fieldOfView, self.aspectRatio, self.near, self.far);
        }
        else if (self.type == BHGLCameraTypeOrthographic)
        {
            proj = GLKMatrix4MakeOrtho(self.left, self.right, self.bottom, self.top, self.near, self.far);
        }
        
        _cachedProjectionMatrix = (GLKMatrix4 *)malloc(sizeof(GLKMatrix4));
        memcpy(_cachedProjectionMatrix, &proj, sizeof(GLKMatrix4));
    }
    
    
    return *_cachedProjectionMatrix;
}

- (GLKMatrix4)viewMatrix
{    
    GLKMatrix4 camWorldMatrix = self.modelMatrix;
    
    GLKVector3 from = GLKVector3Make(camWorldMatrix.m[12], camWorldMatrix.m[13], camWorldMatrix.m[14]);
    GLKVector3 to;
    GLKVector3 up;
    
    if (self.target)
    {
        GLKMatrix4 targetModelMatrix = self.target.modelMatrix;
        to.x = targetModelMatrix.m[12];
        to.y = targetModelMatrix.m[13];
        to.z = targetModelMatrix.m[14];
        
        up = GLKVector3Make(0.0f, 1.0f, 0.0f);
    }
    else
    {
        GLKVector4 forward = GLKVector4Make(self.up.x, self.up.z, -self.up.y, 1);
        GLKVector4 to4 = GLKMatrix4MultiplyVector4(camWorldMatrix, forward);
        
        to.x = to4.x;
        to.y = to4.y;
        to.z = to4.z;
        
        up = GLKMatrix4MultiplyVector3(camWorldMatrix, self.up);
    }
    
    return BHGLMatrix4LookAt(from, to, up);
}

#pragma mark - private interface

- (void)invalidateProjectionMatrixCache
{
    if (_cachedProjectionMatrix)
    {
        free(_cachedProjectionMatrix);
        _cachedProjectionMatrix = NULL;
    }
}

@end
