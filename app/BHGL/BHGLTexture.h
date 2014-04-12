//
//  BHGLTexture.h
//
//  Created by John Visentin on 10/16/13.
//  Copyright (c) 2013 Brockenhaus Studio. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "BHGLTypes.h"

@interface BHGLTexture : NSObject

/** An identifier for use by your application. */
@property (nonatomic, copy) NSString *identifier;

@property (nonatomic, readonly) GLuint name;

/* NOTE: width and height may not be accurate if initialized with textureInfo containing inaccurate values. */
@property (nonatomic, readonly) GLsizei width;
@property (nonatomic, readonly) GLsizei height;

- (id)initWithImageNamed:(NSString *)imageName;

#if TARGET_OS_IPHONE
- (id)initWithImage:(UIImage *)image;
- (id)initWithImage:(UIImage *)image minFilter:(GLenum)minFilter magFilter:(GLenum)magFilter sWrap:(GLenum)sWrap tWrap:(GLenum)tWrap;
#else
- (id)initWithImage:(NSImage *)image;
- (id)initWithImage:(NSImage *)image minFilter:(GLenum)minFilter magFilter:(GLenum)magFilter sWrap:(GLenum)sWrap tWrap:(GLenum)tWrap;
#endif

- (id)initWithTextureInfo:(BHGLTextureInfo)textureInfo;

@end
