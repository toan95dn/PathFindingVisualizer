#include  "PathFindingAlgo.h"
#include "Engine.h"
#include <cmath>
#include <iostream>
#include<stack>
PathFindingAlgo::PathFindingAlgo(Grid& grid):grid(grid){
}
void PathFindingAlgo::ConstructPath() {
	Node* currNode = endNode->parent;
	std::stack<Node*> pathNodes;
	while (currNode->parent != nullptr) {
		pathNodes.push(currNode);
		currNode = currNode->parent;
	}
	while (!pathNodes.empty()) {
		currNode = pathNodes.top();
		currNode->SetColor(pathColor_R, pathColor_G, pathColor_B, pathColor_A);
		currNode->changeColorOverTime = false;
		grid.DrawGraph();
		SDL_RenderPresent(Engine::renderer);
		SDL_Delay(TIMEPERFRAME);
		pathNodes.pop();
	}
}

float PathFindingAlgo::calDistance(Node* lhsNode, Node* rhsNode) {
	return sqrt(pow(lhsNode->rowPos - rhsNode->rowPos, 2) + pow(lhsNode->colPos - rhsNode->colPos, 2));
}