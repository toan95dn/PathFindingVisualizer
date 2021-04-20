#pragma once
#include<SDL.h>
#include<SDL_image.h>
#include "Grid.h"
#include "PathFindingAlgo.h"
#include <Button.h>
class Engine{
public:
	Engine();
	~Engine();

	static SDL_Renderer* renderer;
	static SDL_Event event;

	bool IsRunning();

	void Init();
	void InputProcessing();
	void Update();
	void Render();

private:
	SDL_Window* window;
	bool isRunning;
	Grid* grid;
	bool startSolve;
	PathFindingAlgo* currAlgo;
	//managing button click
	std::vector<Button*> mapSetUpButtons;
	std::vector<Button*> algoSetUpButtons;
	std::vector<Button*> tags;
	std::vector<Button*> mazeGenButtons;
	std::vector<Button*> movementModeButtons;
	Button* solveButton;
	Button* clearGridButton;
	void LoadButtons();
	void DestroyButtons();
	bool isReadyToSolve();
	//
	bool isPressed = false;
	bool isBeingExecuted = false;
};

