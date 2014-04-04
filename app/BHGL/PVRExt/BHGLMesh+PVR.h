//
//  BHGLMesh+PVR.h
//
//  Created by John Visentin on 10/15/13.
//  Copyright (c) 2013 Brockenhaus Studio. All rights reserved.
//

#import "BHGLMesh.h"

class SPODMesh;

@interface BHGLMesh (PVR)

- (id)initWithSPODMesh:(const SPODMesh &)mesh;

@end
