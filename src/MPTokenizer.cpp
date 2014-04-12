//
//  MPTokenizer.cpp
//
//  Created by John Visentin on 4/11/14.
//  Copyright (c) 2014 John Visentin. All rights reserved.
//

#include "MPTokenizer.h"

namespace MP
{

#pragma mark - public methods
    
void Tokenizer::setTokenDelimiter(const char delimiter)
{
    this->tokenDelimiter_ = delimiter;
}
    
std::string Tokenizer::getNext()
{
    std::string token = EmptyToken;
    
    while (std::getline(this->lineStream_, token, this->tokenDelimiter_))
    {
        if (token.length() == 0) continue; // ignore white space
        if (std::regex_match(token.begin(), token.end(), Comment)) break; // skip comments
        
        token = std::regex_replace(token, Whitespace, "");
        
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

std::string Tokenizer::match(const std::string &expected)
{
    std::string next = this->getNext();
    
    if (next != expected)
    {
        this->throw_token_error(next);
    }
    
    return next;
}

std::string Tokenizer::match(const std::regex &expected)
{
    std::string next = this->getNext();
    
    if (!std::regex_match(next.begin(), next.end(), expected))
    {
        this->throw_token_error(next);
    }
    
    return next;
}
    
void Tokenizer::throw_token_error(const std::string &token)
{
    throw std::runtime_error("unexpected token '" + token + "' in line " + std::to_string(this->lineNum_) + ".");
}

}