//
//  NSValue+BHGLTypes.h
//
//  Created by John Visentin on 10/16/13.
//  Copyright (c) 2013 Brockenhaus Studio. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "BHGLTypes.h"

@interface NSValue (BHGLTypes)

- (BHGLVertexAttribInfo)BHGLVertexAttribInfoValue;
+ (NSValue *)BHGLvalueWithVertexAttribInfo:(BHGLVertexAttribInfo)info;

- (BHGLTextureInfo)BHGLTextureInfoValue;
+ (NSValue *)BHGLvalueWithTextureInfo:(BHGLTextureInfo)info;

@end
