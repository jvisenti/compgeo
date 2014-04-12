//
//  MPCube.m
//  MotionPlanner
//
//  Created by John Visentin on 4/12/14.
//  Copyright (c) 2014 Brockenhaus Studio. All rights reserved.
//

#import "MPCube.h"
#import "BHGLCUtils.h"

const GLfloat Vertices[24][6] = {
    // Front
    {0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f},
    {0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f},
    {-0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f},
    {-0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f},
    // Back
    {0.5, -0.5, -0.5f, 0.0f, 0.0f, -1.0f},
    {-0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f},
    {-0.5, 0.5, -0.5f, 0.0f, 0.0f, -1.0f},
    {0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f},
    // Left
    {-0.5f, -0.5f, 0.5f, -1.0f, 0.0f, 0.0f},
    {-0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f},
    {-0.5f, 0.5f, -0.5f, -1.0f, 0.0f, 0.0f},
    {-0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f},
    // Right
    {0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f},
    {0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f},
    {0.5f, 0.5, 0.5, 1.0f, 0.0f, 0.0f},
    {0.5, -0.5, 0.5, 1.0f, 0.0f, 0.0f},
    // Top
    {0.5, 0.5, 0.5, 0.0f, 1.0f, 0.0f},
    {0.5, 0.5, -0.5, 0.0f, 1.0f, 0.0f},
    {-0.5, 0.5, -0.5, 0.0f, 1.0f, 0.0f},
    {-0.5, 0.5, 0.5, 0.0f, 1.0f, 0.0f},
    // Bottom
    {0.5, -0.5, -0.5, 0.0f, -1.0f, 0.0f},
    {0.5, -0.5, 0.5, 0.0f, -1.0f, 0.0f},
    {-0.5, -0.5, 0.5, 0.0f, -1.0f, 0.0f},
    {-0.5, -0.5f, -0.5, 0.0f, -1.0f, 0.0f}
};

const GLuint Indices[] = {
    // Front
    0, 1, 2,
    2, 3, 0,
    // Back
    4, 5, 6,
    6, 7, 4,
    // Left
    8, 9, 10,
    10, 11, 8,
    // Right
    12, 13, 14,
    14, 15, 12,
    // Top
    16, 17, 18,
    18, 19, 16,
    // Bottom
    20, 21, 22,
    22, 23, 20
};

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

static MPMesh *SharedMesh;
+ (MPMesh *)sharedMesh
{
    static MPMesh *SharedMesh;
    
    static dispatch_once_t onceToken;
    dispatch_once(&onceToken, ^{
        
        SharedMesh = MPMeshCreate((const MPVec3 *)Vertices, sizeof(Vertices[0]), sizeof(Vertices)/sizeof(Vertices[0]), (const void *)Indices, sizeof(Indices[0]), sizeof(Indices)/sizeof(Indices[0]));
        MPMeshRetain(SharedMesh);
    });
    
    return SharedMesh;
}

@end
