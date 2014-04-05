#include "MPEnvironment2D.h"

namespace MP
{

Environment2D::Environment2D(int xMin, int xMax, int yMin, int yMax)
  : xMin_(xMin), xMax_(xMax), yMin_(yMin), yMax_(yMax), states_(point2DHash)
{
}

Environment2D::~Environment2D()
{
}

void Environment2D::getSuccessors(SearchState2D *s,
				  std::vector<SearchState2D *> &successors,
				  std::vector<double> &costs)
{
  if(states_.get(s->getValue()) == nullptr)
    states_.insert(s);

  int xs = s->getValue().x_;
  int ys = s->getValue().y_;

  // Assume 8-connectedness of the 2D grid world
  for(int i = -1; i <= 1; ++i)
  {
    for(int j = -1; j <= 1; ++j)
    {
      if(i == 0 && j == 0) continue;
      
      // Ensure that the candidate neighbor is in-bounds
      if(xs+i >= xMin_ && xs+i <= xMax_ && ys+j >= yMin_ && ys+j <= yMax_)
      {
	Point2D p(xs+i, ys+j);
	// If the state has already been allocated, do not re-allocate a duplicate
	SearchState2D *neighbor = states_.get(p);
	if(neighbor == nullptr)
	{
	  std::cout << "Allocating state (" << p.x_ << ", " << p.y_ << ")" << std::endl;
	  neighbor = new SearchState2D();
	  neighbor->setValue(p);
	  states_.insert(neighbor);
	}
	else
	{
	  std::cout << "Already seen (" << p.x_ << ", " << p.y_ << ")" << std::endl;
	}
	successors.push_back(neighbor);
	costs.push_back(1.0);
      }
    }
  }
}

}
