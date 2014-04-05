//
//  BHGLNode.h
//
//  Created by John Visentin on 10/17/13.
//  Copyright (c) 2013 Brockenhaus Studio. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <GLKit/GLKMath.h>
#import "BHGLAnimatedObject.h"
#import "BHGLRenderedObject.h"

@class BHGLScene;

@interface BHGLNode : BHGLAnimatedObject <BHGLRenderedObject>

/** An descriptive name for use by your application. */
@property (nonatomic, copy) NSString *name;

/** The parent node in the node heirarchy. A node can only be the child of one parent at a time. */
@property (nonatomic, readonly) BHGLNode *parent;

/** The node's immediate children in the order they were added. */
@property (nonatomic, readonly) NSArray *children;

/** The scene in which the node is located. */
@property (nonatomic, readonly) BHGLScene *scene;

/** The view matrix with which to render the node. Generally this is set by a camera,
    and is not altered directly. Default is the identity matrix.
    @note When computing a node's view matrix for rendering, parent view matrices are recursively applied. */
@property (nonatomic, assign) GLKMatrix4 viewMatrix;
- (GLKMatrix4)computeViewMatrixRecursive:(BOOL)recursive;

/** The projection matrix with which to render the node. Generally this is set by a camera,
 and is not altered directly. Default is the identity matrix.
 @note When computing a node's projection matrix for rendering, parent matrices are recursively applied. */
@property (nonatomic, assign) GLKMatrix4 projectionMatrix;
- (GLKMatrix4)computeProjectionMatrixRecursive:(BOOL)recursive;

/** The model matrix computed from current position, rotation, and scale.
    @note When computing a node's model matrix for rendering, parent model matrices are recursively applied. */
@property (nonatomic, readonly) GLKMatrix4 modelMatrix;
- (GLKMatrix4)computeModelMatrixRecursive:(BOOL)recursive;

/** The program with which to render the node. If none is specified, the node uses its parent program. */
@property (nonatomic, strong) BHGLProgram *program;

/** Adds the node as a child of the receiver, adding it to the node heirarchy.
    When a node is updated or rendered, its children are also updated or rendered. */
- (void)addChild:(BHGLNode *)node;

/** Adds the array of nodes as a children of the receiver, adding them to the node heirarchy.
    When a node is updated or rendered, its children are also updated or rendered. */
- (void)addChildren:(NSArray *)nodes;

/** Returns the first child in the children array with the given name. */
- (BHGLNode *)childNamed:(NSString *)name;

/** Returns all children with the given name. */
- (NSArray *)childrenNamed:(NSString *)name;

/** Removes the receiver from the node heirarchy. */
- (void)removeFromParent;

/** Performs updates prior to rendering including advancing all animations.
    Subclasses should call the super implementation. */
- (void)update:(NSTimeInterval)dt;

/** Updates the receiver, then recursively updates all children. 
    @note This is equivalent to calling @code - (void)update:(NSTimeInterval)dt @endcode
    On every node in the subtree rooted at the receiver. */
- (void)updateRecursive:(NSTimeInterval)dt;

/** Configures the given program for rendering the receiver.
 Subclasses should override to provide more specific functionality.
 @note Subclasses should call the super implementation. */
- (void)configureProgram:(BHGLProgram *)program;

/** Renders the node and all children. Default implementation simply renders children.
    Subclasses should call the super implementation at the END of their own implementations. */
- (void)render;

/** Renders the node and all children with the given program. Default implementation simply renders children.
    Subclasses should call the super implementation at the END of their own implementations. */
- (void)renderWithProgram:(BHGLProgram *)program;

@end
