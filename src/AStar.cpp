#include "AStar.h"
#include <vector>
#include <algorithm>
#include"Engine.h"

AStar::AStar(Grid& grid) :PathFindingAlgo{ grid } {
}

bool AStar::Solve() {
	startNode = grid.getStartNode();
	endNode = grid.getEndNode();
	startNode->gCost = 0;
	startNode->hCost = calDistance(startNode,endNode);
	startNode->fCost = startNode->hCost;
	startNode->isVisited = true;

	std::vector<Node*> openList;
	openList.push_back(startNode);

	while (!openList.empty()) {
		std::sort(openList.begin(), openList.end(), [](const Node* lhs, const Node* rhs) { return lhs->fCost > rhs->fCost; });
		Node* currNode = openList.back();
		openList.pop_back();
		grid.DrawGraph();
		SDL_RenderPresent(Engine::renderer);
		SDL_Delay(TIMEPERFRAME);
		for (auto& currNeighborNode : currNode->neighborNodes) {
			if (!currNeighborNode->isBlocked) {
				if (currNeighborNode->isVisited) {
					if (currNeighborNode->gCost > (currNode->gCost + calDistance(currNode,currNeighborNode))) {
						currNeighborNode->gCost = currNode->gCost + calDistance(currNeighborNode,currNode);
						currNeighborNode->fCost = currNeighborNode->gCost + currNeighborNode->hCost;
						currNeighborNode->parent = currNode;
					}
				}
				else {
					currNeighborNode->isVisited = true;
					currNeighborNode->parent = currNode;
					if (currNeighborNode == endNode) {
						return true;
					}
					currNeighborNode->SetColor(visitedNodeColor_A, visitedNodeColor_G, visitedNodeColor_B, visitedNodeColor_A);
					currNeighborNode->changeColorOverTime = true;
					currNeighborNode->gCost = currNode->gCost + calDistance(currNode, currNeighborNode);
					currNeighborNode->hCost = calDistance(currNeighborNode, endNode);
					currNeighborNode->fCost = currNeighborNode->gCost + currNeighborNode->hCost;
					openList.push_back(currNeighborNode);
				}
			}
		}
	}
	return false;
}

