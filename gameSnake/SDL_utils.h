#ifndef SDL_UTILS__H
#define SDL_UTILS__H

#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
using namespace std;
void initSDL(SDL_Window* &window, SDL_Renderer* &renderer,
             int SCREEN_WIDTH, int SCREEN_HEIGHT, const string &WINDOW_TITLE);
void logSDLError(std::ostream& os,
                 const std::string &msg, bool fatal = false);

void quitSDL(SDL_Window* window, SDL_Renderer* renderer);
void waitUntilKeyPressed();
SDL_Texture* loadTexture (string path, SDL_Renderer* renderer);
void renderImage(string path, SDL_Renderer* renderer, int dx, int dy, double direction);
void renderVectorImage(string path, SDL_Renderer* renderer, int dx, int dy, bool up, bool down, bool right, bool left);
void renderFont(SDL_Renderer* renderer, SDL_Color color, string path_font, int ptsize, string dest, int dw, int dh, int dx, int dy);
#endif // SDL_UTILS_H

