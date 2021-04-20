#include "Grid.h"
#include "Constants.h"
#include "Engine.h"
#include <iostream>
#include <random>
#include <stack>

Grid::Grid(){
	nodes = new Node[MAX_ROWS * MAX_COLUMNS];
	//Init all the nodes, set color for each node and get all the neighbors
	for (int row = 0; row < MAX_ROWS; row++) {
		for (int col = 0; col < MAX_COLUMNS; col++) {
			Node& currNode = NodeAtIndex(row,col);
			currNode.Init(row, col);
			currNode.SetColor(255, 255, 255, 255);
			//Get all the neighbor of each Node up,down,left,right
			if (row > 0)
				currNode.neighborNodes.push_back(&NodeAtIndex(row - 1, col));
			if (row < MAX_ROWS - 1)
				currNode.neighborNodes.push_back(&NodeAtIndex(row + 1, col));
			if (col > 0)
				currNode.neighborNodes.push_back(&NodeAtIndex(row, col - 1));
			if (col < MAX_COLUMNS - 1)
				currNode.neighborNodes.push_back(&NodeAtIndex(row, col + 1));
		}
	}
}

Grid::~Grid(){
	delete[] nodes;
}

Node& Grid::NodeAtIndex(int row, int column){
	return nodes[row + MAX_COLUMNS*column];
}

void Grid::DrawGraph(){
	for (int row = 0; row < MAX_ROWS; row++) {
		for (int col = 0; col < MAX_COLUMNS; col++) {
			NodeAtIndex(row, col).Draw();
		}
	}

	for (int row = 0; row <= MAX_ROWS; row++) {
			SDL_SetRenderDrawColor(Engine::renderer, 0, 0, 0, 255);
			SDL_RenderDrawLine(Engine::renderer, 0, TILE_LENGTH * row, GRID_WIDTH, TILE_LENGTH * row);
			SDL_RenderDrawLine(Engine::renderer, TILE_LENGTH * row, 0, TILE_LENGTH * row, GRID_HEIGHT);
	}
}

void Grid::SetStartNodeAtIndex(int rowX, int colY) {
	Node* currNode = &NodeAtIndex(rowX, colY);
	if (currNode != endNode && !currNode->isBlocked) {
		if (!hasStartNode) {
			startNode = currNode;
			startNode->parent = nullptr;
			startNode->SetColor(startNodeColor_R, startNodeColor_G, startNodeColor_B, startNodeColor_A);
			startNode->changeColorOverTime = false;
			hasStartNode = true;
		}
		else {
			//remove the old start node (only need to reset the color to white), then set the new one
			startNode->SetColor(255, 255, 255, 255);
			startNode = currNode;
			startNode->parent = nullptr;
			startNode->SetColor(startNodeColor_R, startNodeColor_G, startNodeColor_B, startNodeColor_A);
			startNode->changeColorOverTime = false;
			hasStartNode = true;
		}
	}
}

void Grid::SetEndNodeAtIndex(int row, int col) {
	Node* currNode = &NodeAtIndex(row, col);
	if (currNode != startNode && !currNode->isBlocked) {
		if (!hasEndNode) {
			endNode = currNode;
			endNode->SetColor(endNodeColor_R, endNodeColor_G, endNodeColor_B, endNodeColor_A);
			endNode->changeColorOverTime = false;
			hasEndNode = true;
		}
		else {
			//remove the old start node, set the new one
			endNode->SetColor(255, 255, 255, 255);
			endNode = currNode;
			endNode->SetColor(endNodeColor_R, endNodeColor_G, endNodeColor_B, endNodeColor_A);
			endNode->changeColorOverTime = false;
			hasEndNode = true;
		}
	}
}

void Grid::SetWallAtIndex(int row, int col) {
	Node* currNode = &NodeAtIndex(row, col);

	if (currNode != startNode && currNode != endNode) {
		currNode->isBlocked = true;
		currNode->isVisited = false;
		currNode->parent = nullptr;
		currNode->changeColorOverTime = false;
		currNode->SetColor(wallNodeColor_R, wallNodeColor_G, wallNodeColor_B, wallNodeColor_R);
	}
}

void Grid::DeleteWallAtIndex(int row, int pos) {
	Node* currNode = &NodeAtIndex(row, pos);
	if (currNode->isBlocked) {
		currNode->isBlocked = false;
		currNode->isVisited = false;
		currNode->parent = nullptr;
		currNode->SetColor(defaultTileColor_R, defaultTileColor_G, defaultTileColor_B, defaultTileColor_A);
	}
}


void Grid::enableDiagonal(){
	hasDiagnoalPath = true;
	//Add all neighbor nodes diagnoally "without adding the obstacles"
	for (int row = 0; row < MAX_ROWS; row++) {
		for (int col = 0; col < MAX_COLUMNS; col++) {
			Node& currNode = NodeAtIndex(row, col);
			if (!currNode.isBlocked) {
				if (row > 0 && col > 0) {
					if (!NodeAtIndex(row - 1, col).isBlocked && !NodeAtIndex(row, col - 1).isBlocked) {
						currNode.neighborNodes.push_back(&NodeAtIndex(row - 1, col - 1));
					}
				}
				if (row < MAX_ROWS - 1 && col >0) {
					if (!NodeAtIndex(row, col - 1).isBlocked && !NodeAtIndex(row + 1, col).isBlocked) {
						currNode.neighborNodes.push_back(&NodeAtIndex(row + 1, col - 1));
					}
				}
				if (row > 0 && col < MAX_COLUMNS - 1) {
					if (!NodeAtIndex(row - 1, col).isBlocked && !NodeAtIndex(row, col + 1).isBlocked) {
						currNode.neighborNodes.push_back(&NodeAtIndex(row - 1, col + 1));
					}
				}
				if (row < MAX_ROWS - 1 && col < MAX_COLUMNS - 1) {
					if (!NodeAtIndex(row + 1, col).isBlocked && !NodeAtIndex(row, col + 1).isBlocked) {
						currNode.neighborNodes.push_back(&NodeAtIndex(row + 1, col + 1));
					}
				}
			}
		}
	}
}

void Grid::enableDiagonalandCrossCorner(){
	hasDiagnoalPath = true;
	//Add all neighbor nodes diagnoally "without adding the obstacles"
	for (int row = 0; row < MAX_ROWS; row++) {
		for (int col = 0; col < MAX_COLUMNS; col++) {
			Node& currNode = NodeAtIndex(row, col);
			if (!currNode.isBlocked) {
				if (row > 0 && col > 0) {
					if (!NodeAtIndex(row - 1, col).isBlocked || !NodeAtIndex(row, col - 1).isBlocked) {
						currNode.neighborNodes.push_back(&NodeAtIndex(row - 1, col - 1));
					}
				}
				if (row < MAX_ROWS - 1 && col >0) {
					if (!NodeAtIndex(row, col - 1).isBlocked || !NodeAtIndex(row + 1, col).isBlocked) {
						currNode.neighborNodes.push_back(&NodeAtIndex(row + 1, col - 1));
					}
				}
				if (row > 0 && col < MAX_COLUMNS - 1) {
					if (!NodeAtIndex(row - 1, col).isBlocked ||!NodeAtIndex(row, col + 1).isBlocked) {
						currNode.neighborNodes.push_back(&NodeAtIndex(row - 1, col + 1));
					}
				}
				if (row < MAX_ROWS - 1 && col < MAX_COLUMNS - 1) {
					if (!NodeAtIndex(row + 1, col).isBlocked || !NodeAtIndex(row, col + 1).isBlocked) {
						currNode.neighborNodes.push_back(&NodeAtIndex(row + 1, col + 1));
					}
				}
			}
		}
	}
}

void Grid::disableDiagnoal(){
	hasDiagnoalPath = false;
}

void Grid::GenerateMazeBackTracking(){//Recursive backtracker
	srand(clock());
	//Set all walls
	for (int row = 0; row < MAX_ROWS; row++) {
		for (int col = 0; col < MAX_COLUMNS; col++) {
			SetWallAtIndex(row, col);
		}
	}
	//Let it start at (0,0)
	Node* currNode = &NodeAtIndex(rand()%MAX_ROWS, rand()%MAX_COLUMNS);
	DeleteWallAtIndex(currNode->rowPos, currNode->colPos);
	//
	std::stack<Node*> stackNodes;
	stackNodes.push(currNode);
	while (!stackNodes.empty()) {
		Node* currNode = stackNodes.top();
		//Add all neighbor nodes of the maze, +-2 index because we want a space between 2 walls to move
		std::vector<Node*> neighborNodes;
		if (currNode->rowPos > 1) {
			if (NodeAtIndex(currNode->rowPos - 2, currNode->colPos).isBlocked) {
				neighborNodes.push_back(&NodeAtIndex(currNode->rowPos - 2, currNode->colPos));
			}
		}
		if (currNode->rowPos < MAX_ROWS - 2) {
			if (NodeAtIndex(currNode->rowPos + 2, currNode->colPos).isBlocked) {
				neighborNodes.push_back(&NodeAtIndex(currNode->rowPos + 2, currNode->colPos));
			}
		}
		if (currNode->colPos > 1) {
			if (NodeAtIndex(currNode->rowPos, currNode->colPos - 2).isBlocked) {
				neighborNodes.push_back(&NodeAtIndex(currNode->rowPos, currNode->colPos - 2));
			}
		}
		if (currNode->colPos < MAX_COLUMNS - 2) {
			if (NodeAtIndex(currNode->rowPos, currNode->colPos + 2).isBlocked) {
			    neighborNodes.push_back(&NodeAtIndex(currNode->rowPos, currNode->colPos + 2));
			}
		}

		if (neighborNodes.size() != 0) {
			int randomIndex = rand() % neighborNodes.size();
			Node* deletedNode = neighborNodes[randomIndex];
			DeleteWallAtIndex(deletedNode->rowPos, deletedNode->colPos);
			DeleteWallAtIndex((deletedNode->rowPos + currNode->rowPos) / 2 , (deletedNode->colPos + currNode->colPos) / 2);
			DrawGraph();
			SDL_RenderPresent(Engine::renderer);
			SDL_Delay(FRAMEPERSECOND);
			stackNodes.push(deletedNode);
		}
		else {
			stackNodes.pop();
		}
	}
}

void Grid::GenerateMazeDivision(){
	Devide(0,0,MAX_ROWS,MAX_COLUMNS);
}

void Grid::Devide(int row, int col, int width, int height){
	//Always try to build at an odd index while cut an even index
	srand(clock());
	if (width < 2 || height < 2)
		return;
	bool IsDevidedVetically = WillBeDevidedVertically(width,height);
	if(IsDevidedVetically){//Devided vertically
		int buildWallsFromColumn;
		int deleteWallAtRow;
		buildWallsFromColumn = 2 * (rand() % (width / 2)) + 1 + col;
		deleteWallAtRow = 2 * (rand() % (1 + height / 2)) + row;

		//Draw walls then delete a wall 
		for (int currRow = row; currRow < row + height; currRow++) {
			SetWallAtIndex(currRow, buildWallsFromColumn);
			DrawGraph();
			SDL_RenderPresent(Engine::renderer);
			SDL_Delay(TIMEPERFRAME);
		}
		DeleteWallAtIndex(deleteWallAtRow, buildWallsFromColumn);
		//Continue to devide smaller parts
		Devide(row,col,buildWallsFromColumn - col,height);
		Devide(row,buildWallsFromColumn+1,width - (buildWallsFromColumn - col + 1),height);
	}
	else {//Devided horizontally
		int buildWallsFromRow;
		int deleteWallAtColumn;
		buildWallsFromRow = 2 * (rand() % (height / 2)) + 1+ row;
		deleteWallAtColumn = 2 * (rand() % (1 + width / 2)) + col ;	

		//Draw walls then delete a wall
		for (int currCol = col; currCol < col + width; currCol++) {
			SetWallAtIndex(buildWallsFromRow, currCol);
			DrawGraph();
			SDL_RenderPresent(Engine::renderer);
			SDL_Delay(TIMEPERFRAME);
		}
		DeleteWallAtIndex(buildWallsFromRow, deleteWallAtColumn);
		//Continue to devide smaller parts
		Devide(row,col,width, buildWallsFromRow - row);
		Devide(buildWallsFromRow + 1,col,width,height - (buildWallsFromRow -row +1));
	}
}

bool Grid::WillBeDevidedVertically(int width, int height){
	srand(clock());
	if (width > height)
		return true;
	else if (height > width)
		return false;
	else {
		return rand() % 2;
	}
}

void Grid::resetGrid(){
	startNode->isVisited = false;
	startNode->parent = nullptr;
	endNode->isVisited = false;
	endNode->parent = nullptr;

	/*If we just solved then we want to add a new wall diagnoally, the new wall is still in the old "neighbor list"
	* This clear all the neighbor node and reset the neighbor nodes to be nodes on left, right, up, and down only
	* to avoid the bug
	*/
	for (int row = 0; row < MAX_ROWS; row++) {
		for (int col = 0; col < MAX_COLUMNS; col++) {
			Node& currNode = NodeAtIndex(row, col);
			//if it has DiagnoalPath, clean all the diagnoal neighbors, then reset
			if (hasDiagnoalPath) {
				currNode.neighborNodes.clear();
				if (row > 0)
					currNode.neighborNodes.push_back(&NodeAtIndex(row - 1, col));
				if (row < MAX_ROWS - 1)
					currNode.neighborNodes.push_back(&NodeAtIndex(row + 1, col));
				if (col > 0)
					currNode.neighborNodes.push_back(&NodeAtIndex(row, col - 1));
				if (col < MAX_COLUMNS - 1)
					currNode.neighborNodes.push_back(&NodeAtIndex(row, col + 1));
			}
			//reset all the visited Nodes
			if (!currNode.isBlocked && &currNode != startNode && &currNode != endNode) {
				currNode.isVisited = false;
				currNode.parent = nullptr;
				currNode.SetColor(255, 255, 255, 255);
				currNode.changeColorOverTime = false;
			}
		}
	}
}

void Grid::clearGrid(){
	for (int row = 0; row < MAX_ROWS; row++) {
		for (int col = 0; col < MAX_COLUMNS; col++) {
			Node& currNode = NodeAtIndex(row, col);
			currNode.isVisited = false;
			currNode.isBlocked = false;
			currNode.SetColor(defaultTileColor_R, defaultTileColor_G, defaultTileColor_B, defaultTileColor_A);
			currNode.changeColorOverTime = false;
		}
	}
	startNode = nullptr;
	endNode = nullptr;
	hasStartNode = false;
	hasEndNode = false;
}


