#include "GreedyBFS.h"
#include <vector>
#include <algorithm>
#include"Engine.h"

GreedyBFS::GreedyBFS(Grid& grid):PathFindingAlgo(grid){
}

bool GreedyBFS::Solve() {
	startNode = grid.getStartNode();
	endNode = grid.getEndNode();
	startNode->gCost = 0;
	startNode->hCost = calDistance(startNode, endNode);
	startNode->isVisited = true;

	std::vector<Node*> openList;
	openList.push_back(startNode);

	while (!openList.empty()) {
		std::sort(openList.begin(), openList.end(), [](const Node* lhs, const Node* rhs) { return lhs->hCost > rhs->hCost; });
		Node* currNode = openList.back();
		openList.pop_back();
		grid.DrawGraph();
		SDL_RenderPresent(Engine::renderer);
		SDL_Delay(TIMEPERFRAME);
		for (auto& currNeighborNode : currNode->neighborNodes) {
			if (!currNeighborNode->isBlocked) {
				if (currNeighborNode->isVisited) {
					if (currNeighborNode->gCost > (currNode->gCost + calDistance(currNode, currNeighborNode))) {
						currNeighborNode->gCost = currNode->gCost + calDistance(currNeighborNode, currNode);
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
					openList.push_back(currNeighborNode);
				}
			}
		}
	}
	return false;
}