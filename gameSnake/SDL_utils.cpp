#include <iostream>
#include "SDL_utils.h"
using namespace std;
void initSDL(SDL_Window* &window, SDL_Renderer* &renderer,
             int SCREEN_WIDTH, int SCREEN_HEIGHT, const string &WINDOW_TITLE)
{
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
        logSDLError(std::cout, "SDL_Init", true);

    window = SDL_CreateWindow(WINDOW_TITLE.c_str(), SDL_WINDOWPOS_CENTERED,
       SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (window == nullptr) logSDLError(std::cout, "CreateWindow", true);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED |
                                              SDL_RENDERER_PRESENTVSYNC);
    if (renderer == nullptr) logSDLError(std::cout, "CreateRenderer", true);

    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
    SDL_RenderSetLogicalSize(renderer, SCREEN_WIDTH, SCREEN_HEIGHT);
}

void logSDLError(std::ostream& os,
                 const std::string &msg, bool fatal)
{
    os << msg << " Error: " << SDL_GetError() << std::endl;
    if (fatal) {
        SDL_Quit();
        exit(1);
    }
}

void quitSDL(SDL_Window* window, SDL_Renderer* renderer)
{
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}

void waitUntilKeyPressed()
{
    SDL_Event e;
    while (true) {
        if ( SDL_WaitEvent(&e) != 0 &&
             (e.type == SDL_KEYDOWN || e.type == SDL_QUIT) )
            return;
        SDL_Delay(100);
    }
}

SDL_Texture* loadTexture (string path, SDL_Renderer* renderer) {
    SDL_Texture* newTexture = nullptr;
    SDL_Surface* loadedSurface = IMG_Load (path.c_str());
    if (loadedSurface == nullptr) {
        cout << "Unable to load image " << path << "SDL_image Error: " << IMG_GetError() << endl;
    } else {
        newTexture = SDL_CreateTextureFromSurface(renderer, loadedSurface);
        if (newTexture == nullptr)
            cout << "Unable to create texture from " << path << " SDL Error: " << SDL_GetError() << endl;
        SDL_FreeSurface(loadedSurface);
    }
    return newTexture;
}

void renderImage(string path, SDL_Renderer* renderer, int dx, int dy, double direction) {
    SDL_Texture* newTexture = nullptr;
    SDL_Surface* loadedSurface = IMG_Load (path.c_str());
    if (loadedSurface == nullptr) {
        cout << "Unable to load image " << path << "SDL_image Error: " << IMG_GetError() << endl;
    } else {
        newTexture = SDL_CreateTextureFromSurface(renderer, loadedSurface);
        if (newTexture == nullptr)
            cout << "Unable to create texture from " << path << " SDL Error: " << SDL_GetError() << endl;
        SDL_FreeSurface(loadedSurface);
    }
    SDL_Rect dest;
    dest.w = 24;
    dest.h = 24;
    dest.x = dx;
    dest.y = dy;
    SDL_RenderCopyEx(renderer, newTexture, NULL, &dest, direction, nullptr, SDL_FLIP_NONE);
    SDL_DestroyTexture(newTexture);
}

void renderVectorImage(string path, SDL_Renderer* renderer, int dx, int dy, bool up, bool down, bool right, bool left) {
    SDL_Texture* newTexture = nullptr;
    SDL_Surface* loadedSurface = IMG_Load (path.c_str());
    if (loadedSurface == nullptr) {
        cout << "Unable to load image " << path << "SDL_image Error: " << IMG_GetError() << endl;
    } else {
        newTexture = SDL_CreateTextureFromSurface(renderer, loadedSurface);
        if (newTexture == nullptr)
            cout << "Unable to create texture from " << path << " SDL Error: " << SDL_GetError() << endl;
        SDL_FreeSurface(loadedSurface);
    }
    SDL_Rect dest;
    dest.w = 24;
    dest.h = 24;
    dest.x = dx;
    dest.y = dy;
    double direction = 0;
    if (up) direction = 270;
    else if (down) direction = 90;
    else if (right) direction = 0;
    else if (left) direction = 180;
    SDL_RenderCopyEx(renderer, newTexture, NULL, &dest, direction, nullptr, SDL_FLIP_NONE);
    SDL_DestroyTexture(newTexture);
}

void renderFont(SDL_Renderer* renderer, SDL_Color color, string path_font, int ptsize, string dest, int dw, int dh, int dx, int dy) {
    TTF_Font* font = TTF_OpenFont(path_font.c_str(), ptsize);
    if (font == NULL) {
        cout << "Font loading error" << endl;
        return;
    }
    SDL_Surface* score = TTF_RenderText_Solid(font, dest.c_str(), color);
    SDL_Texture* destTexture = SDL_CreateTextureFromSurface(renderer, score);
    SDL_FreeSurface(score);
    SDL_Rect destRect;
    destRect.w = dw;
    destRect.h = dh;
    destRect.x = dx;
    destRect.y = dy;
    SDL_RenderCopy(renderer, destTexture, NULL, &destRect);
    TTF_CloseFont(font);
}
