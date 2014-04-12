//
//  BHGLModelNode.h
//
//  Created by John Visentin on 10/16/13.
//  Copyright (c) 2013 Brockenhaus Studio. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "BHGLNode.h"
#import "BHGLMesh.h"
#import "BHGLMaterial.h"

@interface BHGLModelNode : BHGLNode <BHGLRenderedObject>

@property (nonatomic, strong) BHGLMesh *mesh;
@property (nonatomic, strong) BHGLMaterial *material;

- (id)initWithMesh:(BHGLMesh *)mesh material:(BHGLMaterial *)material;

@end
