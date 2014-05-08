//
//  MPPotentialFieldPlanner.cpp
//
//  Created by Ellis Ratner on 5/1/14.
//

#include "MPPotentialFieldController.h"
#include "MPUtils.h"

namespace MP
{
PotentialFieldController::PotentialFieldController(const std::vector<Model *> &obstacles, Model *activeObject)
: activeObject_(activeObject)
{    
    for(auto obstacle : obstacles)
    {
        // TODO: make voxel size a parameter somewhere
        int n;
        MPVec3 *voxArray = MPMeshGetVoxels(obstacle->getMesh(), obstacle->getScale(), 0.25, &n);
        
        std::vector<MPVec3> voxels;
        
        for (int i = 0; i < n; ++i)
        {
            voxels.push_back(voxArray[i]);
        }
        
        obstacles_.insert(std::make_pair(obstacle, voxels));
        
        free(voxArray);
    }
}

PotentialFieldController::PotentialFieldController()
{
}

void PotentialFieldController::setGoal(const Transform3D &goal)
{
    goal_ = goal;
}

void PotentialFieldController::move() const
{
    // Perform a single step of gradient descent
    MPVec3 currentPosition = activeObject_->getPosition();
    MPVec3 gradient = potentialGrad(currentPosition);
    const float alpha = 0.01f;
    
    // TODO: Check for convergence (i.e. when gradient of potential function is close to zero
    // or, || gradient || < epsilon for some epsilon)
    MPVec3 nextPosition = MPVec3Subtract(currentPosition, MPVec3MultiplyScalar(gradient, alpha));
    
    activeObject_->setPosition(nextPosition);
}

MPVec3 PotentialFieldController::potentialGrad(const MPVec3 &p) const
{
    // By superposition and linearity, the gradient of the potential at p is simply the
    // sum of the individual contributions from all the "particles"
    MPVec3 potentialGrad = attractivePotentialGrad(p);
    
    const MPMat4 activeT = this->activeObject_->getTransform().getMatrix();
    MPSphere activeSphere = MPMeshGetBoundingSphere(this->activeObject_->getMesh(), &activeT);
    
    for(auto it = this->obstacles_.begin(); it != this->obstacles_.end(); ++it)
    {
        Model *obstacle = it->first;
        std::vector<MPVec3> voxels = it->second;
        
        const MPMat4 matrix = obstacle->getTransform().getMatrix();
        MPSphere boundingSphere = MPMeshGetBoundingSphere(obstacle->getMesh(), &matrix);
        
        if (MPSphereIntersectsSphere(boundingSphere, activeSphere))
        {
            for (auto vox : voxels)
            {
                // TODO: what should this radius be?
                MPVec3 repulsiveGrad = repulsivePotentialGrad(vox, p, 1.0f);
                
                potentialGrad.x += repulsiveGrad.x;
                potentialGrad.y += repulsiveGrad.y;
                potentialGrad.z += repulsiveGrad.z;
            }
        }
    }
    
    return potentialGrad;
}

MPVec3 PotentialFieldController::attractivePotentialGrad(const MPVec3 &p) const
{
    // TODO: Move these (arbitrarily-defined) constants somewhere better
    const float c = 4.0f;
    const float threshold = 1.0f;
    
    float distance = MPVec3EuclideanDistance(p, goal_.getPosition());
    
    MPVec3 potentialGrad = MPVec3Zero;
    
    if(distance <= threshold)
    {
        potentialGrad = MPVec3Subtract(p, goal_.getPosition());
        potentialGrad = MPVec3MultiplyScalar(potentialGrad, c);
    }
    else
    {
        potentialGrad = MPVec3Subtract(p, goal_.getPosition());
        potentialGrad = MPVec3MultiplyScalar(potentialGrad, (threshold * c) / (distance));
    }
    
    return potentialGrad;
}

MPVec3 PotentialFieldController::repulsivePotentialGrad(const MPVec3 &pObs, const MPVec3 &p, float P) const
{
    // TODO: Move this (arbitrarily-defined) constant somewhere better
    const float a = 10.0f;
    
    float distance = MPVec3EuclideanDistance(pObs, p);
    
    // The gradient of the Euclidean distance function
    MPVec3 distanceGrad = MPVec3Make(p.x - pObs.x, p.y - pObs.y, p.z - pObs.z);
    distanceGrad = MPVec3MultiplyScalar(distanceGrad, distance);

    if(distance > P)
        return MPVec3Zero;
    
    float multiplier = a * (1.0f / P - 1.0f / distance) * (1.0f / (distance * distance));
    
    // Compute the gradient of the repulsive potential function
    return MPVec3MultiplyScalar(distanceGrad, multiplier);
}
}