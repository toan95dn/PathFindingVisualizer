#pragma once
#include "Constants.h"
#include<vector>
#include <SDL.h>
struct Node {
	int rowPos;
	int colPos;
	float gCost;
	float hCost;
	float fCost;
	bool isVisited;
	bool isBlocked;
	Node* parent;
	std::vector<Node*> neighborNodes;

	Node() {
		gCost = INFINITY;
		hCost = INFINITY;
		fCost = INFINITY;
		parent = nullptr;
		isVisited = false;
		isBlocked = false;
	}


	//For visualizing each node = a tile on the grid
	SDL_Rect tile;
	int tileColorR;
	int tileColorG;
	int tileColorB;
	int tileColorA;
	bool changeColorOverTime;

	void Init(int row, int col);
	void SetColor(int r, int b, int g, int a);
	void Draw();

};

