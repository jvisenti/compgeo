//
//  BHGLScene.h
//
//  Created by John Visentin on 10/19/13.
//  Copyright (c) 2013 Brockenhaus Studio. All rights reserved.
//

#import "BHGLNode.h"
#import "BHGLCamera.h"
#import "BHGLLight.h"

@interface BHGLScene : BHGLNode

/** The camera through which to render the scene. Setting the active camera with a camera that has not
    yet been added to the scene has no effect.
    @note The active camera's view matrix is used instead of the scene's view matrix property. */
@property (nonatomic, strong) BHGLCamera *activeCamera;
@property (nonatomic, readonly) NSArray *cameras;

/** All lights in the scene. When the scene is rendered, it sets the uniform values for the light array
 uniform specified by the lightUniform property. */
@property (nonatomic, copy) NSArray *lights;

/** If this property is non-nil, the scene passes the lights array as uniforms into the uniform specified
    by the lightUniform property. Default is nil. */
@property (nonatomic, copy) NSString *lightUniform;

/** Adds a camera to the scene. If the activeCamera property is not yet set, this method sets it. */
- (void)addCamera:(BHGLCamera *)camera;

/** Removes a camera from the scene. 
    If the camera is the active camera this method sets the activeCamera property to nil. */
- (void)removeCamera:(BHGLCamera *)camera;

/** Returns the first camera in the cameras array with the given name. */
- (BHGLCamera *)cameraNamed:(NSString *)name;

/** Adds a light to the scene. When the scene is rendered, it sets the uniform values for the light array
    uniform specified by the lightUniform property. */
- (void)addLight:(BHGLLight *)light;

/** Removes a light from the scene. */
- (void)removeLight:(BHGLLight *)light;

/** Returns the first light in the light array with the given name. */
- (BHGLLight *)lightNamed:(NSString *)name;

@end
