//
//  MPReader.cpp
//
//  Created by John Visentin on 4/10/14.
//  Copyright (c) 2014 John Visentin. All rights reserved.
//

#include "MPReader.h"
#include <cstdlib>

namespace MP
{
    
static const std::regex MeshValue("Vertex|Index|Texture");
static const std::regex ModelValue("Mesh|Position|Rotation|Scale");
static const std::regex EnvironmentValue("Dynamic|Step|RotationStep|Origin|Size|ActiveObject|Obstacles");
    
static const std::regex FloatLit("^\\-?[\\d]*\\.?[\\d]*$");
static const std::regex IntLit("^\\-?[\\d]+");
static const std::regex Identifier("^\\w+[\\w\\d]*$");

#pragma mark - public methods
    
Environment3D* Reader::generateEnvironment3D() const
{
    Tokenizer tokens(this->file_);
    
    std::string token;
    std::map<std::string, MPMesh *> importedMeshes;
    
    try
    {
        // import all referenced meshes
        while ((token = tokens.getNext()) == "import")
        {
            // NOTE: retains meshes
            this->importMeshes_(tokens, importedMeshes);
        }
        
        if (token != "new")
        {
            tokens.throw_token_error(token);
        }
        
        tokens.match("Environment");
        
        Environment3D *environment = this->generateEnvironment3D_(tokens, importedMeshes);
        
        // release all imported meshes
        for(std::map<std::string, MPMesh *>::iterator it = importedMeshes.begin(); it != importedMeshes.end(); ++it)
        {
            MPMeshRelease(it->second);
        }
        
        return environment;
    }
    catch (std::runtime_error e)
    {
        std::cout << e.what() << std::endl;
        
        return nullptr;
    }
}
    
Model* Reader::generateModel() const
{
    Tokenizer tokens(this->file_);
    
    std::string token;
    std::map<std::string, MPMesh *> importedMeshes;
    
    try
    {
        // import all referenced meshes
        while ((token = tokens.getNext()) == "import")
        {
            // NOTE: retains meshes
            this->importMeshes_(tokens, importedMeshes);
        }
        
        if (token != "new")
        {
            tokens.throw_token_error(token);
        }
        
        tokens.match("Model");
        
        Model *model = this->generateModel_(tokens, importedMeshes);
        
        // release all imported meshes
        for(std::map<std::string, MPMesh *>::iterator it = importedMeshes.begin(); it != importedMeshes.end(); ++it)
        {
            MPMeshRelease(it->second);
        }
        
        return model;
    }
    catch (std::runtime_error e)
    {
        std::cout << e.what() << std::endl;
        
        return nullptr;
    }
}

MPMesh* Reader::generateMesh() const
{
    Tokenizer tokens(this->file_);
    
    try
    {
        tokens.match("new");
        tokens.match("Mesh");
        
        return this->generateMesh_(tokens);
    }
    catch (std::runtime_error e)
    {
        std::cout << e.what() << std::endl;
        
        return nullptr;
    }
}
    
#pragma mark - private methods
    
Environment3D* Reader::generateEnvironment3D_(Tokenizer &tokens, const std::map<std::string, MPMesh *> &meshes) const
{
    Environment3D *environment = new Environment3D();
    std::string token;
    
    try
    {
        tokens.match("{");
        
        while ((token = tokens.getNext()) == "set")
        {
            token = tokens.match(EnvironmentValue);
            tokens.match("=");
            
            if (token == "Dynamic")
            {
                tokens.match("{");
                
                environment->setDynamic(atoi(tokens.match(IntLit).c_str()));
                
                tokens.match("}");
            }
            else if (token == "Step")
            {
                tokens.match("{");
                
                environment->setStepSize(atof(tokens.match(FloatLit).c_str()));
                
                tokens.match("}");
            }
            else if (token == "RotationStep")
            {
                tokens.match("{");
                
                environment->setRotationStepSize(RADIANS(atof(tokens.match(FloatLit).c_str())));
                
                tokens.match("}");
            }
            else if (token == "Origin")
            {
                environment->setOrigin(this->loadVector3_(tokens));
            }
            else if (token == "Size")
            {
                environment->setSize(this->loadVector3_(tokens));
            }
            else if (token == "ActiveObject")
            {
                tokens.match("new");
                tokens.match("Model");
                
                environment->setActiveObject(this->generateModel_(tokens, meshes));
            }
            else // token = Obstacles
            {
                tokens.match("{");
                
                while ((token = tokens.getNext()) == "new")
                {
                    tokens.match("Model");
                    
                    environment->addObstacle(this->generateModel_(tokens, meshes));
                }
                
                if (token != "}")
                {
                    tokens.throw_token_error(token);
                }
            }
        }
        
        if (token != "}")
        {
            tokens.throw_token_error(token);
        }
    }
    catch (std::runtime_error e)
    {
        delete environment;
        std::cout << e.what() << std::endl;
        
        return nullptr;
    }
    
    return environment;
}
    
Model* Reader::generateModel_(Tokenizer &tokens, const std::map<std::string, MPMesh *> &meshes) const
{
    std::string token;
    Model *model = new Model();
    
    try
    {
        tokens.match("{");
        
        while ((token = tokens.getNext()) == "set")
        {
            token = tokens.match(ModelValue);
            tokens.match("=");
            
            if (token == "Position")
            {
                model->setPosition(this->loadVector3_(tokens));
            }
            else if (token == "Rotation")
            {
                // NOTE: this may not be the best format to store rotations in
                MPVec3 axisRotations = this->loadVector3_(tokens);
                
                MPQuaternion rx = MPQuaternionMakeWithAngleAndAxis(RADIANS(axisRotations.x), 1.0f, 0.0f, 0.0f);
                MPQuaternion ry = MPQuaternionMakeWithAngleAndAxis(RADIANS(axisRotations.y), 0.0f, 1.0f, 0.0f);
                MPQuaternion rz = MPQuaternionMakeWithAngleAndAxis(RADIANS(axisRotations.z), 0.0f, 0.0f, 1.0f);
                
                MPQuaternion r = MPQuaternionMultiply(ry, rx);
                r = MPQuaternionMultiply(rz, r);
                
                model->setRotation(r);
            }
            else if (token == "Scale")
            {
                model->setScale(this->loadVector3_(tokens));
            }
            else // token = Mesh
            {
                token = tokens.getNext();
                
                if (token == "new")
                {
                    tokens.match("Mesh");
                    
                    model->setMesh(this->generateMesh_(tokens));
                }
                else
                {
                    std::map<std::string, MPMesh *>::const_iterator kv = meshes.find(token);
                    
                    if (kv != meshes.end())
                    {
                        // mesh has been imported already, so use it
                        model->setMesh(kv->second);
                    }
                    else
                    {
                        tokens.throw_token_error(token);
                    }
                }
            }
        }
        
        if (token != "}")
        {
            tokens.throw_token_error(token);
        }
    }
    catch (std::runtime_error e)
    {
        delete model;
        std::cout << e.what() << std::endl;
        
        return nullptr;
    }
    
    return model;
}

MPMesh* Reader::generateMesh_(Tokenizer &tokens) const
{
    std::string token;
    
    void *vertexData, *indexData;
    vertexData = indexData = nullptr;
    
    size_t stride, numVertices, indexSize, numIndices;
    stride = numVertices = indexSize = numIndices = 0;
    
    char *texName = nullptr;
    
    try
    {
        tokens.match("{");
        
        while ((token = tokens.getNext()) == "set")
        {
            token = tokens.match(MeshValue);
            
            if (token == "Texture")
            {
                tokens.match("=");
                tokens.match("{");
                
                std::string name = tokens.getNext();
                
                texName = (char *)malloc(name.size());
                strcpy(texName, name.c_str());
                
                tokens.match("}");
            }
            else if (token == "Vertex")
            {
                this->loadVertices(tokens, &vertexData, stride, numVertices);
            }
            else // token = Index
            {
                this->loadIndices(tokens, &indexData, indexSize, numIndices);
            }
        }
        
        if (token != "}")
        {
            tokens.throw_token_error(token);
        }
    }
    catch (std::runtime_error e)
    {
        free(vertexData);
        free(indexData);
        
        std::cout << e.what() << std::endl;
        
        return nullptr;
    }
    
    MPMesh *mesh = MPMeshCreate((const MPVec3 *)vertexData, stride, numVertices, (const void *)indexData, indexSize, numIndices);
    mesh->texName = texName;
    
    return mesh;
}
    
void Reader::importMeshes_(Tokenizer &tokens, std::map<std::string, MPMesh *> &meshes) const
{
    std::string importFile = tokens.getNext();
    tokens.match("as");
    std::string alias = tokens.match(Identifier);
    
    if (importFile[0] != '/')
    {
        // import should be a relative path
        size_t slash = this->file_.find_last_of("/\\");
        importFile = this->file_.substr(0, slash) + "/" + importFile;
    }
    
    Reader meshReader(importFile);
    
    MPMesh *mesh = meshReader.generateMesh();
    MPMeshRetain(mesh);
    
    meshes.insert(std::make_pair(alias, mesh));
}
    
MPVec3 Reader::loadVector3_(Tokenizer &tokens) const
{
    std::string token;
    
    tokens.match("{");

    tokens.setTokenDelimiter(',');
    
    MPVec3 vec;
    
    for (int i = 0; (token = tokens.getNext()) != "}" && i < 3; ++i)
    {
        if (!std::regex_match(token.begin(), token.end(), FloatLit))
        {
            tokens.throw_token_error(token);
        }
        
        vec.v[i] = atof(token.c_str());
    }
    
    tokens.setTokenDelimiter(' ');
    
    return vec;
}
    
void Reader::loadVertices(Tokenizer &tokens, void **vertexData, size_t &stride, size_t &count) const
{
    std::string token;
    
    count = atol(tokens.match(IntLit).c_str());
    size_t elementsPerVertex = atol(tokens.match(IntLit).c_str());
    
    // currently enforcing that vertex elements be float types
    stride = elementsPerVertex * sizeof(float);
    
    float *values = (float *)malloc(count * elementsPerVertex * sizeof(float));
    *vertexData = values;
    
    tokens.match("=");
    tokens.match("{");
    
    // arrays have elements delimited by a comma
    tokens.setTokenDelimiter(',');
    
    for (int i = 0; (token = tokens.getNext()) != "}" && i < count * elementsPerVertex; ++i)
    {
        if (!std::regex_match(token.begin(), token.end(), FloatLit))
        {
            tokens.throw_token_error(token);
        }
        
        values[i] = atof(token.c_str());
    }
    
    // set delimiter back to default
    tokens.setTokenDelimiter(' ');
}

void Reader::loadIndices(Tokenizer &tokens, void **indexData, size_t &size, size_t &count) const
{
    std::string token;
    
    count = atol(tokens.match(IntLit).c_str());
    
    // currently enforcing that indices be unsigned ints
    size = sizeof(unsigned int);
    
    unsigned int *values = (unsigned int *)malloc(count * sizeof(unsigned int));
    *indexData = values;
    
    tokens.match("=");
    tokens.match("{");
    
    // arrays have elements delimited by a comma
    tokens.setTokenDelimiter(',');
    
    for (int i = 0; (token = tokens.getNext()) != "}" && i < count; ++i)
    {
        if (!std::regex_match(token.begin(), token.end(), IntLit))
        {
            tokens.throw_token_error(token);
        }
        
        values[i] = (unsigned int)atoi(token.c_str());
    }
    
    // set delimiter back to default
    tokens.setTokenDelimiter(' ');
}

}