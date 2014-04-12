//
//  MPTokenizer.h
//
//  Created by John Visentin on 4/11/14.
//  Copyright (c) 2014 John Visentin. All rights reserved.
//
// A class that crudely breaks a file into tokens.

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

// helper class for reading tokens from a file one by one
class Tokenizer
{
public:
    Tokenizer(std::string file) : fileStream_(file), tokenDelimiter_(' ') {}
    ~Tokenizer() {}
    
    void setTokenDelimiter(const char delimiter);
    
    std::string getNext();
    
    std::string match(const std::string &expected);
    std::string match(const std::regex &expected);
    
    void throw_token_error(const std::string &token);
    
private:
    std::ifstream fileStream_;
    std::istringstream lineStream_;
    
    char tokenDelimiter_;
    
    int lineNum_;
};
    
}

#endif
