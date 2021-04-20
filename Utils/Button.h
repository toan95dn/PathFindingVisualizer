#pragma once
#include <SDL.h>
#include <iostream>
enum BUTTON_TYPE {
	START_BUTTON = 0, END_BUTTON = 1, WALL_BUTTON = 2, DELETEWALLS_BUTTON = 3, TAG = 4, ASTAR = 5, DIJKSTRA = 6,
	GREEDYBFS = 7, DFS = 8, BFS = 9, RBACKTRACK = 10, RDIVISION = 11, ALLOWDIAGNOAL = 12, ALLOWCROSSCORNER =13,SOLVE =14,CLEAR=15
};
class Button {
private:
	SDL_Texture* texture;
	SDL_Rect sourceRect;
	SDL_Rect destinationRect;
	bool isIndependent;//the button is not depended on others, if yes it is gonna turned off when another button in the group is on
public:
	Button(std::string filePath, int posX, int posY, BUTTON_TYPE buttonType, bool isIndependent);
	~Button();
	const BUTTON_TYPE buttonType;
	bool isOn = false;
	bool isClicked(int x,int y);
	void Draw();
};