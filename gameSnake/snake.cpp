#include <iostream>
#include "snake.h"
using namespace std;

SDL_Rect make_obstacle() {
    SDL_Rect obstacle;
    obstacle.x = 24*(rand() % 16+2);
    obstacle.y = 24*(rand() % 20+2);
    obstacle.w = 24*(rand() % 4 +1);
    obstacle.h = 24;
    return obstacle;
}

void Snake::renderObstacle(SDL_Renderer* renderer) {
    int s = all_obs.size();
    for (int i = 0; i < s; i++) {
        for (int j = 0; j < all_obs[i].w/unit; j++) {
              renderImage("obs1.png",renderer,all_obs[i].x + unit*j , all_obs[i].y, 0);
        }
    }
}

void Snake::renderPlayer(SDL_Renderer* renderer, SDL_Rect player, int x, int y, bool up, bool down, bool right, bool left) {
    player.w = unit;
    player.h = unit;
    for (int i = 0; i < tailLength; i++) {
        player.x = tailX[i];
        player.y = tailY[i];
        renderVectorImage("ceil1.png", renderer, player.x, player.y, up, down, right, left);

    }
    player.x = x;
    player.y = y;
    renderVectorImage("head1.png", renderer, x, y, up, down, right, left);
}

void Snake::renderFood(SDL_Renderer* renderer, SDL_Rect food, int &n) {
    if (n == 1) renderImage("apple1.png",renderer, food.x, food.y , 0);
    else if (n == 2) renderImage("kiwi.png",renderer, food.x, food.y , 0);
    else if (n == 3) renderImage("orange1.png",renderer, food.x, food.y , 0);
    else if (n == 4) renderImage("cherry2.png",renderer, food.x, food.y , 0);
}

void Snake::renderScore(SDL_Renderer* renderer) {
    SDL_Color Black = {0, 0, 0};
    renderFont(renderer, Black,"COOLVETI.TTF", 20, (string("Score: "))+to_string(Score).c_str(), 150, 48, 213, 0);
}

bool Snake::checkEatFood(int foodx, int foody, int playerx, int playery) {
    if (playerx == foodx && playery == foody){
        return true;
    }
    return false;
}

pair<int, int> Snake::makeFood(int playerX, int playerY) {
    bool valid = false;
    int x = 0;
    int y = 0;
    while (valid == false) {
        x = unit * ((rand() % 22)+1);
        y = unit * ((rand() % 21)+2);
        valid = true;
        for (int i = 0; i < tailLength; i++) {
            if ((x == tailX[i] && y == tailY[i]) || (x == playerX && y == playerY))
                valid = false;
        }
        for (int i = 0; i < all_obs.size(); i++) {
            for (int j = 0; j < all_obs[i].w/24; j++) {
                if (x == all_pos_obs[i][j].first && y == all_pos_obs[i][j].second)
                    valid = false;
            }
        }
    }
    pair<int, int> food;
    food = make_pair(x, y);
    return food;
}

bool Snake::gameOver(int x, int y) {
    if (x < 24 || y < 48 || x > unit * unit - 2*unit || y > unit * unit - 2*unit) return true;
    for (int i = 0; i < tailLength; i++) {
            if (x == tailX[i] && y == tailY[i]) {
                return true;
            }
    }
    for (int i = 0; i < all_obs.size(); i++) {
        for (int j = 0; j < all_obs[i].w/24; j++) {
            if (x == all_pos_obs[i][j].first && y == all_pos_obs[i][j].second) return true;
        }
    }
    return false;
}

void Snake::renderGameOver(SDL_Renderer* renderer, SDL_Event event) {
    SDL_Color Red = { 255, 0, 0 };
    SDL_Color White = { 255, 255, 255 };
    SDL_Color Black = { 0, 0, 0 };
    SDL_Color Pink = { 0, 128, 255};

    renderFont(renderer, Red, "BoobToob.ttf", 20, "Game Over" , 320, 120, 128, 158);
    renderFont(renderer, Black, "BoobToob.ttf", 20, "Press Enter to retry" , 300, 50, 138, 413);
    renderFont(renderer, White, "COOLVETI.TTF", 30,string("Your Score: ") + to_string(Score).c_str() , 300, 50, 140, 280);
    if (Score > highScore_prev) renderFont(renderer, Pink, "COOLVETI.TTF", 30, (string("New High Score: ") + to_string(highScore)).c_str() , 200, 30, 200, 330);
    else renderFont(renderer, White, "COOLVETI.TTF", 30, (string("High Score: ") + to_string(highScore)).c_str() , 200, 30, 200, 330);
    while (true) {
        SDL_RenderPresent(renderer);
        if (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                exit(0);
            }
            if (event.key.keysym.scancode == SDL_SCANCODE_RETURN) {
                return;
            }
        }
    }
}

void Snake::make_all_pos () {
    for (int i = 0; i < all_obs.size(); i++) {
        vector <pair<int,int>> pos;
        for (int j = 0; j < all_obs[i].w/24; j++) {
            pair <int, int> pa = make_pair(all_obs[i].x + 24*j,all_obs[i].y);
            pos.push_back(pa);
        }
        all_pos_obs.push_back(pos);
    }
}

void Snake::afterGameOver(int x, int y, pair<int, int> food_pos, SDL_Rect food) {
    Score = 0;
    tailX.clear();
    tailY.clear();
    tailX.push_back(24);
    tailY.push_back(48);
    tailLength = 1;
    all_obs.clear();
    all_pos_obs.clear();
    for (int i = 0; i < 6; i++) {
        SDL_Rect obstacle = make_obstacle();
        all_obs.push_back(obstacle);
    }
    make_all_pos();
    food_pos = makeFood(x, y);
    food.x = food_pos.first;
    food.y = food_pos.second;
}

void Snake::updateTail(int &prevX, int &prevY) {
    if (tailX.size() != tailLength) {
        tailX.push_back(prevX);
        tailY.push_back(prevY);
    }
    for (int i = 0; i < tailLength; i++) {
        if (i > 0) {
            tailX[i - 1] = tailX[i];
            tailY[i - 1] = tailY[i];
        }
    }
    if (tailLength > 0) {
        tailX[tailLength - 1] = prevX;
        tailY[tailLength - 1] = prevY;
    }
}

