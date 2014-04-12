//
//  BHGLTextNode.m
//
//  Created by John Visentin on 3/6/14.
//  Copyright (c) 2014 Brockenhaus Studio. All rights reserved.
//

#import "BHGLTextNode.h"
#import "BHGLCUtils.h"
#import "BHGLMesh.h"
#import "BHGLMaterial.h"

#if TARGET_OS_IPHONE
#define BHGL_COLOR_TYPE UIColor
#define BHGL_FONT_TYPE  UIFont
#else
#define BHGL_COLOR_TYPE NSColor
#define BHGL_FONT_TYPE  NSFont
#endif

@interface BHGLTextNode ()

@property (nonatomic, strong) BHGLMesh *mesh;
@property (nonatomic, strong) BHGLMaterial *material;

- (void)createMesh;
- (void)createMaterial;

- (void)updateTexture;

@end

@implementation BHGLTextNode

- (id)init
{
    if ((self = [super init]))
    {
        _font = [BHGL_FONT_TYPE systemFontOfSize:[BHGL_FONT_TYPE systemFontSize]];
        _textColor = [BHGL_COLOR_TYPE blackColor];
                
        BHGLProgram *program = [[BHGLProgram alloc] initWithVertexShaderNamed:kBHGLBasicVertexShader fragmentShaderNamed:kBHGLBasicFragmentShader mvpUniform:kBHGLMVPUniformName];
        [program setVertexAttribute:BHGLVertexAttribPosition forName:kBHGLPositionAttributeName];
        [program setVertexAttribute:BHGLVertexAttribTexCoord0 forName:kBHGLTexCoord0AttributeName];
        
        if ([program link])
        {
            [super setProgram:program];
        }
        
        [self createMesh];
        [self createMaterial];
    }
    return self;
}

- (void)createMesh
{
    const BHGLTextureVertex vertices[] = {
        {{1.0f, 0.0f, 0.0f}, {1.0f, 0.0f}},
        {{1.0f, 1.0f, 0.0f}, {1.0f, 1.0f}},
        {{0.0f, 1.0f, 0.0f}, {0.0f, 1.0f}},
        {{0.0f, 0.0f, 0.0f}, {0.0f, 0.0f}}
    };
    
    const GLubyte indices[] = {
        0, 1, 2,
        2, 3, 0
    };
    
    BHGLVertexType vType = BHGLVertexTypeCreateWithType(BHGL_TEXTURE_VERTEX);
    self.mesh = [[BHGLMesh alloc] initWithVertexData:vertices vertexDataSize:sizeof(vertices) vertexType:&vType indexData:indices indexDataSize:sizeof(indices) indexType:GL_UNSIGNED_BYTE];
    BHGLVertexTypeFree(vType);
}

- (void)createMaterial
{
    self.material = [[BHGLMaterial alloc] init];
    self.material.blendEnabled = GL_TRUE;
    self.material.blendSrcRGB = GL_SRC_ALPHA;
    self.material.blendDestRGB = GL_ONE_MINUS_SRC_ALPHA;
}

#pragma mark - property overrides

- (void)setProgram:(BHGLProgram *)program
{
    // empty implementation
}

- (void)setText:(NSString *)text
{
    _text = [text copy];
    
    [self updateTexture];
    
    [self sizeToFit];
}

- (void)setTextColor:(BHGL_COLOR_TYPE *)textColor
{
    _textColor = textColor;
    
    [self updateTexture];
}

- (void)setFont:(BHGL_FONT_TYPE *)font
{
    _font = font;
    
    [self updateTexture];
}

- (CGRect)bounds
{
    return CGRectMake(0.0f, 0.0f, self.size.width * self.scale.x, self.size.height * self.scale.y);
}

#pragma mark - public interface

- (void)configureProgram:(BHGLProgram *)program
{
    [super configureProgram:program];
    
    program.projectionMatrix = [self computeProjectionMatrixRecursive:YES];
    program.modelViewMatrix = GLKMatrix4Multiply([self computeViewMatrixRecursive:YES], [self computeModelMatrixRecursive:YES]);    
}

- (void)sizeToFit
{
    GLint viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);
    
    self.scale = GLKVector3Make(self.size.width / viewport[2], self.size.height / viewport[3], 1.0f);
}

#pragma mark - BHGLRenderedObject

- (void)render
{
    glDisable(GL_CULL_FACE);
    
    [self configureProgram:self.program];
    [self.program prepareToDraw];
        
    [self.material bind];
    
    [self.mesh render];
    
    [self.material unbind];
    
    glEnable(GL_CULL_FACE);
    
    [super render];
}

- (void)renderWithProgram:(BHGLProgram *)program
{
    glDisable(GL_CULL_FACE);
    
    [self configureProgram:program];
    [program prepareToDraw];
    
    [self.material bind];
    
    [self.mesh render];
    
    [self.material unbind];
    
    glEnable(GL_CULL_FACE);
    
    [super renderWithProgram:program];
}

#pragma mark - private interface

#if TARGET_OS_IPHONE
// TODO: iOS version

#else

- (void)updateTexture
{
    if (self.text && self.font && self.textColor)
    {
        NSDictionary *textAttribs = @{NSFontAttributeName : self.font, NSForegroundColorAttributeName : self.textColor};
        
        CGSize textSize = [self.text sizeWithAttributes:textAttribs];
        CGRect textRect = CGRectMake(0.0f, 0.0f, textSize.width, textSize.height);
        
        NSImage *image = [[NSImage alloc] initWithSize:textSize];
        
        [image lockFocus];
        
        [self.text drawInRect:textRect withAttributes:textAttribs];
        
        [image unlockFocus];
        
        self.material.texture = [[BHGLTexture alloc] initWithImage:image minFilter:GL_LINEAR magFilter:GL_LINEAR sWrap:GL_CLAMP_TO_EDGE tWrap:GL_CLAMP_TO_EDGE];
        _size = CGSizeMake(floor(self.material.texture.width), floor(self.material.texture.height));
    }
    else
    {
        self.material.texture = nil;
    }
}
#endif

@end
