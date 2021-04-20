#include "Node.h"
#include "Engine.h"
#include "Constants.h"
void Node::Init(int row, int col) {
	rowPos = row;
	colPos = col;
	tile.x = col * TILE_LENGTH;
	tile.y = row * TILE_LENGTH;
	tile.w = TILE_LENGTH;
	tile.h = TILE_LENGTH;
	changeColorOverTime = false;
}

void Node::SetColor(int r, int g, int b, int a) {
	this->tileColorR = r;
	this->tileColorG = g;
	this->tileColorB = b;
	this->tileColorA = a;
}

void Node::Draw() {
	if (changeColorOverTime){//The color will change overtime if it is being visited
		if (tileColorR > finalVisitedNodeColor_R) {
			tileColorR--;
		}
		if (tileColorG < finalVisitedNodeColor_G) {
			tileColorG++;
		}
		if (tileColorB < finalVisitedNodeColor_B) {
			tileColorB++;
		}

	}
	SDL_SetRenderDrawColor(Engine::renderer, tileColorR, tileColorG, tileColorB, tileColorA);
	SDL_RenderFillRect(Engine::renderer, &tile);
}