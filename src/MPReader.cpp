//
//  MPReader.cpp
//
//  Created by John Visentin on 4/10/14.
//  Copyright (c) 2014 Brockenhaus Studio. All rights reserved.
//

#include "MPReader.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <regex>

namespace MP
{
    
/* token constants */
static const std::string EmptyToken = "";
static const std::regex Whitespace("[ \t\n]+");
static const std::regex Comment("//");
static const std::regex Keyword("import|new");
static const std::regex Type("Mesh|Environment");

// helper class for reading tokens from a file one by one
class Tokenizer
{
    
public:
    Tokenizer(std::string file) : fileStream(file) {}
    ~Tokenizer() {}
    
    std::string getNext()
    {
        std::string token;
        
        while (std::getline(this->lineStream, token, ' '))
        {
            if (token.length() == 0) continue;
            if (std::regex_match(token.begin(), token.end(), Comment)) break;
            
            return token;
        }
        
        std::string line;
        if (std::getline(this->fileStream, line))
        {
            line = std::regex_replace(line, Whitespace, " ");
            
            this->lineStream.str(line);
            this->lineStream.clear();
            
            token = this->getNext();
        }
        
        return token;
    }
    
private:
    std::ifstream fileStream;
    std::istringstream lineStream;
};

Reader::Reader(std::string filePath) : file(filePath) {}

Reader::~Reader() {}
    
Environment3D* Reader::generateEnvironment3D() const
{
    std::string token;
    
    Tokenizer tokens(this->file);
    
    while ((token = tokens.getNext()) != EmptyToken)
    {
        std::cout << token << std::endl;
    }
    
    return nullptr;
}

}