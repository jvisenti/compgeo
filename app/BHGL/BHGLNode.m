//
//  BHGLNode.m
//
//  Created by John Visentin on 10/17/13.
//  Copyright (c) 2013 Brockenhaus Studio. All rights reserved.
//

#import "BHGLNode.h"
#import "BHGLMath.h"

@interface BHGLNode ()
{
    __weak BHGLScene *_scene;
}

@property (nonatomic, strong) NSMutableArray *mutableChildren;

- (void)removeChild:(BHGLNode *)node;
- (void)setParent:(BHGLNode *)parent;

@end

@implementation BHGLNode

@synthesize scene = _scene;

- (id)init
{
    if ((self = [super init]))
    {        
        self.viewMatrix = GLKMatrix4Identity;
        self.projectionMatrix = GLKMatrix4Identity;
    }
    return self;
}

#pragma mark - property overrides

- (NSArray *)children
{
    return [self.mutableChildren copy];
}

- (BHGLProgram *)program
{
    BHGLProgram *program = _program;
    if (!program)
    {
        program = [self.parent program];
    }
    
    return program;
}

#pragma mark - public interface

- (GLKMatrix4)computeViewMatrixRecursive:(BOOL)recursive
{
    GLKMatrix4 viewMatrix = self.viewMatrix;
    
    if (recursive && self.parent)
    {
        viewMatrix = GLKMatrix4Multiply([self.parent computeViewMatrixRecursive:YES], viewMatrix);
    }
    
    return viewMatrix;
}

- (GLKMatrix4)computeProjectionMatrixRecursive:(BOOL)recursive
{
    GLKMatrix4 projectionMatrix = self.projectionMatrix;
    
    if (recursive && self.parent)
    {
        projectionMatrix = GLKMatrix4Multiply([self.parent computeProjectionMatrixRecursive:YES], projectionMatrix);
    }
    
    return projectionMatrix;
}

- (GLKMatrix4)computeModelMatrixRecursive:(BOOL)recursive
{
    GLKMatrix4 modelMatrix = self.modelMatrix;
    
    if (recursive && self.parent)
    {
        modelMatrix = GLKMatrix4Multiply([self.parent computeModelMatrixRecursive:YES], modelMatrix);
    }
    
    return modelMatrix;
}

- (void)addChild:(BHGLNode *)node
{
    NSAssert([node isKindOfClass:[BHGLNode class]], @"Attempted to add invalid object %@ as a child of BHGLNode. Children of BHGLNodes must be subclasses of BHGLNode.", [node class]);
    NSAssert(node != self, @"Can't add a BHGLNode as a child of itself!");
    
    [node removeFromParent];
    [self.mutableChildren addObject:node];
    [node setValue:self forKey:@"parent"];
}

- (void)addChildren:(NSArray *)nodes
{
    [nodes enumerateObjectsUsingBlock:^(BHGLNode *node, NSUInteger idx, BOOL *stop) {
        [self addChild:node];
    }];
}

- (BHGLNode *)childNamed:(NSString *)name
{
    __block BHGLNode *c = nil;
    [self.children enumerateObjectsUsingBlock:^(BHGLNode *child, NSUInteger idx, BOOL *stop)
     {
         if ([child.name isEqualToString:name])
         {
             c = child;
             *stop = YES;
         }
     }];
    
    return c;
}

- (NSArray *)childrenNamed:(NSString *)name
{
    NSIndexSet *indices = [self.children indexesOfObjectsPassingTest:^BOOL(BHGLNode *child, NSUInteger idx, BOOL *stop) {
        return [child.name isEqualToString:name];
    }];
    
    return [self.mutableChildren objectsAtIndexes:indices];
}

- (void)removeFromParent
{
    if (self.parent)
    {
        [self.parent performSelector:@selector(removeChild:) withObject:self];
        self.parent = nil;
    }
}

- (void)updateRecursive:(NSTimeInterval)dt
{
    [self update:dt];
    
    [self.children enumerateObjectsUsingBlock:^(BHGLNode *child, NSUInteger idx, BOOL *stop) {
        [child updateRecursive:dt];
    }];
}

- (void)configureProgram:(BHGLProgram *)program
{
    [program use];
}

#pragma mark - BHGLRenderedObject

- (void)update:(NSTimeInterval)dt
{
    [super update:dt];
}

- (void)render
{
    [self.children enumerateObjectsUsingBlock:^(BHGLNode *child, NSUInteger idx, BOOL *stop) {
        [child render];
    }];
}

- (void)renderWithProgram:(BHGLProgram *)program
{
    [self.children enumerateObjectsUsingBlock:^(BHGLNode *child, NSUInteger idx, BOOL *stop) {
        [child renderWithProgram:program];
    }];
}

#pragma mark - private interface

- (NSMutableArray *)mutableChildren
{
    if (!_mutableChildren)
    {
        _mutableChildren = [[NSMutableArray alloc] init];
    }
    return _mutableChildren;
}

- (void)removeChild:(BHGLNode *)node
{
    [self.mutableChildren removeObject:node];
}

- (void)setParent:(BHGLNode *)parent
{
    _parent = parent;
    
    _scene = parent.scene;
}

@end
