#include <SDL.h>
#include <SDL_image.h>
#include <SDL_main.h>
#include <stdio.h>
#include <SDL_mixer.h>
#include <vector>
using namespace std;
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const int VELOCITY = 8;

bool init();
bool loadMedia();
void close();

class Frame {
public:
	int getX(){
		return this->x;
	}
	int getY() {
		return this->y;
	}
	int getW() {
		return this->w;
	}
	int getH() {
		return this->h;
	}
	Frame(int x, int y, int w, int h){
		this->x = x;
		this->y = y;
		this->w = w;
		this->h = h;
	}
private:
	int x, y, w, h;
};


class FrameSet {
public:
	FrameSet() {
		i = 0;
		nextFrameSet = NULL;
	}
	FrameSet(FrameSet *nextFrameSet) {
		vector<Frame> temp;
		this->frames = temp;
		this->nextFrameSet = nextFrameSet;
	}
	vector<Frame> getFrames(){
		return this->frames;
	}
	void addFrame(Frame frame) {
		this->frames.push_back(frame);
	}
	Frame getFrameAtual() {
		i += 1;
		if (i >= this->frames.size())
			i = 0;
		
		return frames.at(i);
	}
	void setNextFrameSet(FrameSet* nextFrameSet) {
		this->nextFrameSet = nextFrameSet;
	}
	FrameSet& getNextFrameSet() {
		printf("%s", nextFrameSet);
		if (nextFrameSet != NULL){
			return *nextFrameSet;
		}
		return *this;
	}
	void reset() {
		i = 0;
	}
private:
	int i;
	vector<Frame> frames;
	FrameSet *nextFrameSet;
	Frame *frameAtual;

	bool isUltimoFrame(int i) {
		return i >= frames.size();
	}
};

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

FrameSet *parado = new FrameSet();
FrameSet *caminhando = new FrameSet();
FrameSet *iniciandoCaminhada = new FrameSet();
FrameSet *atual = parado;

bool init() {
	
	parado->addFrame(Frame(269, 8, 37, 49));
	parado->addFrame(Frame(311, 8, 37, 49));
	parado->addFrame(Frame(354, 8, 37, 49));
	parado->addFrame(Frame(396, 8, 37, 49));
	parado->addFrame(Frame(438, 8, 37, 49));
	parado->addFrame(Frame(396, 8, 37, 49));
	parado->addFrame(Frame(354, 8, 37, 49));
	parado->addFrame(Frame(311, 8, 37, 49));
	parado->addFrame(Frame(269, 8, 37, 49));

	
	caminhando->addFrame(Frame(230, 342, 34, 49));
	caminhando->addFrame(Frame(270, 342, 42, 49));
	caminhando->addFrame(Frame(315, 342, 48, 49));
	caminhando->addFrame(Frame(365, 342, 50, 49));
	caminhando->addFrame(Frame(422, 342, 42, 49));
	caminhando->addFrame(Frame(468, 342, 32, 49));
	caminhando->addFrame(Frame(144, 400, 32, 49));
	caminhando->addFrame(Frame(182, 400, 37, 49));
	caminhando->addFrame(Frame(226, 400, 42, 49));
	caminhando->addFrame(Frame(276, 400, 44, 49));
	caminhando->addFrame(Frame(330, 400, 44, 49));
	caminhando->addFrame(Frame(382, 400, 40, 49));
	caminhando->addFrame(Frame(431, 400, 34, 49));
	caminhando->addFrame(Frame(470, 400, 31, 49));

	
	iniciandoCaminhada->addFrame(Frame(145, 342, 35, 49));
	iniciandoCaminhada->addFrame(Frame(189, 342, 33, 49));
	iniciandoCaminhada->setNextFrameSet(caminhando);
	atual = parado;

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
						
						if (atual == parado)
							atual = iniciandoCaminhada;
						
						if (e.key.keysym.sym == SDLK_RIGHT)
							bodyX = bodyX + VELOCITY;
						else if (e.key.keysym.sym == SDLK_LEFT)
							bodyX = bodyX - VELOCITY;

						
						
					} 
					if (e.type == SDL_KEYUP) {
						Mix_PlayChannel(-1, wind, 0);
						atual->reset();
						atual = parado;
					}
				}
				

				Frame frameAtual = atual->getFrameAtual();
				atual = &(atual->getNextFrameSet());
				SDL_Rect source;
				source.x = frameAtual.getX();
				source.y = frameAtual.getY();
				source.w = frameAtual.getW();
				source.h = frameAtual.getH();

				SDL_Rect destiny;
				destiny.x = bodyX + bodyH - source.h;
				destiny.y = bodyY + (bodyW - source.w) / 2;

				SDL_BlitSurface(fundoBranco, NULL, mainSurface, NULL);
				SDL_BlitSurface(megamanSprite, &source, mainSurface, &destiny);

				SDL_UpdateWindowSurface(window);
				SDL_Delay(50);
			}

		}
		close();
		return 0;
	}
}