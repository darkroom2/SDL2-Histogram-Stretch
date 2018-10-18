#include <SDL.h>

extern "C" void function(void *pPixelBuffer, int width, int height);

class Display {
private:
	SDL_Surface *surf;
	SDL_Window *window;
	SDL_Renderer *rend;

	void histogramStretch() {
		int min = 255; // to check whos commiting wtf
		int max = 0;
		int lut[256] = {};

		auto bpp = surf->format->BytesPerPixel;
		auto pitch = surf->pitch;
		auto pixelsBegin = surf->pixels;
		auto width = surf->w;
		auto height = surf->h;

		for (int i = 0; i < height; ++i)
			for (int j = 0; j < width; ++j) {
				uint8_t *p = (uint8_t *)pixelsBegin + i * pitch + j * bpp;
				if (*p > max)
					max = *p;
				if (*p < min)
					min = *p;
			}

		for (int i = 0; i < height; ++i)
			for (int j = 0; j < width; ++j) {
				uint8_t *p = (uint8_t *)pixelsBegin + i * pitch + j * bpp;
				lut[*p] = (int)round(255.0 / (max - min) * (*p - min));
			}

		for (int i = 0; i < height; ++i)
			for (int j = 0; j < width; ++j) {
				uint8_t *p = (uint8_t *)pixelsBegin + i * pitch + j * bpp;
				*p = lut[*p];
			}
	}
	// this function only colors the picture in (55,55,55) or simply 55 as its 8bpp image
	// WIP
	void asmHistStretch() {
		auto pixelsBegin = surf->pixels; // rm this after figuring out how to find variables on stack :)
		auto width = surf->w; //
		auto height = surf->h; //
		function(pixelsBegin, width, height);
	}

	void drawSideBySide(SDL_Texture *textureBefore, SDL_Texture *textureAfter, int width, int height) {
		//W and H of the picture, to know where to render them.
		SDL_Rect rectForAftTxt; //rect for textureAfter
		rectForAftTxt.x = width;
		rectForAftTxt.y = 0;
		rectForAftTxt.w = width;
		rectForAftTxt.h = height;

		//SDL_SetRenderDrawColor(rend, 255, 0, 0, SDL_ALPHA_OPAQUE);
		//SDL_RenderClear(rend);

		SDL_RenderCopy(rend, textureBefore, NULL, &surf->clip_rect);
		SDL_RenderCopy(rend, textureAfter, NULL, &rectForAftTxt);

		SDL_RenderPresent(rend);

		SDL_DestroyTexture(textureAfter);
		SDL_DestroyTexture(textureBefore);
	}

public:
	Display(const char *path) {
		
		SDL_Init(SDL_INIT_VIDEO);
		
		surf = SDL_LoadBMP(path); //lena512.bmp
		
		window = SDL_CreateWindow(
			"SDL Test",
			SDL_WINDOWPOS_CENTERED,
			SDL_WINDOWPOS_CENTERED,
			surf->w * 2,
			surf->h,
			SDL_WINDOW_OPENGL
		);

		rend = SDL_CreateRenderer(
			window,
			-1,
			SDL_RENDERER_ACCELERATED |
			SDL_RENDERER_PRESENTVSYNC
		);
	}

	void cleanUp() {
		SDL_DestroyRenderer(rend);
		SDL_DestroyWindow(window);
		SDL_FreeSurface(surf);
		SDL_Quit();
	}

	void showStretchedHistogram() { // works for 8bpp bmp's.
		
		SDL_Texture *textureBefore = SDL_CreateTextureFromSurface(rend, surf);

		//asmHistStretch(); //by asm, or by c++
		histogramStretch();

		SDL_Texture *textureAfter = SDL_CreateTextureFromSurface(rend, surf);

		drawSideBySide(textureBefore, textureAfter, surf->w, surf->h);
	}
};

int main(int argc, char* argv[]) {

	Display disp("lena512.bmp");

	disp.showStretchedHistogram();

	SDL_Event event;
	bool running = true;
	while (running) {

		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT) {
				running = false;
				break;
			}


			if (event.key.keysym.scancode == SDL_SCANCODE_ESCAPE) {
				running = false;
				break;
			}
		}
	}
	
	disp.cleanUp();

	return 0;
}