//
//  MPCube.m
//  MotionPlanner
//
//  Created by John Visentin on 4/12/14.
//  Copyright (c) 2014 Brockenhaus Studio. All rights reserved.
//

#import "MPCube.h"
#import "BHGLCUtils.h"

@interface MPCube ()

+ (MPMesh *)sharedMesh;

@end

@implementation MPCube

- (id)init
{
    if ((self = [super init]))
    {
        MP::Model *model = new MP::Model([MPCube sharedMesh]);
        [super setModel:model];
    }
    
    return self;
}

- (id)initWithMesh:(BHGLMesh *)mesh material:(BHGLMaterial *)material
{
    return [self init];
}

- (id)initWithModel:(MP::Model *)model
{
    return [self init];
}

- (void)setModel:(MP::Model *)model
{
    // does nothing. model can't be changed from a cube
}

- (void)dealloc
{
    // model was allocd locally, so cube is also responsible for freeing it
    delete self.model;
}

#pragma mark - private methods

+ (MPMesh *)sharedMesh
{
    static MPMesh *SharedMesh;
    
    static dispatch_once_t onceToken;
    dispatch_once(&onceToken, ^{
        
        SharedMesh = MPMeshCreateCube();
        MPMeshRetain(SharedMesh);
    });
    
    return SharedMesh;
}

@end
