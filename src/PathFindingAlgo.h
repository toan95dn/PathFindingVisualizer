#pragma once
#include "Grid.h"
class PathFindingAlgo {
protected:
	Grid& grid;
	Node* startNode;
	Node* endNode;
	float calDistance(Node* lhsNode, Node* rhsNode);//const ????
public:
	PathFindingAlgo(Grid& grid);
	void ConstructPath();
	virtual bool Solve() = 0;
};