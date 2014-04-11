//
//  MPTokenizer.h
//
//  Created by John Visentin on 4/11/14.
//  Copyright (c) 2014 John Visentin. All rights reserved.
//

#ifndef __MPTokenizer__
#define __MPTokenizer__

#include <fstream>
#include <sstream>
#include <regex>
#include <stdexcept>

namespace MP
{

/* token constants */
static const std::string EmptyToken = "";
static const std::regex Whitespace("[ \t\n]+");
static const std::regex Comment("^//");
static const std::regex Keyword("import|new");
static const std::regex Type("Mesh|Model|Environment|Vertex|Index");
static const std::regex Identifier("^\\w+[\\w\\d]*$");

// helper class for reading tokens from a file one by one
class Tokenizer
{
public:
    Tokenizer(std::string file) : fileStream_(file) {}
    ~Tokenizer() {}
    
    void throw_token_error(const std::string &token)
    {
        throw std::runtime_error("unexpected token '" + token + "' in line " + std::to_string(this->lineNum_) + ".");
    }
    
    std::string getNext()
    {
        std::string token;
        
        while (std::getline(this->lineStream_, token, ' '))
        {
            if (token.length() == 0) continue; // ignore white space
            if (std::regex_match(token.begin(), token.end(), Comment)) break; // skip comments
            
            return token;
        }
        
        std::string line;
        if (std::getline(this->fileStream_, line))
        {
            line = std::regex_replace(line, Whitespace, " ");
            
            this->lineStream_.str(line);
            this->lineStream_.clear();
            this->lineNum_++;
            
            token = this->getNext();
        }
        
        return token;
    }
    
    std::string match(const std::string &expected)
    {
        std::string next = this->getNext();
        
        if (next != expected)
        {
            this->throw_token_error(next);
        }
        
        return next;
    }
    
    std::string match(const std::regex &expected)
    {
        std::string next = this->getNext();
        
        if (!std::regex_match(next.begin(), next.end(), expected))
        {
            this->throw_token_error(next);
        }
        
        return next;
    }
    
private:
    std::ifstream fileStream_;
    std::istringstream lineStream_;
    
    int lineNum_;
};
    
}

#endif
