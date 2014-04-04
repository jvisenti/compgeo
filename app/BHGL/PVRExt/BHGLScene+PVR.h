//
//  BHGLScene+PVR.h
//
//  Created by John Visentin on 10/20/13.
//  Copyright (c) 2013 Brockenhaus Studio. All rights reserved.
//

#import "BHGLScene.h"

class SPODScene;

@class BHGLNode;

@interface BHGLScene (PVR)

#if TARGET_OS_IPHONE
- (id)initWithPODFileNamed:(NSString *)podFileName orientation:(UIInterfaceOrientation)orientation;
- (id)initWithPODFileNamed:(NSString *)podFileName orientation:(UIInterfaceOrientation)orientation nodeClass:(Class)nodeClass;

- (id)initWithSPODScene:(const SPODScene &)scene orientation:(UIInterfaceOrientation)orientation;
- (id)initWithSPODScene:(const SPODScene &)scene orientation:(UIInterfaceOrientation)orientation nodeClass:(Class)nodeClass;
#endif

- (id)initWithPODFileNamed:(NSString *)podFileName;
- (id)initWithPODFileNamed:(NSString *)podFileName nodeClass:(Class)nodeClass;

- (id)initWithSPODScene:(const SPODScene &)scene;
- (id)initWithSPODScene:(const SPODScene &)scene nodeClass:(Class)nodeClass;

+ (BHGLNode *)rootNodeFromPODFileNamed:(NSString *)podFileName;
+ (id)rootNodeFromPODFileNamed:(NSString *)podFileName nodeClass:(Class)nodeClass;

+ (BHGLNode *)rootNodeFromScene:(const SPODScene &)scene;
+ (id)rootNodeFromScene:(const SPODScene &)scene nodeClass:(Class)nodeClass;

@end
