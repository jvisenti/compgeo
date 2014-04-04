//
//  BHGLMesh+PVR.m
//
//  Created by John Visentin on 10/15/13.
//  Copyright (c) 2013 Brockenhaus Studio. All rights reserved.
//

#import "BHGLMesh+PVR.h"
#import "BHGLCUtils.h"
#import "PVRTModelPOD.h"

@implementation BHGLMesh (PVR)

- (id)initWithSPODMesh:(const SPODMesh &)mesh
{
    // TODO: support more attributes
    
    BOOL pos = TRUE;
    BOOL norm = mesh.sNormals.pData != NULL;
    BOOL tex0 = mesh.nNumUVW > 0;
    BOOL tex1 = mesh.nNumUVW > 1;
    BOOL col = mesh.sVtxColours.pData != NULL;
    
    int numAttribs = pos + norm + col + tex0 + tex1;
    
    BHGLVertexType vType = BHGLVertexTypeCreate(numAttribs);
    vType.stride = mesh.sVertex.nStride;
    
    int i = 0;
    
    if (pos)
    {
        vType.attribs[i] = BHGLVertexAttribPosition;
        vType.types[i] = GL_FLOAT;
        vType.lengths[i] = mesh.sVertex.n;
        vType.offsets[i] = mesh.sVertex.pData;
        i++;
    }
    
    if (norm)
    {
        vType.attribs[i] = BHGLVertexAttribNormal;
        vType.types[i] = GL_FLOAT;
        vType.lengths[i] = mesh.sNormals.n;
        vType.offsets[i] = mesh.sNormals.pData;
        i++;
    }
    
    if (col)
    {
        vType.attribs[i] = BHGLVertexAttribColor;
        vType.types[i] = GL_FLOAT;
        vType.lengths[i] = mesh.sVtxColours.n;
        vType.offsets[i] = mesh.sVtxColours.pData;
        i++;
    }
    
    if (tex0)
    {
        vType.attribs[i] = BHGLVertexAttribTexCoord0;
        vType.types[i] = GL_FLOAT;
        vType.lengths[i] = mesh.psUVW[0].n;
        vType.offsets[i] = mesh.psUVW[0].pData;
        i++;
    }
    
    if (tex1)
    {
        vType.attribs[i] = BHGLVertexAttribTexCoord1;
        vType.types[i] = GL_FLOAT;
        vType.lengths[i] = mesh.psUVW[1].n;
        vType.offsets[i] = mesh.psUVW[1].pData;
        i++;
    }
    
    GLvoid *vertexData = mesh.pInterleaved;
    GLsizeiptr vertexDataSize = mesh.nNumVertex * mesh.sVertex.nStride;
    
    if (mesh.sFaces.pData != NULL)
    {
        GLvoid *indexData = mesh.sFaces.pData;
        GLsizeiptr indexDataSize = PVRTModelPODCountIndices(mesh) * mesh.sFaces.nStride;
        GLenum indexType = (mesh.sFaces.eType == EPODDataUnsignedShort) ? GL_UNSIGNED_SHORT : GL_UNSIGNED_INT;
        
        self = [self initWithVertexData:vertexData vertexDataSize:vertexDataSize vertexType:&vType indexData:indexData indexDataSize:indexDataSize indexType:indexType];
    }
    else
    {
        self = [self initWithVertexData:vertexData vertexDataSize:vertexDataSize vertexType:&vType];
    }
    
    BHGLVertexTypeFree(vType);
    
    return self;
}

@end
