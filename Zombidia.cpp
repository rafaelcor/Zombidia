#include <SDL/SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <stdlib.h>
#include <stdio.h>

SDL_Surface *window;
int x;
int y;
Uint8 state; //0-Pueblo 1-Fuera
bool change;
int mouse[2]; //0-X 1-Y 2-Left Button 3-Right Button
bool mbutton;
TTF_Font *butfont;

bool events(){
 SDL_Event event;
	SDL_PumpEvents();
	mbutton = false;
	while(SDL_PollEvent(&event)){
		if (event.type == SDL_QUIT){
			return true;
		}
		if(event.type == SDL_MOUSEBUTTONDOWN){
			if(event.button.button == SDL_BUTTON_LEFT){ 
				mbutton = true;
			}
		}
	}
	SDL_GetMouseState(&mouse[0], &mouse[1]);
	return false;
}

class Button{
	public:
		void (*func)();
		SDL_Rect ubc;
		bool focus;
		SDL_Surface *surface, *rect;
		void set(void (*callfunc)(), unsigned short nx, unsigned short ny, const char *ntext){
			func = callfunc;
			ubc.x = nx;
			ubc.y = ny;
			SDL_Color color = {240, 215, 158};
			SDL_Surface *text = TTF_RenderUTF8_Solid(butfont, ntext, color);
			surface = SDL_CreateRGBSurface(SDL_SWSURFACE, text->w, text->h, 32, 16711680, 65280, 255, 0);
			rect = SDL_CreateRGBSurface(SDL_SWSURFACE, text->w+2, text->h+2, 32, 16711680, 65280, 255, 0);
			SDL_FillRect(surface, NULL, 0xA43019);
			SDL_BlitSurface(text, NULL, surface, NULL);
			SDL_FillRect(rect, NULL, 0);
			SDL_Rect subc;
			subc.x = 1;
			subc.y = 1;
			SDL_BlitSurface(surface, NULL, rect, &subc);
			focus = false;
			SDL_FreeSurface(text);
		}
		void update(){
			if (mouse[0] >= ubc.x and mouse[0] < ubc.x+surface->w and mouse[1] >= ubc.y and mouse[1] < ubc.y+surface->h){
				if(not focus){
					SDL_FillRect(rect, NULL, 0xF0D79E);
					SDL_Rect subc;
					subc.x = 1;
					subc.y = 1;
					SDL_BlitSurface(surface, NULL, rect, &subc);
					focus = true;
				}
				if (mbutton){
					func();
				}
			}
			else if(focus){
				SDL_FillRect(rect, NULL, 0);
				SDL_Rect subc;
				subc.x = 1;
				subc.y = 1;
				SDL_BlitSurface(surface, NULL, rect, &subc);
				focus = false;
			}
		}
		void blit(){
			SDL_BlitSurface(rect, NULL, window, &ubc);
		}
};

void pfunc(){
	printf("Click\n");
}

void interface(bool exit){
	static Button *buttons = NULL;
 static Uint8 num = 0;
	if (change and not exit){
		if (buttons != NULL){
			delete [] buttons;
		}
		num = 2;
		buttons = new Button[2];
		buttons[0].set(pfunc, 0, 0, "Test");
		buttons[1].set(pfunc, 0, 100, "Test2");
		/*switch(state){
			case 0:
			case 1:
		}*/
	}
	if (not exit){
		for(Uint8 i=0 ; i<num ; i++){
			buttons[i].update();
		 buttons[i].blit();
		}
	}
	else{
		delete [] buttons;
	}
}

void exit(){
	interface(true);
	TTF_CloseFont(butfont);
	SDL_Quit();
	TTF_Quit();
}

int main(){
	if (SDL_Init(SDL_INIT_VIDEO) < 0){
		fprintf(stderr, "%s\n", SDL_GetError());
		return 1;
	}
	window = SDL_SetVideoMode(500, 300, 32, SDL_HWSURFACE|SDL_DOUBLEBUF);
	SDL_WM_SetCaption("Zombidia", "Zombidia");
	TTF_Init();
	butfont = TTF_OpenFont("FreeSans.ttf", 15);
	
	SDL_Surface *imagen = IMG_Load("personaje.png");
	SDL_Surface *fondo = IMG_Load("fondo.png");
	SDL_Surface *piso = IMG_Load("piso.png");
	SDL_Rect ppos;
	SDL_Rect fpos;
	fpos.x = 0;
	fpos.y = 0;
	fpos.w = 500;
	fpos.h = 300;
	SDL_Rect pos;
	pos.x = 0;
	pos.y = 100;
	state = 1;
	change = true;
	
	//SDL_FillRect(window, NULL, 0xFFFFFF);
	SDL_BlitSurface(fondo, NULL, window, &fpos);
	SDL_BlitSurface(imagen, NULL, window, &pos);
	SDL_Flip(window);
	
	atexit(exit);
	int velx = pos.x, vely = pos.y;
	loop:
		SDL_Delay(1000/30);
		if(events()){
			SDL_FreeSurface(fondo);
			SDL_FreeSurface(imagen);
			SDL_FreeSurface(piso);
			return 0;
		}
		SDL_BlitSurface(fondo, &fpos, window, NULL);
		SDL_BlitSurface(imagen, NULL, window, &pos);
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
		//SDL_Flip(window);
		if (pos.x >= 499){
			fpos.x = 500;
			pos.x = 24;
			state = 0;
			SDL_BlitSurface(fondo, &fpos, window, NULL);
			SDL_BlitSurface(imagen, NULL, window, &pos);
			//ppos.x = 0;
			//ppos.y = 100;
			//SDL_BlitSurface(piso, NULL, window, &ppos);
			//SDL_Flip(window);
		}
		if (state == 0)
		{
			ppos.x = 0;
			ppos.y = 100;
			SDL_BlitSurface(piso, NULL, window, &ppos);
			//SDL_Flip(window);
		}
		interface(false);
		SDL_Flip(window);
		change = false;
	goto loop;
	return 0;
}

