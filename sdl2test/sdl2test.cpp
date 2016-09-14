// sdl2test.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

int main(int argc,char* argv[])
{
	SDLGL s(1200,800);
	SDL_Event *e=new SDL_Event();
	while (1) {
		while (SDL_PollEvent(e)) {
			switch (e->type) {
			case SDL_MOUSEBUTTONDOWN:
				cout << "click" << endl;	
				break;
			case SDL_MOUSEBUTTONUP:
				if (e->button.state == SDL_PRESSED)
					cout << "r";
				break;
			case SDL_QUIT:
				goto done;
			default:
				break;
			}
		}

	}
done:
	delete e;
	//system("pause");
	SDL_Quit();
    return 0;
}