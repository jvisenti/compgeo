//
//  NSValue+BHGLTypes.m
//
//  Created by John Visentin on 10/16/13.
//  Copyright (c) 2013 Brockenhaus Studio. All rights reserved.
//

#import "NSValue+BHGLTypes.h"

@implementation NSValue (BHGLTypes)

- (BHGLVertexAttribInfo)BHGLVertexAttribInfoValue
{
    BHGLVertexAttribInfo info;
    [self getValue:&info];
    
    return info;
}

+ (NSValue *)BHGLvalueWithVertexAttribInfo:(BHGLVertexAttribInfo)info
{
    return [NSValue valueWithBytes:&info objCType:@encode(BHGLVertexAttribInfo)];
}

- (BHGLTextureInfo)BHGLTextureInfoValue
{
    BHGLTextureInfo info;
    [self getValue:&info];
    
    return info;
}

+ (NSValue *)BHGLvalueWithTextureInfo:(BHGLTextureInfo)info
{
    return [NSValue valueWithBytes:&info objCType:@encode(BHGLTextureInfo)];
}

@end
