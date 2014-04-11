//
//  MPEnvironment3D.cpp
//
//  Created by Ellis Ratner on 4/9/14.
//  Copyright (c) 2014 Ellis Ratner. All rights reserved.
//

#include "MPEnvironment3D.h"

#include "geometry/MPCube.h"
#include "geometry/MPPyramid.h"

namespace MP
{
    
static bool operator==(const Transform3D &lhs, const Transform3D &rhs)
{
    // Only consider the (x, y, z) projection of the transform
    // Also, assume that the coordinates are integer-valued
    return (((int)lhs.getPosition().x == (int)rhs.getPosition().x) &&
            ((int)lhs.getPosition().y == (int)rhs.getPosition().y) &&
            ((int)lhs.getPosition().z == (int)rhs.getPosition().z));
}

static int transform3DHash(Transform3D t)
{
    const int p1 = 73856093;
    const int p2 = 19349663;
    const int p3 = 83492791;
    
    // Assume that the coordinates are integer-valued
    int x = (int)t.getPosition().x;
    int y = (int)t.getPosition().y;
    int z = (int)t.getPosition().z;
    return ((x*p1) ^ (y*p2) ^ (z*p3));
}

Environment3D::Environment3D(const MPVec3 &size)
  : Environment<Transform3D>(transform3DHash), origin_(MPVec3Make(0.0f, 0.0f, 0.0f)), size_(size), activeObject_(nullptr)
{
}
    
Environment3D::Environment3D(const MPVec3 &origin, const MPVec3 &size)
  : Environment<Transform3D>(transform3DHash), origin_(origin), size_(size), activeObject_(nullptr)
{
}
    
Environment3D::Environment3D(EnvironmentPreset preset)
  : Environment<Transform3D>(transform3DHash)
{
    switch (preset)
    {
        case EnvironmentPresetDefault:
        {
            this->origin_   = MPVec3Make(0.0f, 0.0f, 0.0f);
            this->size_     = MPVec3Make(10.0f, 5.0f, 5.0f);
            
            MPMesh *cubeMesh = MPMeshCreate((const MPVec3 *)CubeVertices, sizeof(CubeVertices[0]), sizeof(CubeVertices) / sizeof(CubeVertices[0]), (const void *)CubeIndices, sizeof(CubeIndices[0]), sizeof(CubeIndices) / sizeof(CubeIndices[0]));
            MPMesh *pyramidMesh = MPMeshCreate((const MPVec3 *)PyramidVertices, sizeof(PyramidVertices[0]), sizeof(PyramidVertices) / sizeof(PyramidVertices[0]), (const void *)PyramidIndices, sizeof(PyramidIndices[0]), sizeof(PyramidIndices) / sizeof(PyramidIndices[0]));
            
            Model *cube = new Model(cubeMesh);
            cube->setPosition(MPVec3Make(-1.0f, 0.0f, 0.0f));
            
            Model *pyramid = new Model(pyramidMesh);
            pyramid->setPosition(MPVec3Make(1.0f, 0.0f, 0.0f));
            
            this->setActiveObject(cube);
            this->addObstacle(pyramid);
            
            break;
        }
            
        default:
            std::cout << "invalid environment preset" << std::endl;
            break;
    }
}

Environment3D::~Environment3D()
{
}

void Environment3D::getSuccessors(SearchState3D *s, 
				  std::vector<SearchState3D *> &successors,
				  std::vector<double> &costs)
{
  // @todo
}

bool Environment3D::getCost(SearchState3D *s, SearchState3D *t, double &cost)
{
  // @todo
  // If the distance between the two states is > 1, then there is no edge
  // (i.e. primitive motion) between them
  return false;
}

}
