#include "MPEnvironment2D.h"

namespace MP
{

Environment2D::Environment2D(int xMin, int xMax, int yMin, int yMax)
  : xMin_(xMin), xMax_(xMax), yMin_(yMin), yMax_(yMax)
{
}

Environment2D::~Environment2D()
{
}

void Environment2D::getSuccessors(SearchState2D *s,
				  std::vector<SearchState2D *> &successors,
				  std::vector<double> &costs)
{
  // @todo
}

}
