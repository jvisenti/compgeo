//
//  BHGLTextNode.h
//
//  Created by John Visentin on 3/6/14.
//  Copyright (c) 2014 Brockenhaus Studio. All rights reserved.
//

#import "BHGLNode.h"

@interface BHGLTextNode : BHGLNode

@property (nonatomic, readonly) CGSize size;
@property (nonatomic, readonly) CGRect bounds;

@property (nonatomic, copy) NSString *text;

#if TARGET_OS_IPHONE
@property (nonatomic, strong) UIColor *textColor;
@property (nonatomic, strong) UIFont *font;
#else
@property (nonatomic, strong) NSColor *textColor;
@property (nonatomic, strong) NSFont *font;
#endif

- (void)sizeToFit;

@end
