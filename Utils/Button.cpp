#include "Button.h"
#include <Engine.h>
#include <Constants.h>
Button::Button(std::string filePath,int posX, int posY,BUTTON_TYPE buttonType, bool isIndependent):buttonType(buttonType),isIndependent(isIndependent){
	//Load Texture
	SDL_Surface* surface = IMG_Load(filePath.c_str());
	texture = SDL_CreateTextureFromSurface(Engine::renderer, surface);
	SDL_FreeSurface(surface);
	//
	sourceRect.x = 0;
	sourceRect.y = 0;
	sourceRect.w = BUTTON_WIDTH;
	sourceRect.h = BUTTON_HEIGHT;
	destinationRect.x = posX;
	destinationRect.y = posY;
	destinationRect.w = BUTTON_WIDTH;
	destinationRect.h = BUTTON_HEIGHT;
}

Button::~Button(){
	delete texture;
}

bool Button::isClicked(int x,int y){
	//if it's not independent, then turn it off when another button in the group got click, else just return whether it got clicked or not
	if (!isIndependent) {
		if (x > destinationRect.x && x<(destinationRect.x + BUTTON_WIDTH)
			&& y>destinationRect.y && y < (destinationRect.y + BUTTON_HEIGHT)) {
			isOn = true;
			return true;
		}
		else {
			isOn = false;
			return false;
		}
	}
	else {
		return x > destinationRect.x && x<(destinationRect.x + BUTTON_WIDTH)
			&& y>destinationRect.y && y < (destinationRect.y + BUTTON_HEIGHT);
	}
}

void Button::Draw(){
	if (isOn) {
		sourceRect.y = BUTTON_HEIGHT;
	}
	else {
		sourceRect.y = 0;
	}
	SDL_RenderCopyEx(Engine::renderer, texture, &sourceRect, &destinationRect, 0.0, NULL, SDL_FLIP_NONE);
}
