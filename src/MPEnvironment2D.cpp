#include "MPEnvironment2D.h"

namespace MP
{

Environment2D::Environment2D(int xMin, int xMax, int yMin, int yMax)
  : Environment<Point2D>(point2DHash), xMin_(xMin), xMax_(xMax), yMin_(yMin), yMax_(yMax),
    obstacles_(point2DHash)
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
	// If the state is an obstacle, do not add it as a neighbor
	if(obstacles_.get(p) != nullptr)
	{
	  std::cout << "(" << xs+i << ", " << ys+j << ") is an obstacle" << std::endl;
	  continue;
	}

	// If the state has already been allocated, do not re-allocate a duplicate
	SearchState2D *neighbor = states_.get(p);
	if(neighbor == nullptr)
	{
	  std::cout << "Allocating state (" << p.x_ << ", " << p.y_ << ")" << std::endl;
	  neighbor = new SearchState2D();
	  neighbor->setValue(p);
	  neighbor->setParent(s);
	  states_.insert(neighbor);
	}
	else
	{
	  std::cout << "Already seen (" << p.x_ << ", " << p.y_ << ")" << std::endl;
	}
	successors.push_back(neighbor);
	double cost;
	getCost(s, neighbor, cost);
	costs.push_back(cost);
      }
    }
  }
}

bool Environment2D::getCost(SearchState2D *s, SearchState2D *t, double &cost)
{
  int xDiff = std::abs(s->getValue().x_ - t->getValue().x_);
  int yDiff = std::abs(s->getValue().y_ - t->getValue().y_);
  // Check reachability
  if(xDiff > 1 || yDiff > 1)
  {
    std::cout << "State unreachable" << std::endl;
    return false;
  }

  // @todo increase cost of moving along diagonal
  cost = 1.0f;

  return true;
}

void Environment2D::readMap(const std::string &file)
{
  std::ifstream f(file);

  int xMin, xMax, yMin, yMax;
  f >> xMin >> xMax >> yMin >> yMax;

  // @todo check validity of the limits
  xMin_ = xMin;
  xMax_ = xMax;
  yMin_ = yMin;
  yMax_ = yMax;

  std::cout << "Reading map [" << xMin << ", " << xMax << "] x ["
	    << yMin << ", " << yMax << "]" << std::endl;

  int cell;
  for(int y = yMin; y < yMax; ++y)
  {
    for(int x = xMin; x < xMax; ++x)
    {
      f >> cell;
      //std::cout << cell << " ";
      if(cell > 0)
      {
	Point2D o(x, y);
	std::cout << "Adding obstacle at (" << x << ", " << y << ")" << std::endl;
	SearchState2D *state = this->addState(o);
	obstacles_.insert(state);
      }
    }
    //std::cout << std::endl;
  }

  f.close();
}

void Environment2D::writeMap(const std::string &file, const std::vector<Point2D> &path)
{
  std::ofstream f(file);

  std::cout << "Writing map [" << xMin_ << ", " << xMax_ << "] x ["
	    << yMin_ << ", " << yMax_ << "]" << std::endl;
  
  for(int y = yMin_; y < yMax_; ++y)
  {
    for(int x = xMin_; x < xMax_; ++x)
    {
      Point2D p(x, y);
      bool onPath = false;
      // @todo ugly
      for(int i = 0; i < path.size(); ++i)
      {
	if(path[i].x_ == p.x_ && path[i].y_ == p.y_)
	{
	  onPath = true;
	  break;
	}
      }
      if(onPath)
	f << "* ";
      else if(obstacles_.get(p))
	f << "1 ";
      else
	f << "0 ";
    }
    f << std::endl;
  }

  f.close();
}

}
