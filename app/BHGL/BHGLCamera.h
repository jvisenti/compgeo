//
//  BHGLCamera.h
//
//  Created by John Visentin on 10/18/13.
//  Copyright (c) 2013 Brockenhaus Studio. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <GLKit/GLKMath.h>
#import "BHGLNode.h"

typedef enum
{
    BHGLCameraTypeUnknown = 0,
    BHGLCameraTypePerspective,
    BHGLCameraTypeOrthographic
} BHGLCameraType;

@interface BHGLCamera : BHGLAnimatedObject

/** An descriptive name for use by your application. */
@property (nonatomic, copy) NSString *name;

/** The node the camera should follow. May be null.
    @note The camera maintains a weak reference to the target. */
@property (nonatomic, weak) BHGLNode *target;

/** The unit vector representing the "up" direction, 
    used when camera does not have a target. Default is (0, 1, 0) */
@property (nonatomic, assign) GLKVector3 up;

/** The type of camera. Default is BHGLPerspective. */
@property (nonatomic, assign) BHGLCameraType type;

/** The field of view of the camera in radians. Only used by perspective cameras. */
@property (nonatomic, assign) float fieldOfView;

/** Aspect ratio of the camera (width/height).Only used by perspective cameras. */
@property (nonatomic, assign) float aspectRatio;

/** The left coordinate of the projection volume in eye coordinates. Only used by orthographic cameras. */
@property (nonatomic, assign) float left;

/** The right coordinate of the projection volume in eye coordinates. Only used by orthographic cameras. */
@property (nonatomic, assign) float right;

/** The bottom coordinate of the projection volume in eye coordinates. Only used by orthographic cameras. */
@property (nonatomic, assign) float bottom;

/** The top coordinate of the projection volume in eye coordinates. Only used by orthographic cameras. */
@property (nonatomic, assign) float top;

/** The near clipping plane. Must be positive. */
@property (nonatomic, assign) float near;

/** The far clipping plane. Must be positive. */
@property (nonatomic, assign) float far;

/** The view matrix of the camera, computed from the camera's current position, rotation,
    scale, and its target. */
@property (nonatomic, readonly) GLKMatrix4 viewMatrix;

/** The projection matrix of the camera. */
@property (nonatomic, readonly) GLKMatrix4 projectionMatrix;

/** Initializes a new perspective camera with the given attributes. */
- (id)initWithFieldOfView:(float)fov aspectRatio:(float)aspectRatio nearClippingPlane:(float)near farClippingPlane:(float)far;

/** Initializes a new orthographic camera with the given attributes. */
- (id)initWithLeft:(float)left right:(float)right top:(float)top bottom:(float)bottom nearClippingPlane:(float)near farClippingPlane:(float)far;

@end
