#include "BreadthFS.h"
#include "Engine.h"
#include<queue>
BreadthFS::BreadthFS(Grid& grid) :PathFindingAlgo{grid} {
}

bool BreadthFS::Solve(){
	startNode = grid.getStartNode();
	endNode = grid.getEndNode();
	startNode->isVisited = true;

	std::queue<Node*> queueNodes;
	queueNodes.push(startNode);
	while (!queueNodes.empty()) {
		grid.DrawGraph();
		SDL_RenderPresent(Engine::renderer);
		SDL_Delay(TIMEPERFRAME);
		Node* currNode = queueNodes.front();
		queueNodes.pop();
		for (auto& currNeighborNode : currNode->neighborNodes) {
			if (!currNeighborNode->isVisited && !currNeighborNode->isBlocked) {
				currNeighborNode->parent = currNode;
				currNeighborNode->isVisited = true;
				if (currNeighborNode == endNode)
					return true;
				currNeighborNode->SetColor(visitedNodeColor_A, visitedNodeColor_G, visitedNodeColor_B, visitedNodeColor_A);
				currNeighborNode->changeColorOverTime = true;
				queueNodes.push(currNeighborNode);
			}
		}
	}
	return false;
}
