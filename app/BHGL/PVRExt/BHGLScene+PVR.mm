//
//  BHGLScene+PVR.m
//
//  Created by John Visentin on 10/20/13.
//  Copyright (c) 2013 Brockenhaus Studio. All rights reserved.
//

#import "BHGLScene+PVR.h"
#import "PVRTModelPOD.h"
#import "PVRTString.h"
#import "BHGLMesh+PVR.h"
#import "BHGLTexture+PVR.h"
#import "BHGLKeyframeAnimation+PVR.h"
#import "BHGLMaterial+PVR.h"
#import "BHGLModelNode.h"

@implementation BHGLScene (PVR)

#if TARGET_OS_IPHONE

- (id)initWithPODFileNamed:(NSString *)podFileName orientation:(UIInterfaceOrientation)orientation
{
    return [self initWithPODFileNamed:podFileName orientation:orientation nodeClass:[BHGLModelNode class]];
}

- (id)initWithPODFileNamed:(NSString *)podFileName orientation:(UIInterfaceOrientation)orientation nodeClass:(Class)nodeClass
{
    if ((self = [self initWithPODFileNamed:(NSString *)podFileName nodeClass:nodeClass]))
    {
        CGSize screenSize = [UIScreen mainScreen].bounds.size;
        float aspectRatio = UIInterfaceOrientationIsPortrait(orientation) ? screenSize.width/screenSize.height : screenSize.height / screenSize.width;
        
        self.activeCamera.aspectRatio = aspectRatio;
    }
    return self;
}

- (id)initWithSPODScene:(const SPODScene &)scene orientation:(UIInterfaceOrientation)orientation
{
    return [self initWithSPODScene:scene orientation:orientation nodeClass:[BHGLModelNode class]];
}

- (id)initWithSPODScene:(const SPODScene &)scene orientation:(UIInterfaceOrientation)orientation nodeClass:(Class)nodeClass
{
    if ((self = [self initWithSPODScene:scene nodeClass:nodeClass]))
    {
        CGSize screenSize = [UIScreen mainScreen].bounds.size;
        float aspectRatio = UIInterfaceOrientationIsPortrait(orientation) ? screenSize.width/screenSize.height : screenSize.height / screenSize.width;
        
        self.activeCamera.aspectRatio = aspectRatio;
    }
    return self;
}

#endif

- (id)initWithPODFileNamed:(NSString *)podFileName
{
    return [self initWithPODFileNamed:podFileName nodeClass:[BHGLModelNode class]];
}

- (id)initWithPODFileNamed:(NSString *)podFileName nodeClass:(Class)nodeClass
{
    CPVRTModelPOD scene;
    NSString *path = [[NSBundle mainBundle] pathForResource:podFileName ofType:@"pod"];
    
    if (scene.ReadFromFile([path UTF8String]) == PVR_SUCCESS)
    {
        return [self initWithSPODScene:scene nodeClass:nodeClass];
    }
    else
    {
#ifdef DEBUG
        NSLog(@"failed to load pod file: %@", podFileName);
#endif
        return nil;
    }
}

- (id)initWithSPODScene:(const SPODScene &)scene
{
    return [self initWithSPODScene:scene nodeClass:[BHGLModelNode class]];
}

- (id)initWithSPODScene:(const SPODScene &)scene nodeClass:(Class)nodeClass
{
    NSMutableArray *meshes = [NSMutableArray arrayWithCapacity:scene.nNumMesh];
    NSMutableArray *textures = [NSMutableArray arrayWithCapacity:scene.nNumTexture];
    
    NSMutableArray *models = [NSMutableArray arrayWithCapacity:scene.nNumMeshNode];
    
    for (int i = 0; i < scene.nNumMesh; i++)
    {
        BHGLMesh *mesh = [[BHGLMesh alloc] initWithSPODMesh:scene.pMesh[i]];
        
        [meshes addObject:mesh];
    }
    
    for (int i = 0; i < scene.nNumTexture; i++)
    {
        CPVRTString texName = scene.pTexture[i].pszName;
        BHGLTexture *tex = [[BHGLTexture alloc] initWithPVRFileNamed:[NSString stringWithUTF8String:texName.c_str()]];
        
        tex.identifier = [NSString stringWithUTF8String:scene.pTexture[i].pszName];
        
        [textures addObject:tex];
    }
    
    for (int i = 0; i < scene.nNumMeshNode; i++)
    {
        SPODNode& node = scene.pNode[i];
        
        int j = scene.pNode[i].nIdx;
        
        BHGLMesh *mesh = meshes[j];
        
        BHGLMaterial *material = nil;
		if(node.nIdxMaterial != -1)
        {
            material = [[BHGLMaterial alloc] initWithSPODMaterial:scene.pMaterial[node.nIdxMaterial]];
            
            if (scene.pMaterial[node.nIdxMaterial].nIdxTexDiffuse < textures.count) {
                material.texture = textures[scene.pMaterial[node.nIdxMaterial].nIdxTexDiffuse];
            }
        }
        
        BHGLModelNode *model = [[nodeClass alloc] initWithMesh:mesh material:material];
        [model runAnimation:[[BHGLKeyframeAnimation alloc] initWithSPODNode:node numFrames:scene.nNumFrame fps:scene.nFPS]];
        
        model.name = [NSString stringWithUTF8String:node.pszName];
        
        [models addObject:model];
    }
    
    if ((self = [super init]))
    {
        for (int i = 0; i < scene.nNumMeshNode; i++)
        {
            SPODNode& node = scene.pNode[i];
            
            if (node.nIdxParent != -1 && node.nIdxParent < models.count)
            {
                [models[node.nIdxParent] addChild:models[i]];
            }
            else
            {
                [self addChild:models[i]];
            }
        }
        
        for (int i = 0; i < scene.nNumCamera; i++)
        {
            SPODNode& camNode = scene.pNode[scene.nNumMeshNode + scene.nNumLight + i];
            SPODCamera& cam = scene.pCamera[camNode.nIdx];
            
            BHGLCamera *camera = [[BHGLCamera alloc] initWithFieldOfView:cam.fFOV aspectRatio:1.0f nearClippingPlane:cam.fNear farClippingPlane:cam.fFar];
            [camera runAnimation:[[BHGLKeyframeAnimation alloc] initWithSPODNode:camNode numFrames:scene.nNumFrame fps:scene.nFPS]];
            
            camera.name = [NSString stringWithUTF8String:camNode.pszName];
            
            if (cam.nIdxTarget != -1 && cam.nIdxTarget < models.count)
            {
                camera.target = models[cam.nIdxTarget];
            }
            
            // cameras point straight down by default in POD format
            camera.up = GLKVector3Make(0.0f, 0.0f, -1.0f);
            
            [self addCamera:camera];
        }
    }
    
    return self;
}

+ (BHGLNode *)rootNodeFromPODFileNamed:(NSString *)podFileName
{
    return [BHGLScene rootNodeFromPODFileNamed:podFileName nodeClass:[BHGLNode class]];
}

+ (id)rootNodeFromPODFileNamed:(NSString *)podFileName nodeClass:(Class)nodeClass
{
    CPVRTModelPOD scene;
    NSString *path = [[NSBundle mainBundle] pathForResource:podFileName ofType:@"pod"];
    
    if (scene.ReadFromFile([path UTF8String]) == PVR_SUCCESS)
    {
        return [self rootNodeFromScene:scene nodeClass:nodeClass];
    }
    else
    {
#ifdef DEBUG
        NSLog(@"failed to load pod file: %@", podFileName);
#endif
        return nil;
    }
}

+ (BHGLNode *)rootNodeFromScene:(const SPODScene &)scene
{
    return [BHGLScene rootNodeFromScene:scene nodeClass:[BHGLNode class]];
}

+ (id)rootNodeFromScene:(const SPODScene &)scene nodeClass:(Class)nodeClass
{
    NSMutableArray *meshes = [NSMutableArray arrayWithCapacity:scene.nNumMesh];
    NSMutableArray *textures = [NSMutableArray arrayWithCapacity:scene.nNumTexture];
    
    NSMutableArray *models = [NSMutableArray arrayWithCapacity:scene.nNumMeshNode];
    
    for (int i = 0; i < scene.nNumMesh; i++)
    {
        BHGLMesh *mesh = [[BHGLMesh alloc] initWithSPODMesh:scene.pMesh[i]];
        
        [meshes addObject:mesh];
    }
    
    for (int i = 0; i < scene.nNumTexture; i++)
    {
        CPVRTString texName = scene.pTexture[i].pszName;
        BHGLTexture *tex = [[BHGLTexture alloc] initWithPVRFileNamed:[NSString stringWithUTF8String:texName.c_str()]];
        
        tex.identifier = [NSString stringWithUTF8String:scene.pTexture[i].pszName];
        
        [textures addObject:tex];
    }
    
    for (int i = 0; i < scene.nNumMeshNode; i++)
    {
        SPODNode& node = scene.pNode[i];
        
        int j = scene.pNode[i].nIdx;
        
        BHGLMesh *mesh = meshes[j];
        
        BHGLMaterial *material = nil;
		if(node.nIdxMaterial != -1)
        {
            material = [[BHGLMaterial alloc] initWithSPODMaterial:scene.pMaterial[node.nIdxMaterial]];
            
            if (scene.pMaterial[node.nIdxMaterial].nIdxTexDiffuse < textures.count)
            {
                material.texture = textures[scene.pMaterial[node.nIdxMaterial].nIdxTexDiffuse];
            }
        }
        
        BHGLModelNode *model = [[nodeClass alloc] initWithMesh:mesh material:material];
        [model runAnimation:[[BHGLKeyframeAnimation alloc] initWithSPODNode:node numFrames:scene.nNumFrame fps:scene.nFPS]];
        
        model.name = [NSString stringWithUTF8String:node.pszName];
        
        [models addObject:model];
    }
    
    BHGLNode *rootNode = [[BHGLNode alloc] init];
    
    for (int i = 0; i < scene.nNumMeshNode; i++)
    {
        SPODNode& node = scene.pNode[i];
        
        if (node.nIdxParent != -1 && node.nIdxParent < models.count)
        {
            [models[node.nIdxParent] addChild:models[i]];
        }
        else
        {
            [rootNode addChild:models[i]];
        }
    }
    
    return rootNode;
}

@end
