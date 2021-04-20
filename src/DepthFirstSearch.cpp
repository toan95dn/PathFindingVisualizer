#include "DepthFirstSearch.h"
#include <Engine.h>
#include <stack>
#include <iostream>

DepthFirstSearch::DepthFirstSearch(Grid& grid):PathFindingAlgo{grid}{
}

bool DepthFirstSearch::Solve(){

	startNode = grid.getStartNode();
	endNode = grid.getEndNode();
	startNode->isVisited = true;
	
	std::stack<Node*> stackNodes;
	stackNodes.push(startNode);
	while (!stackNodes.empty()) {
		grid.DrawGraph();
		SDL_RenderPresent(Engine::renderer);
		SDL_Delay(TIMEPERFRAME);
		Node* currNode = stackNodes.top();
		stackNodes.pop();
		for (auto& currNeighborNode : currNode->neighborNodes) {
			if (!currNeighborNode->isVisited && !currNeighborNode->isBlocked) {
				currNeighborNode->parent = currNode;
				currNeighborNode->isVisited = true;
				if (currNeighborNode == endNode)
					return true;
				currNeighborNode->SetColor(visitedNodeColor_A, visitedNodeColor_G, visitedNodeColor_B, visitedNodeColor_A);
				currNeighborNode->changeColorOverTime = true;
				stackNodes.push(currNeighborNode);
			}
		}
	}
	return false;
}
