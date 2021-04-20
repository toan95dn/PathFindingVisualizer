#pragma once
#include "PathFindingAlgo.h"
class Dijkstra:public PathFindingAlgo{
public:
	Dijkstra(Grid& grid);
	bool Solve() override;
};

