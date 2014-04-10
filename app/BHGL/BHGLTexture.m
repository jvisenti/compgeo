//
//  BHGLTexture.m
//
//  Created by John Visentin on 10/16/13.
//  Copyright (c) 2013 Brockenhaus Studio. All rights reserved.
//

#import "BHGLTexture.h"
#import "BHGLCUtils.h"

@interface BHGLTexture ()

- (id)initWithCGImage:(CGImageRef)cgImage minFilter:(GLenum)minFilter magFilter:(GLenum)magFilter sWrap:(GLenum)sWrap tWrap:(GLenum)tWrap;

@end

@implementation BHGLTexture

- (id)initWithTextureInfo:(BHGLTextureInfo)textureInfo
{
    if ((self = [super init]))
    {
        _name = textureInfo.name;
        _width = textureInfo.width;
        _height = textureInfo.height;
    }
    return self;
}

- (id)initWithImageNamed:(NSString *)imageName
{
#if TARGET_OS_IPHONE
    return [self initWithImage:[UIImage imageNamed:imageName]];
#else
    return [self initWithImage:[NSImage imageNamed:imageName]];
#endif
}

#if TARGET_OS_IPHONE
- (id)initWithImage:(UIImage *)image
{
    return [self initWithImage:image minFilter:GL_NEAREST magFilter:GL_LINEAR sWrap:GL_REPEAT tWrap:GL_REPEAT];
}

- (id)initWithImage:(UIImage *)image minFilter:(GLenum)minFilter magFilter:(GLenum)magFilter sWrap:(GLenum)sWrap tWrap:(GLenum)tWrap
{
    CGImageRef cgImage = image.CGImage;
    return [self initWithCGImage:cgImage minFilter:minFilter magFilter:magFilter sWrap:sWrap tWrap:tWrap];
}

#else

- (id)initWithImage:(NSImage *)image
{
    return [self initWithImage:image minFilter:GL_NEAREST magFilter:GL_LINEAR sWrap:GL_REPEAT tWrap:GL_REPEAT];
}

- (id)initWithImage:(NSImage *)image minFilter:(GLenum)minFilter magFilter:(GLenum)magFilter sWrap:(GLenum)sWrap tWrap:(GLenum)tWrap
{
    CGImageRef cgImage = [image CGImageForProposedRect:NULL context:nil hints:nil];
    return [self initWithCGImage:cgImage minFilter:minFilter magFilter:magFilter sWrap:sWrap tWrap:tWrap];
}
#endif

#pragma mark - private interface

- (id)initWithCGImage:(CGImageRef)cgImage minFilter:(GLenum)minFilter magFilter:(GLenum)magFilter sWrap:(GLenum)sWrap tWrap:(GLenum)tWrap
{
    if (cgImage)
    {
        size_t width = CGImageGetWidth(cgImage);
        size_t height = CGImageGetHeight(cgImage);
        
        GLubyte *imageData = (GLubyte *)calloc(width*height*4, sizeof(GLubyte));
        
        CGContextRef context = CGBitmapContextCreate(imageData, width, height, 8, 4*width, CGImageGetColorSpace(cgImage), (CGBitmapInfo)kCGImageAlphaPremultipliedLast);
        
        CGContextTranslateCTM(context, 0, height);
        CGContextScaleCTM(context, 1.0, -1.0);
        
        CGContextDrawImage(context, CGRectMake(0, 0, width, height), cgImage);
        CGContextRelease(context);
        
        GLuint texName = BHGLGenerateRGBATexture((GLsizei)width, (GLsizei)height, imageData, minFilter, magFilter, sWrap, tWrap);
        
        BHGLError();
        
        free(imageData);
        
        return [self initWithTextureInfo:BHGLTextureInfoMake(texName, (GLsizei)width, (GLsizei)height)];
    }
    else
    {
        return [self initWithTextureInfo:BHGLTextureInfoMake(0, 0, 0)];
    }
}

@end
