#pragma once
#include "PathFindingAlgo.h"
class AStar : public PathFindingAlgo {
public:
	AStar(Grid& grid);
	bool Solve() override;
};
