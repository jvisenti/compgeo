//
//  MPVoxelGrid.h
//
//  Created by Ellis M. Ratner on 5/5/14.
//

// Algorithm for voxelizing the border of a mesh: for each voxel, test if that voxel's bounding box
// (so we need a helper to generate the bounding box of a given voxel) intersects with each triangle
// Slow (num voxels * num triangles), but will get the job done

#ifndef __MPAction6D__
#define __MPAction6D__

#include "MPMath.h"
#include <iostream>

namespace MP
{
    template <typename VD>
    class VoxelGrid
    {
    public:
        VoxelGrid(const MPAABox &boundingBox, float resolution)
        : boundingBox_(boundingBox), resolution_(resolution)
        {
            // Note: the voxel grid origin is by convention at the min
            cells_[0] = (int)((boundingBox_.max.x - boundingBox_.min.x) / resolution_); // i
            cells_[1] = (int)((boundingBox_.max.y - boundingBox_.min.y) / resolution_); // j
            cells_[2] = (int)((boundingBox_.max.z - boundingBox_.min.z) / resolution_); // k
            
            std::cout << "Initializing voxel grid of dimensions (" << cells_[0] << ", " << cells_[1]
            << ", " << cells_[2] << ") for a total of " << cells_[0] * cells_[1] * cells_[2] <<
                " voxels" << std::endl;
            
            voxels_ = new VD[cells_[0] * cells_[1] * cells_[2]];
        }
        
        ~VoxelGrid()
        {
            delete [] voxels_;
            voxels_ = nullptr;
        }
        
        bool getVoxel(int i, int j, int k, VD &value) const
        {
            if(!validCell(i, j, k))
                return false;
            
            value = voxels_[i * cells_[0] * cells_[1] + j * cells_[1] + k];
            
            return true;
        }
        
        bool getVoxel(const MPVec3 &point, VD &value) const
        {
            int i, j, k;
            if(!worldToGrid(point, i, j, k))
                return false;
            
            return getVoxel(i, j, k, value);
        }
        
        bool setVoxel(int i, int j, int k, VD value)
        {
            if(!validCell(i, j, k))
                return false;
            
            voxels_[i * cells_[0] * cells_[1] + j * cells_[1] + k] = value;
            
            return true;
        }
        
        bool setVoxel(const MPVec3 &point, VD value)
        {
            int i, j, k;
            if(!worldToGrid(point, i, j, k))
                return false;
            
            return setVoxel(i, j, k, value);
        }
        
        void translate(const MPVec3 &translation)
        {
            boundingBox_.min = MPVec3Add(boundingBox_.min, translation);
            boundingBox_.max = MPVec3Add(boundingBox_.max, translation);
        }
        
    private:
        bool validCell(int i, int j, int k) const
        {
            return (i >= 0 && i < cells_[0] && j >= 0 && j < cells_[1] && k >= 0 && k < cells_[2]);
        }
        
        bool worldToGrid(const MPVec3 &point, int &i, int &j, int &k) const
        {
            if(!MPAABoxContainsPoint(boundingBox_, point))
                return false;
            
            MPVec3 relPoint = MPVec3Subtract(point, boundingBox_.min);
            i = (int)(relPoint.x / resolution_);
            j = (int)(relPoint.y / resolution_);
            k = (int)(relPoint.z / resolution_);
            
            return true;
        }
        
        MPAABox boundingBox_;
        float resolution_;
        
        int cells_[3];
        VD *voxels_;
        
    };
    
}

#endif