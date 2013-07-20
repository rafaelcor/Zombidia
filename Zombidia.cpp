#include <SDL/SDL.h>
#include <SDL_image.h>
#include <stdlib.h>
#include <stdio.h>
#include <SDL_rotozoom.h>
int x;
int y;
int main(){
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		fprintf(stderr, "%s\n", SDL_GetError());
		return 1;
	}
	SDL_Surface *window = SDL_SetVideoMode(500, 500, 32, SDL_HWSURFACE|SDL_DOUBLEBUF);
	SDL_WM_SetCaption("Zombidia", "Zombidia");
	
	SDL_Surface *imagen = IMG_Load("personaje.png"); 
	SDL_Surface *modimage = zoomSurface(imagen, 2, 2, SMOOTHING_OFF);
	SDL_Surface *fondo = IMG_Load("fondo.png");
	SDL_Rect fpos;
	fpos.x = 0;
	fpos.y = 0;
	SDL_Rect pos;
	pos.x = 0;
	pos.y = 300;
	
	//SDL_FillRect(window, NULL, 0xFFFFFF);
	SDL_BlitSurface(fondo, NULL, window, &fpos);
	SDL_BlitSurface(imagen, NULL, window, &pos);
	SDL_Flip(window);

	
	atexit(SDL_Quit);
	SDL_Event event;
	int velx = pos.x, vely = pos.y;
	loop:
		SDL_Delay(1000/30);
		SDL_PumpEvents();
		SDL_BlitSurface(fondo, NULL, window, &fpos);
		SDL_BlitSurface(imagen, NULL, window, &pos);
		SDL_Flip(window);
		Uint8 *keystate = SDL_GetKeyState(NULL);
		
		if(keystate[SDLK_LEFT]){
			velx = -10;
			imagen = IMG_Load("personaje_left.png"); 
			}
		else if(keystate[SDLK_RIGHT]){
			velx = 10; 
			imagen = IMG_Load("personaje.png"); 
			}
		else{
			velx = 0; 
			}
		if(keystate[SDLK_UP]){
			vely = -10;
			imagen = IMG_Load("personaje_back.png");  
			}
		else if(keystate[SDLK_DOWN]){
			vely = 10;
			imagen = IMG_Load("personaje_front.png"); 
			}
		else if(keystate[SDLK_ESCAPE]){
			printf("escape");
			SDL_Quit();
			}
			
		else{
			vely = 0;
			}
		pos.x += velx;
		pos.y += vely;
		SDL_Flip(window);
		while(SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT){
				SDL_FreeSurface(imagen);
				SDL_FreeSurface(modimage);
				SDL_FreeSurface(fondo);
				return 0;
			}
			if( event.type == SDL_MOUSEBUTTONDOWN ) {
				if( event.button.button == SDL_BUTTON_LEFT )
				{ 
					x = event.button.x; 
					y = event.button.y;
					printf("x: %d\n", x);
					printf("y: %d\n", y);
					printf("xper: %d\n", pos.x);
					printf("yper: %d\n", pos.y);
				}
			}
		}
		if (pos.x >= 150){
		//fpos.x = 500;
		//SDL_BlitSurface(fondo, NULL, window, &fpos);
		//SDL_BlitSurface(imagen, NULL, window, &pos);
		//SDL_Flip(window);
		}
	goto loop;
	return 0;
}

