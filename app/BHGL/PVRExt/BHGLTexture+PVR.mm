//
//  BHGLTexture+PVR.m
//
//  Created by John Visentin on 10/16/13.
//  Copyright (c) 2013 Brockenhaus Studio. All rights reserved.
//

#import "BHGLTexture+PVR.h"
#import "PVRTTextureAPI.h"

@implementation BHGLTexture (PVR)

- (id)initWithPVRFileNamed:(NSString *)pvrFileName
{
    BHGLTextureInfo texInfo = BHGLTextureInfoMake(0, 0, 0);
            
    pvrFileName = [pvrFileName stringByDeletingPathExtension];
    NSString *filePath = [[NSBundle mainBundle] pathForResource:pvrFileName ofType:@"pvr"];
    
    PVRTTextureLoadFromPVR([filePath UTF8String], &texInfo.name);
    
    return [self initWithTextureInfo:texInfo];
}

@end
