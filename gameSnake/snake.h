#ifndef SNAKE__H
#define SNAKE__H

#include <iostream>
#include <vector>
#include <string>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include "SDL_utils.h"
using namespace std;
const int unit = 24;
SDL_Rect make_obstacle();
struct Snake {
    int tailLength;
    int Score;
    int highScore;
    int highScore_prev;
    vector<SDL_Rect> all_obs;
    vector<int> tailX;
    vector<int> tailY;
    vector<vector<pair<int,int>>> all_pos_obs;

    void renderObstacle(SDL_Renderer* renderer);
    void renderPlayer(SDL_Renderer* renderer, SDL_Rect player, int x, int y, bool up, bool down, bool right, bool left);
    void renderScore(SDL_Renderer* renderer);
    void renderFood(SDL_Renderer* renderer, SDL_Rect food, int &n);
    bool checkEatFood(int foodx, int foody, int playerx, int playery);
    pair<int, int> makeFood(int playerX, int playerY);
    bool gameOver(int x, int y);
    void renderGameOver(SDL_Renderer* renderer, SDL_Event event);
    void make_all_pos ();
    void afterGameOver(int x, int y, pair<int, int> food_pos, SDL_Rect food);
    void updateTail(int &prevX, int &prevY);

};


#endif // SNAKE__H
