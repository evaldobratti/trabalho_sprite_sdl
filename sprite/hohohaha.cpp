#include <SDL.h>
#include <SDL_image.h>
#include <SDL_main.h>
#include <stdio.h>
#include <SDL_mixer.h>

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const int VELOCITY = 7;

bool init();
bool loadMedia();
void close();


int spriteParadoX[] = { 269, 311, 354, 396, 438, 396, 354, 311, 269 };
int spriteParadoY[] = { 8,   8,   8,   8  , 8,   8,   8,   8,   8};
int spriteParadoW[] = { 37 , 37 , 37,  37 , 37,  37,  37,  37,  37};
int spriteParadoH[] = { 49 , 49 , 49,  49 , 49,  49,  49,  49,  49};
int lenghtParado = sizeof(spriteParadoH) / sizeof(int);

int spriteCaminhandoX[] = { 230, 270, 315, 365, 422, 468, 144, 182, 226, 276, 330, 382, 431, 470 };
int spriteCaminhandoY[] = { 342, 342, 342, 342, 342, 342, 400, 400, 400, 400, 400, 400, 400, 400 };
int spriteCaminhandoW[] = { 34,  42,  48,  50,  42,  32,  32,  37,  42,  44,  44,  40,  34,  31 };
int spriteCaminhandoH[] = { 49,  49,  49,  49,  49,  49,  49,  49,  49,  49,  49,  49,  49,  49 };
int lenghtCaminhando = sizeof(spriteCaminhandoH) / sizeof(int);

int *currentSpriteX = spriteParadoX;
int *currentSpriteY = spriteParadoY;
int *currentSpriteW = spriteParadoW;
int *currentSpriteH = spriteParadoH;
int currentLength = lenghtParado;

int bodyW = 35;
int bodyH = 49;
int bodyX = SCREEN_WIDTH / 2 - bodyH;
int bodyY = SCREEN_HEIGHT / 2 - bodyW;

SDL_Window* window = NULL;
SDL_Surface* mainSurface = NULL;
SDL_Surface* buffer = NULL;

SDL_Surface* megamanSprite = NULL;
SDL_Surface* fundoBranco = NULL;

Mix_Chunk *wind = NULL;

bool init() {
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		printf("Erro ao inicializar o vídeo! %s", SDL_GetError());
		return false;
	}
	else {
		window = SDL_CreateWindow("SQL", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
		if (window == NULL) {
			printf("Interface gráfica não pode ser criada! %s", SDL_GetError());
			return false;
		}
		else {
			mainSurface = SDL_GetWindowSurface(window);
		}
	}
	
	
	if (Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 2048) < 0) {
		printf("iniciou %s", Mix_GetError());
		return false;
	}
	return true;

}

bool loadMedia() {
	SDL_Surface* spriteSurface = IMG_Load("C:\\pos-jogos\\sprite\\sprite\\sprite\\x6shadow.gif");
	megamanSprite = SDL_ConvertSurfaceFormat(spriteSurface, SDL_PIXELFORMAT_RGBA8888, 0);
	SDL_FreeSurface(spriteSurface);

	spriteSurface = IMG_Load("C:\\pos-jogos\\sprite\\sprite\\sprite\\branco.gif");
	fundoBranco = SDL_ConvertSurfaceFormat(spriteSurface, SDL_PIXELFORMAT_RGBA8888, 0);
	SDL_FreeSurface(spriteSurface);
	
	wind = Mix_LoadWAV("C:/pos-jogos/sprite/sprite/sprite/footsteps.wav");
	if (wind == NULL) {
		printf("erro abrindo arquivo %s", Mix_GetError());
	}
	return true;
}


void close() {
	SDL_FreeSurface(mainSurface);
	SDL_DestroyWindow(window);
	window = NULL;
	SDL_Quit();
}

int main(int argc, char* args[])
{
	if (!init()) {
		printf("falha ao inicializar sdl");
	}
	else {
		if (!loadMedia()) {
			printf("erro ao carregar imagem");
		}
		else {
			bool quit = false;
			int currentSprite = 0;
			SDL_Event e;
			while (!quit) {
				while (SDL_PollEvent(&e) != 0) {
					if (e.type == SDL_QUIT) {
						quit = true;
					}
					if (e.type == SDL_KEYDOWN) {
						if (e.key.keysym.sym == SDLK_ESCAPE)
							quit = true;
						
						currentSpriteX = spriteCaminhandoX;
						currentSpriteY = spriteCaminhandoY;
						currentSpriteW = spriteCaminhandoW;
						currentSpriteH = spriteCaminhandoH;
						currentLength = lenghtCaminhando;
						
						if (e.key.keysym.sym == SDLK_RIGHT)
							bodyX = bodyX + VELOCITY;
						else if (e.key.keysym.sym == SDLK_LEFT)
							bodyX = bodyX - VELOCITY;

						
						Mix_PlayChannel(-1, wind, 0);
						
					} 
					if (e.type == SDL_KEYUP) {
						currentSpriteX = spriteParadoX;
						currentSpriteY = spriteParadoY;
						currentSpriteW = spriteParadoW;
						currentSpriteH = spriteParadoH;
						currentLength = lenghtParado;
						currentSprite = 0;
					}
				}
				

				SDL_Rect source;
				source.x = currentSpriteX[currentSprite];
				source.y = currentSpriteY[currentSprite];
				source.w = currentSpriteW[currentSprite];
				source.h = currentSpriteH[currentSprite];

				SDL_Rect destiny;
				destiny.x = bodyX + bodyH - source.h;
				destiny.y = bodyY + (bodyW - source.w) / 2;

				currentSprite = currentSprite + 1;
				if (currentSprite >= currentLength)
					currentSprite = 0;

				SDL_BlitSurface(fundoBranco, NULL, mainSurface, NULL);
				SDL_BlitSurface(megamanSprite, &source, mainSurface, &destiny);

				SDL_UpdateWindowSurface(window);
				SDL_Delay(100);
			}

		}
		close();
		return 0;
	}
}