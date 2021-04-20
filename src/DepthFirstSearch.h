#pragma once
#include "PathFindingAlgo.h"

class DepthFirstSearch: public PathFindingAlgo{
public:
	DepthFirstSearch(Grid& grid);
	bool Solve() override;
};

