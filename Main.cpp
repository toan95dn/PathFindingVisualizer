#include<iostream>
#include "src/Engine.h"
#include <SDL.h>

int main(int argc, char* argv[]) {
	Engine *engine = new Engine();
	engine->Init();
	while (engine->IsRunning()) {
		engine->InputProcessing();
		engine->Update();
		engine->Render();
	}
	delete engine;
	return 0;
}