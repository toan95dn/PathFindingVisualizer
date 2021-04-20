#pragma once
#include "PathFindingAlgo.h"

class GreedyBFS : public PathFindingAlgo {
public:
	GreedyBFS(Grid& grid);
	bool Solve() override;

};
