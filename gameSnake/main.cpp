#include <bits/stdc++.h>
#include <SDL.h>
#include <Windows.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include "SDL_utils.h"
#define MAX_SIZE_OF_SNAKE 100
// #define SDL_ERROR 1
// #define IMG_ERROR 2
using namespace std;

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;
const string WINDOW_TITLE = "Game Snake";
int STEP_DELAY = 200;

void Background(SDL_Renderer* renderer) {   // chua dung den
//    SDL_Texture* background = loadTexture("bgr.jpg", renderer);
//    SDL_RenderCopy(renderer, background, NULL, NULL);
//    SDL_RenderPresent(renderer);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 0);    // mau nen
    SDL_RenderClear(renderer);
}
void presentFood(SDL_Renderer* renderer, SDL_Rect rec) {
    SDL_SetRenderDrawColor(renderer, 255, 165, 0, 0);       // mau thuc an
    SDL_RenderFillRect(renderer, &rec);
    SDL_RenderPresent(renderer);
}
class Snake {
private:
    SDL_Rect snaketemp;
    vector <SDL_Rect> cell;
    int debug = 1;
    int LorR, UorD;

public:
    Snake() {
        snaketemp.h = 20;
        snaketemp.w = 20;
        snaketemp.x = 0;
        snaketemp.y = 0;
        cell.push_back(snaketemp);
        UorD = 0;
        LorR = 1;
    }
    int getSnakeTempX() {
        return snaketemp.x;
    }
    int getSnakeTempY() {
        return snaketemp.y;
    }
    bool isEat(SDL_Rect food) {
        return (food.x == snaketemp.x && food.y == snaketemp.y);
    }
    void setDirection(int a, int b) {
        if ((LorR * a) != -1) LorR = a;
        if ((UorD * b) != -1) UorD = b;
    }

    void EraseTail() {
        cell.erase(cell.begin());
    }
    int getLorR() {
        return LorR;
    }
    int getUorD() {
        return UorD;
    }
    void GrowUp() {
        cell.push_back(snaketemp);
    }
    void Move() {
        switch (LorR) {
        case 0:
            snaketemp.y += 20 * UorD;
            break;
        case -1 : case 1:
            snaketemp.x += 20 * LorR;
            break;
        }
// dam vao bo tuong khong chet
//        if (snaketemp.x == -20)
//            snaketemp.x = SCREEN_WIDTH - 20;
//        else if (snaketemp.x == SCREEN_WIDTH)
//            snaketemp.x = 0;
//        if (snaketemp.y == -20)
//            snaketemp.y = SCREEN_HEIGHT - 20;
//        else if (snaketemp.y == SCREEN_HEIGHT)
//            snaketemp.y = 0;

        cell.push_back(snaketemp);
        EraseTail();
        cout << cell.size() << endl;
    }
    void ShowSnake(SDL_Renderer* renderer) {
        for (size_t i = 0; i < cell.size(); i++) {
            SDL_SetRenderDrawColor(renderer, 128, 0, 128, 100);     // mau ran
            SDL_RenderFillRect(renderer, &cell[i]);
        }
        SDL_RenderPresent(renderer);
    }
};

bool gameOver(int x, int y) {
     return (x < 0 || y < 0 || x > SCREEN_WIDTH || y > SCREEN_HEIGHT);
}
void setLevel() {
    cout << "Choose level: " << endl;
    int n;
    cin >> n;
    if (n == 1) STEP_DELAY = 500;
    else if (n == 2) STEP_DELAY = 400;
    else if (n == 3) STEP_DELAY = 300;
    else if (n == 4) STEP_DELAY = 200;
    else if (n == 5) STEP_DELAY = 100;
}
void Play(int &score) {
    srand((unsigned int)time(NULL));
    SDL_Window* window;
    SDL_Renderer* renderer;
    initSDL(window, renderer, SCREEN_WIDTH, SCREEN_HEIGHT, WINDOW_TITLE);

    SDL_Event e;
    Snake snake;
    SDL_Rect food;
    food.w = 20;
    food.h = 20;
    food.x = rand() % (SCREEN_WIDTH / 20) * 20;
    food.y = rand() % (SCREEN_HEIGHT / 20) * 20;
    bool run = true;
    while (run && !gameOver(snake.getSnakeTempX(), snake.getSnakeTempY())) {
        //SDL_Texture* background = loadTexture("bgr.jpg", renderer);
        //SDL_RenderCopy(renderer, background, NULL, NULL);
        //SDL_RenderPresent(renderer);
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) run = false;
            else if (e.type == SDL_KEYDOWN) {
                switch (e.key.keysym.sym) {
                case SDLK_ESCAPE:
                    run = false;
                    break;
                case SDLK_w: case SDLK_UP:
                    snake.setDirection(0,-1);
                    Background(renderer);
                    SDL_Delay(STEP_DELAY);
                    snake.Move();
                    snake.ShowSnake(renderer);
                    if (snake.isEat(food)) {
                        snake.GrowUp();
                        food.x = rand() % (SCREEN_WIDTH / 20) * 20;
                        food.y = rand() % (SCREEN_HEIGHT / 20) * 20;
                        presentFood(renderer, food);
                        score++;
                    }
                    presentFood(renderer, food);
                    break;
                case SDLK_s: case SDLK_DOWN:
                    snake.setDirection(0,1);
                    Background(renderer);
                    SDL_Delay(STEP_DELAY);
                    snake.Move();
                    snake.ShowSnake(renderer);
                    if (snake.isEat(food)) {
                        snake.GrowUp();
                        food.x = rand() % (SCREEN_WIDTH / 20) * 20;
                        food.y = rand() % (SCREEN_HEIGHT / 20) * 20;
                        presentFood(renderer, food);
                        score++;
                    }
                    presentFood(renderer, food);
                    break;
                case SDLK_a: case SDLK_LEFT:
                    snake.setDirection(-1,0);
                    Background(renderer);
                    SDL_Delay(STEP_DELAY);
                    snake.Move();
                    snake.ShowSnake(renderer);
                    if (snake.isEat(food)) {
                        snake.GrowUp();
                        food.x = rand() % (SCREEN_WIDTH / 20) * 20;
                        food.y = rand() % (SCREEN_HEIGHT / 20) * 20;
                        presentFood(renderer, food);
                        score++;
                    }
                    presentFood(renderer, food);
                    break;
                case SDLK_d: case SDLK_RIGHT:
                    snake.setDirection(1,0);
                    Background(renderer);
                    SDL_Delay(STEP_DELAY);
                    snake.Move();
                    snake.ShowSnake(renderer);
                    if (snake.isEat(food)) {
                        snake.GrowUp();
                        food.x = rand() % (SCREEN_WIDTH / 20) * 20;
                        food.y = rand() % (SCREEN_HEIGHT / 20) * 20;
                        presentFood(renderer, food);
                        score++;
                    }
                    presentFood(renderer, food);
                    break;
                }
            }
        }
        Background(renderer);
        SDL_Delay(STEP_DELAY);
        snake.Move();
        snake.ShowSnake(renderer);
        if (snake.isEat(food)) {
            snake.GrowUp();
            food.x = rand() % (SCREEN_WIDTH / 20) * 20;
            food.y = rand() % (SCREEN_HEIGHT / 20) * 20;
            presentFood(renderer, food);
            score++;
        }
        presentFood(renderer, food);
    }
    quitSDL(window, renderer);
}
int main (int argc, char* argv[]) {
    int score = 1;
    //setLevel();
    Play(score);
    cout << "GAME OVER!" << endl;
    cout << "Your score: " << score << endl;
    return 0;
}


























