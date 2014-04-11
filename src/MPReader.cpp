//
//  MPReader.cpp
//
//  Created by John Visentin on 4/10/14.
//  Copyright (c) 2014 Brockenhaus Studio. All rights reserved.
//

#include "MPReader.h"
#include <iostream>

namespace MP
{

#pragma mark - public methods
    
Reader::Reader(std::string filePath) : file_(filePath) {}

Reader::~Reader() {}
    
Environment3D* Reader::generateEnvironment3D() const
{
    Tokenizer tokens(this->file_);
    
    return this->generateEnvironment3D_(tokens);
}
    
Model* Reader::generateModel() const
{
    Tokenizer tokens(this->file_);
    
    return this->generateModel_(tokens);
}

MPMesh* Reader::generateMesh() const
{
    Tokenizer tokens(this->file_);
    
    return this->generateMesh_(tokens);
}
    
#pragma mark - private methods
    
Environment3D* Reader::generateEnvironment3D_(Tokenizer &tokens) const
{
    std::string token;
    
    try
    {
        while ((token = tokens.getNext()) != EmptyToken)
        {
            std::cout << token << std::endl;
        }
    }
    catch (std::runtime_error e)
    {
        std::cout << e.what() << std::endl;
    }
    
    return nullptr;
}
    
Model* Reader::generateModel_(Tokenizer &tokens) const
{
    return nullptr;
}

MPMesh* Reader::generateMesh_(Tokenizer &tokens) const
{
    return nullptr;
}

}