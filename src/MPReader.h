//
//  MPReader.h
//
//  Created by John Visentin on 4/10/14.
//  Copyright (c) 2014 Brockenhaus Studio. All rights reserved.
//

#ifndef __MPReader__
#define __MPReader__

#include "MPEnvironment3D.h"

namespace MP
{
    
class Reader
{
public:
    Reader(std::string filePath);
    ~Reader();
    
    Environment3D* generateEnvironment3D() const;

private:
    std::string file;
};
    
}
#endif
