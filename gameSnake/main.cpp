#include <iostream>
#include <vector>
#include <string>
#include <ctime>
#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>
#include "SDL_utils.h"
#include "snake.h"
using namespace std;

const string WINDOW_TITLE = "Snake";

int main(int argc, char* argv[]) {
    srand(time(0));
	SDL_Init(SDL_INIT_EVERYTHING);
    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_Event event;
	initSDL(window, renderer, unit*unit, unit*unit , WINDOW_TITLE);
	if (TTF_Init() < 0) {
		cout << "Error: " << TTF_GetError() << endl;
	}

	Snake snake;

    snake.Score = 0;
	snake.highScore = 0;
	snake.highScore_prev = 0;

	SDL_Rect player;
	player.x = 48;
	player.y = 48;
	player.h = 0;
	player.w = 0;

	snake.tailLength = 1;

	snake.tailX.push_back(24);
	snake.tailY.push_back(48);

    for (int i = 0; i < 6; i++) {
        SDL_Rect obstacle = make_obstacle();
        snake.all_obs.push_back(obstacle);
    }
    snake.make_all_pos();

	int x = 48;
	int y = 48;
	int prevX = 48;
	int prevY = 48;

	bool up = false;
	bool down = false;
	bool right = false;
	bool left = false;

	SDL_Rect food;
	food.w = unit;
	food.h = unit;
	pair<int, int> food_pos = snake.makeFood(x, y);
	food.x = food_pos.first;
	food.y = food_pos.second;

	float time = SDL_GetTicks() / 100;
    int n = 1;

	while (true) {
        bool check = false;
		float newTime = SDL_GetTicks() / 100; //This value (75) is the speed at which the blocks are updated
		float delta = newTime - time;
		time = newTime;
		if (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT) {
				exit(0);
			}
			if (event.type == SDL_KEYDOWN ) {
				if (down == false && event.key.keysym.sym == SDLK_UP) {
					up = true;
					left = false;
					right = false;
					down = false;
				}
				else if (right == false && event.key.keysym.sym == SDLK_LEFT) {
					up = false;
					left = true;
					right = false;
					down = false;
				}
				else if (up == false && event.key.keysym.sym == SDLK_DOWN) {
					up = false;
					left = false;
					right = false;
					down = true;
				}
				else if (left == false && event.key.keysym.sym == SDLK_RIGHT) {
					up = false;
					left = false;
					right = true;
					down = false;
				}
			}
		}
		prevX = x;
		prevY = y;

		if (up) y -= delta * unit;
		else if (left) x -= delta * unit;
		else if (right) x += delta * unit;
		else if (down) y += delta * unit;

		if (snake.checkEatFood(food.x, food.y, x, y)) {
			if (n == 1) snake.Score += 10;
            else if (n == 2) snake.Score += 20;
            else if (n == 3) snake.Score += 30;
            else if (n == 4) snake.Score += 40;

            check = true;
			food_pos = snake.makeFood( x, y);
			food.x = food_pos.first;
			food.y = food_pos.second;

			snake.tailLength++;
		}
		if (delta * unit == 24 && (up || down || right || left)) {
			snake.updateTail(prevX, prevY);
		}

		if (snake.gameOver(x, y)) {
            snake.highScore_prev =snake.highScore;
			if (snake.Score > snake.highScore) snake.highScore = snake.Score;
			snake.renderGameOver(renderer, event);
			x = 48;
			y = 48;
			up = false;
			left = false;
			right = false;
			down = false;
			snake.afterGameOver(x, y, food_pos, food);
		}
        snake.renderObstacle(renderer);
        if (check) {
            n = (rand() % 4+1);
            snake.renderFood(renderer, food, n);
        }
        else {
            snake.renderFood(renderer, food, n);
        }
		snake.renderPlayer(renderer, player, x, y, up, down, right, left);
		snake.renderScore(renderer);

		SDL_SetRenderDrawColor(renderer, 0, 153, 76, 255);

		SDL_RenderDrawLine(renderer, 24, 48, 24 * 23, 48);
		SDL_RenderDrawLine(renderer, 24, 48, 24, 24 * 23);
		SDL_RenderDrawLine(renderer, 24 * 23, 48, 24*23, 24 * 23);
		SDL_RenderDrawLine(renderer, 24, 24 * 23, 24*23, 24 * 23);

		SDL_RenderPresent(renderer);

		SDL_SetRenderDrawColor(renderer, 204, 200, 220, 255);
		SDL_RenderClear(renderer);
	}
	quitSDL(window,renderer);

	return 0;

}


