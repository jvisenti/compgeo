//
//  BHGLMaterial+PVR.h
//
//  Created by John Visentin on 12/18/13.
//  Copyright (c) 2013 Brockenhaus Studio. All rights reserved.
//

#import "BHGLMaterial.h"

class SPODMaterial;

@interface BHGLMaterial (PVR)

- (id)initWithSPODMaterial:(const SPODMaterial &)material;

@end
