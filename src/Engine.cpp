#include "Engine.h"
#include "Constants.h"
#include <iostream>
#include "AStar.h"
#include "Dijkstra.h"
#include "GreedyBFS.h"
#include "DepthFirstSearch.h"
#include "BreadthFS.h"
SDL_Renderer* Engine::renderer;
SDL_Event Engine::event;

Engine::Engine() {
	isRunning = false;
	startSolve = false;
	grid = new Grid();
}

Engine::~Engine() {
	delete currAlgo;
	delete grid;
	DestroyButtons();
}

bool Engine::IsRunning() {
	return isRunning;
}

void Engine::Init() {
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		std::cerr << "Error initializing SDL" << std::endl;
		return;
	}


	window = SDL_CreateWindow(NULL, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_BORDERLESS);
	if (!window) {
		std::cerr << "Error creating the window" << std::endl;
		return;
	}

	renderer = SDL_CreateRenderer(window, -1, 0);
	if (!renderer) {
		std::cerr << "Error creating the renderer" << std::endl;
		return;
	}

	LoadButtons();
	isRunning = true;

	return;
}

void Engine::Update() {
	//check if we can go diagnoally/ cross corner
	bool canGoDianoally = false;
	bool canCrossCorner=false;

	for (auto currMovementModeButton : movementModeButtons) {
		if (currMovementModeButton->isOn) {
			if (currMovementModeButton->buttonType == ALLOWDIAGNOAL) {
				canGoDianoally = true;
			}
			else if (currMovementModeButton->buttonType == ALLOWCROSSCORNER) {
				canCrossCorner = true;
			}
		}
	}

	if (grid->getStartNode() && grid->getEndNode() && currAlgo) {
		if (startSolve) {
			grid->resetGrid();
			if (canGoDianoally && canCrossCorner) {
				grid->enableDiagonalandCrossCorner();
			}
			else if (canGoDianoally && !canCrossCorner) {
				grid->enableDiagonal();
			}
			//Will change to algo->solve,algo->construct path
			if (currAlgo->Solve()) {
				currAlgo->ConstructPath();
			}
			startSolve = false;
			solveButton->isOn = false;
		}
	}

}

void Engine::InputProcessing() {
	SDL_PollEvent(&event);
	int x, y;
	SDL_GetMouseState(&x, &y);
	switch (event.type) {
	case SDL_QUIT: {
		isRunning = false;
		break;
	}
	case SDL_KEYDOWN: {
		if (event.key.keysym.sym == SDLK_ESCAPE) {
			isRunning = false;
		}
		if (event.key.keysym.sym == SDLK_SPACE) {
			startSolve = true;
		}
		if (event.key.keysym.sym == SDLK_h) {
			grid->clearGrid();
			grid->GenerateMazeDivision();
			//grid->GenerateMazeBackTracking();
		}
		break;
	}
	case (SDL_MOUSEBUTTONDOWN): {
		isPressed = true;
		if (event.button.button == SDL_BUTTON_LEFT) {
			if (x > GRID_WIDTH || y > GRID_HEIGHT) {
				if (y < 210) {//The area of buttons that set up the map
					for (auto currButton : mapSetUpButtons) {
						currButton->isClicked(x, y);
					}
				}
				else if (y > 210 && y < 500 && x>GRID_WIDTH) {//The area of buttons that pick the algo
					for (auto currButton : algoSetUpButtons) {
						if (currButton->isClicked(x, y)) {
							if (currButton->buttonType == ASTAR) {
								delete currAlgo;
								currAlgo = new AStar(*grid);
							}
							else if (currButton->buttonType == DIJKSTRA) {
								delete currAlgo;
								currAlgo = new Dijkstra(*grid);
							}
							else if (currButton->buttonType == GREEDYBFS) {
								delete currAlgo;
								currAlgo = new GreedyBFS(*grid);
							}
							else if (currButton->buttonType == DFS) {
								delete currAlgo;
								currAlgo = new DepthFirstSearch(*grid);
							}
							else if (currButton->buttonType == BFS) {
								delete currAlgo;
								currAlgo = new BreadthFS(*grid);
							}
						}
					}
				}
				else if (y > 500 && y < 650 && x > GRID_WIDTH) {
					for (auto currButton : mazeGenButtons) {
						if (currButton->isClicked(x, y)) {
							if (currButton->buttonType == RBACKTRACK) {
								currButton->Draw();
								currButton->isOn = false;
								grid->clearGrid();
								grid->GenerateMazeBackTracking();
							}
							else if (currButton->buttonType == RDIVISION) {
								currButton->Draw();
								currButton->isOn = false;
								grid->clearGrid();
								grid->GenerateMazeDivision();
							}
						}
					}
				}
				else if(y>GRID_HEIGHT){
					for (auto currButton : movementModeButtons) {
						if (currButton->isClicked(x, y)) {
							if (currButton->isOn) {
								currButton->isOn = false;
							}
							else {
								currButton->isOn = true;
							}
							SDL_Delay(250);
						}
					}
					if (solveButton->isClicked(x, y) && isReadyToSolve() && isBeingExecuted == false) {
						solveButton->isOn = true;
						solveButton->Draw();
						startSolve = true;
					}
					if (clearGridButton->isClicked(x,y)) {
						clearGridButton->isOn = true;
						clearGridButton->Draw();
						//Do this so that the button will be ligted up a little and then turned off immediately
						SDL_RenderPresent(renderer);
						SDL_Delay(50);
						grid->clearGrid();
						clearGridButton->isOn = false;
					}
				}
			}
			else {//In the map
				Button* currPressedButton = nullptr;
				for (auto currButton : mapSetUpButtons) {
					if (currButton->isOn)
						currPressedButton = currButton;
				}
				if (currPressedButton) {
					if (currPressedButton->buttonType == START_BUTTON) {
						grid->SetStartNodeAtIndex(y / TILE_LENGTH, x / TILE_LENGTH);
					}
					else if (currPressedButton->buttonType == END_BUTTON) {
						grid->SetEndNodeAtIndex(y / TILE_LENGTH, x / TILE_LENGTH);
					}
					else if (currPressedButton->buttonType == WALL_BUTTON) {
						grid->SetWallAtIndex(y / TILE_LENGTH, x / TILE_LENGTH);
					}
					else if (currPressedButton->buttonType == DELETEWALLS_BUTTON) {
						grid->DeleteWallAtIndex(y / TILE_LENGTH, x / TILE_LENGTH);
					}
				}
			}
		}
		break;
	}
	case SDL_MOUSEBUTTONUP: {
		isPressed = false;
	}
	case SDL_MOUSEMOTION: {
		if (isPressed) {
			if (x < GRID_WIDTH && y < GRID_HEIGHT) {
				//Check if wall button is on and we are pressing the mouse in the map to create walls
				Button* currMapSetUpButton = nullptr;
				for (auto currButton : mapSetUpButtons) {
					if (currButton->isOn)
						currMapSetUpButton = currButton;
				}
				if (currMapSetUpButton) {
					if (currMapSetUpButton->buttonType == WALL_BUTTON) {
						grid->SetWallAtIndex(y / TILE_LENGTH, x / TILE_LENGTH);
					}
					else if (currMapSetUpButton->buttonType == DELETEWALLS_BUTTON) {
						grid->DeleteWallAtIndex(y / TILE_LENGTH, x / TILE_LENGTH);
					}
				}
			}
		}
		break;
	}
	default: {
		break;
	}
	}
}

void Engine::Render() {
	SDL_SetRenderDrawColor(renderer, 16, 165, 245, 255);
	SDL_RenderClear(renderer);
	grid->DrawGraph();
	for (auto curMapSetUpButton : mapSetUpButtons) {
		curMapSetUpButton->Draw();
	}
	for (auto curTag : tags) {
		curTag->Draw();
	}
	for (auto curAlgoButton : algoSetUpButtons) {
		curAlgoButton->Draw();
	}
	for (auto currMazeButton : mazeGenButtons) {
		currMazeButton->Draw();
	}
	for (auto currMoveMentButton : movementModeButtons) {
		currMoveMentButton->Draw();
	}
	solveButton->Draw();
	clearGridButton->Draw();
	SDL_RenderPresent(renderer);
}

void Engine::LoadButtons() {
	mapSetUpButtons.emplace_back(new Button("./Assets/StartPoint.png", 645, 10, START_BUTTON, false));
	mapSetUpButtons.emplace_back(new Button("./Assets/EndPoint.png", 645, 60, END_BUTTON, false));
	mapSetUpButtons.emplace_back(new Button("./Assets/Walls.png", 645, 110, WALL_BUTTON, false));
	mapSetUpButtons.emplace_back(new Button("./Assets/DeleteWalls.png", 645, 160, DELETEWALLS_BUTTON, false));
	tags.emplace_back(new Button("./Assets/Algorithms_Tag.png", 645, 210, TAG, false));
	algoSetUpButtons.emplace_back(new Button("./Assets/AStar.png", 645, 260, ASTAR, false));
	algoSetUpButtons.emplace_back(new Button("./Assets/Dijkstra.png", 645, 310, DIJKSTRA, false));
	algoSetUpButtons.emplace_back(new Button("./Assets/GreedyBFS.png", 645, 360, GREEDYBFS, false));
	algoSetUpButtons.emplace_back(new Button("./Assets/DFS.png", 645, 410, DFS, false));
	algoSetUpButtons.emplace_back(new Button("./Assets/BreadthFS.png", 645, 460, BFS, false));
	tags.emplace_back(new Button("./Assets/AutoMaze.png", 645, 510, TAG, true));
	mazeGenButtons.emplace_back(new Button("./Assets/RBackTrack.png", 645, 560, RBACKTRACK, false));
	mazeGenButtons.emplace_back(new Button("./Assets/RDivision.png", 645, 610, RDIVISION, false));
	mazeGenButtons.emplace_back(new Button("./Assets/Options.png", 65, 630, TAG, false));
	movementModeButtons.emplace_back(new Button("./Assets/AllowDiagnoal.png", 235, 630, ALLOWDIAGNOAL, true));
	movementModeButtons.emplace_back(new Button("./Assets/AllowCrossCorner.png", 405, 630, ALLOWCROSSCORNER, true));
	solveButton = new Button("./Assets/Solve.png", 235, 680, SOLVE, true);
	clearGridButton = new Button("./Assets/ClearGrid.png", 405, 680, CLEAR, true);
}

void Engine::DestroyButtons() {
	for (auto curMapSetUpButton : mapSetUpButtons) {
		delete curMapSetUpButton;
	}
	for (auto curTag : tags) {
		delete curTag;
	}
	for (auto curAlgoButton : algoSetUpButtons) {
		delete curAlgoButton;
	}
	for (auto currMazeButton : mazeGenButtons) {
		delete currMazeButton;
	}
	for (auto currMoveMentButton : movementModeButtons) {
		delete currMoveMentButton;
	}
	delete solveButton;
	delete clearGridButton;
}

bool Engine::isReadyToSolve(){
	return grid->getStartNode() && grid->getEndNode() && currAlgo;
}
