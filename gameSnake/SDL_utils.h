#ifndef SDL_UTILS__H
#define SDL_UTILS__H

#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
using namespace std;
void initSDL(SDL_Window* &window, SDL_Renderer* &renderer,
             int SCREEN_WIDTH, int SCREEN_HEIGHT, const string &WINDOW_TITLE);
void logSDLError(std::ostream& os,
                 const std::string &msg, bool fatal = false);

void quitSDL(SDL_Window* window, SDL_Renderer* renderer);
void waitUntilKeyPressed();
SDL_Texture* loadTexture (string path, SDL_Renderer* renderer);

#endif // SDL_UTILS_H

