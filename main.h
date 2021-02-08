#ifndef MAIN_H
#define MAIN_H

#include "pch.h"

SDL_Window*		window   = NULL;
SDL_Renderer*	renderer = NULL;

SDL_Texture*	solidTexture_font;
SDL_Texture*	text_font;
SDL_Rect			solidRect_font;
TTF_Font*		gFont = NULL;

SDL_Event e;

LTexture image1[4];




#define TAHOMA "c:/z/cpp/sdl remake/_MI/data/font/Tahoma.ttf"
					
#endif // MAIN_H
