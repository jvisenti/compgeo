//
//  MPReader.h
//
//  Created by John Visentin on 4/10/14.
//  Copyright (c) 2014 Brockenhaus Studio. All rights reserved.
//
// A file reader for loading mesh/model/environment data.

#ifndef __MPReader__
#define __MPReader__

#include "MPEnvironment3D.h"
#include "MPTokenizer.h"
#include <map>

namespace MP
{
    
class Reader
{
public:
    Reader(std::string filePath) : file_(filePath) {}
    
    Environment3D* generateEnvironment3D() const;
    Model* generateModel() const;
    MPMesh* generateMesh() const;

private:
    std::string file_;
    
    Environment3D* generateEnvironment3D_(Tokenizer &tokens, const std::map<std::string, MPMesh *> &meshes) const;
    Model* generateModel_(Tokenizer &tokens, const std::map<std::string, MPMesh *> &meshes) const;
    MPMesh* generateMesh_(Tokenizer &tokens) const;
    
    void importMeshes_(Tokenizer &tokens, std::map<std::string, MPMesh *> &meshes) const;
    
    MPVec3 loadVector3_(Tokenizer &tokens) const;
    
    void loadVertices(Tokenizer &tokens, void **vertexData, size_t &stride, size_t &count) const;
    void loadIndices(Tokenizer &tokens, void **indexData, size_t &size, size_t &count) const;
};
    
}
#endif
