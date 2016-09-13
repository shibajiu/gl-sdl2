// sdl2test.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"


int main(int argc,char* argv[])
{
	
	SDLGL s(800,600);
	s.Init_sdl();
	system("pause");
	SDL_Quit();
    return 0;
}

