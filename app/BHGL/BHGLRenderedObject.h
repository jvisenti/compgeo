//
//  BHGLRenderedObject.h
//
//  Created by John Visentin on 10/11/13.
//  Copyright (c) 2013 Brockenhaus Studio. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "BHGLProgram.h"

@protocol BHGLRenderedObject <NSObject>

@required
/** Should render the object. Texture, VAO and VBOs should be bound to 0 before returning. */
- (void)render;

@optional
/** Should be called before each frame to perform any updates prior to drawing. */
- (void)update:(NSTimeInterval)dt;

/** Render the object with the given program. Texture, VAO and VBOs should be bound to 0 before returning. */
- (void)renderWithProgram:(BHGLProgram *)program;

@end
