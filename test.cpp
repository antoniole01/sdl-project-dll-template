#include <windows.h>
#include <iostream>
#include <string>
#include <fstream>
#include <stdio.h>
#include <stdio.h>
#include <sstream>
#include <vector>

//#include <stdlib.h>

#include "main.h"
#include "test.h"

#include "pch.h"

#pragma warning(disable:4996)

bool FF = 0;

int width;
int height;
int fontscale;

static bool running_within;
static int do_once = 0;

extern "C" GAME_UPDATE_AND_RENDER(GameUpdateAndRender)
{
	
	running_within = true;

	char title[100] = "Title"; // set title of window
	
	int x_pos = 450;
	int y_pos = 225;
	
	if(!FF)
	{
		width = 800;
		height = 600;
		fontscale = 40;
		SDL_SetWindowSize(window,width,height);
		SDL_SetWindowPosition(window,x_pos,y_pos);
		//SDL_SetWindowFullscreen(window,0);//SDL_WINDOW_FULLSCREEN, SDL_WINDOW_FULLSCREEN_DESKTOP or 0
	}
	else
	{
		width = 1920;
		height = 1080;
		fontscale = 40*2;
		SDL_SetWindowSize(window,width,height);
		SDL_SetWindowPosition(window,0,0);
		//SDL_SetWindowFullscreen(window,SDL_WINDOW_FULLSCREEN_DESKTOP);
	}

	
	if(do_once == 0)
	{
		destroy_Window();
		init_Window(title,0,x_pos,y_pos, width,height, 0,0); //window mode
	}
	do_once = 1;
	
	
	while(POLLEVENT)
	{
		if(QUIT){exit(0);}
		if(DOWNPRESS)
		{
			if(ESCAPE)  {}
			if(F12_KEY){FF=!FF;}
		}
		if(UPPRESS){}
	}

	// sdl
	SDL_SetRenderDrawColor(renderer,0,64,64,255);
	SDL_RenderClear(renderer);

	
	// sdl
	SDL_Delay(1000/33);
	SDL_RenderPresent(renderer);
}
