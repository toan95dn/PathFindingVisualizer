#pragma once
#include "PathFindingAlgo.h"
class BreadthFS: public PathFindingAlgo{//same as depth 1st search, just explore children first using queue
public:
	BreadthFS(Grid& grid);
	bool Solve() override;
};

