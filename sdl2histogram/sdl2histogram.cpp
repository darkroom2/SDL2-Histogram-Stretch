#include <iostream>
#include <SDL.h>

extern "C" void function(void *pPixelBuffer, int width, int height);

int main(int argc, char* argv[]) {

	SDL_Init(SDL_INIT_VIDEO);

	SDL_Surface *surf = SDL_LoadBMP("lena512.bmp");

	SDL_Window *window = SDL_CreateWindow(
		"SDL Test",
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		surf->w * 2,
		surf->h,
		SDL_WINDOW_OPENGL
	);

	SDL_Renderer *rend = SDL_CreateRenderer(
		window,
		-1,
		SDL_RENDERER_ACCELERATED |
		SDL_RENDERER_PRESENTVSYNC
	);

	SDL_Texture *textureBefore = SDL_CreateTextureFromSurface(rend, surf);

	int min = 255;
	int max = 0;

	int lut[256] = {};

	auto bpp = surf->format->BytesPerPixel;
	auto pitch = surf->pitch;
	auto pixelsBegin = surf->pixels;
	auto width = surf->w;
	auto height = surf->h;

	function(pixelsBegin, width, height);

	//for (int i = 0; i < height; ++i)
	//	for (int j = 0; j < width; ++j) {
	//		uint8_t *p = (uint8_t *)pixelsBegin + i * pitch + j * bpp;
	//		if (*p > max)
	//			max = *p;
	//		if (*p < min)
	//			min = *p;
	//	}

	//for (int i = 0; i < height; ++i)
	//	for (int j = 0; j < width; ++j) {
	//		uint8_t *p = (uint8_t *)pixelsBegin + i * pitch + j * bpp;
	//		lut[*p] = (int)round(255.0 / (max - min) * (*p - min));
	//	}

	//for (int i = 0; i < height; ++i)
	//	for (int j = 0; j < width; ++j) {
	//		uint8_t *p = (uint8_t *)pixelsBegin + i * pitch + j * bpp;
	//		*p = lut[*p];
	//	}

	SDL_Texture *textureAfter = SDL_CreateTextureFromSurface(rend, surf);

	SDL_Rect rectForAftTxt; //rect for textureAfter
	rectForAftTxt.x = width;
	rectForAftTxt.y = 0;
	rectForAftTxt.w = width;
	rectForAftTxt.h = height;

	SDL_SetRenderDrawColor(rend, 255, 0, 0, SDL_ALPHA_OPAQUE);
	SDL_RenderClear(rend);

	SDL_RenderCopy(rend, textureBefore, NULL, &surf->clip_rect);
	SDL_RenderCopy(rend, textureAfter, NULL, &rectForAftTxt);

	SDL_RenderPresent(rend);

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

	// clean up
	SDL_DestroyTexture(textureAfter);
	SDL_DestroyTexture(textureBefore);
	SDL_DestroyRenderer(rend);
	SDL_DestroyWindow(window);
	SDL_FreeSurface(surf);
	SDL_Quit();

	return 0;
}