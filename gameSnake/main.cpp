#include <iostream>
#include <vector>
#include <string>
#include <ctime>
#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>
#include "SDL_utils.h"
using namespace std;

const string WINDOW_TITLE = "Snake";
const int unit = 24;

SDL_Rect make_obstacle() {
    SDL_Rect obstacle;
    obstacle.x = 24*(rand() % 16+2);
    obstacle.y = 24*(rand() % 20+2);
    obstacle.w = 24*(rand() % 4 +1);
    obstacle.h = 24;
    return obstacle;
}

struct Snake {
    int tailLength;
    int Score, highScore, highScore_prev;
    vector <SDL_Rect> all_obs;
    vector<int> tailX;
    vector<int> tailY;
    vector<vector<pair<int,int>>> all_pos_obs;

    void renderObstacle(SDL_Renderer* renderer) {
        int s = all_obs.size();
        for (int i = 0; i < s; i++) {
            SDL_Surface* surface = IMG_Load("obs1.png");
            SDL_Texture* obs = SDL_CreateTextureFromSurface(renderer, surface);
            SDL_FreeSurface(surface);

            for (int j = 0; j < all_obs[i].w/unit; j++) {
                SDL_Rect dest;
                dest.w = unit;
                dest.h = unit;
                dest.x = all_obs[i].x + unit*j;
                dest.y = all_obs[i].y;
                SDL_RenderCopyEx(renderer, obs, NULL, &dest, 0, nullptr, SDL_FLIP_NONE);
            }
        }
    }

    void renderPlayer(SDL_Renderer* renderer, SDL_Rect player, int x, int y, bool up, bool down, bool right, bool left) {
        player.w = unit;
        player.h = unit;
        for (int i = 0; i < tailLength; i++) {
            player.x = tailX[i];
            player.y = tailY[i];
            SDL_Surface* surface_ceil = IMG_Load("ceil1.png");
            SDL_Texture* ceil = SDL_CreateTextureFromSurface(renderer, surface_ceil);
            SDL_FreeSurface(surface_ceil);
            SDL_Rect dest_ceil;
            dest_ceil.w = 24;
            dest_ceil.h = 24;
            dest_ceil.x = player.x;
            dest_ceil.y = player.y;
            double direction = 0;
            if (up) direction = 270;
            else if (down) direction = 90;
            else if (right) direction = 0;
            else if (left) direction = 180;
            SDL_RenderCopyEx(renderer, ceil, NULL, &dest_ceil, direction, nullptr, SDL_FLIP_NONE);
        }
        player.x = x;
        player.y = y;
        double direction1 = 0;
        if (up) direction1 = 270;
        else if (down) direction1 = 90;
        else if (right) direction1 = 0;
        else if (left) direction1 = 180;
        SDL_Surface* surface_head = IMG_Load("head1.png");
        SDL_Texture* head = SDL_CreateTextureFromSurface(renderer, surface_head);
        SDL_FreeSurface(surface_head);
        SDL_Rect dest_head;
        dest_head.w = 24;
        dest_head.h = 24;
        dest_head.x = x;
        dest_head.y = y;
        SDL_RenderCopyEx(renderer, head, NULL, &dest_head, direction1, nullptr, SDL_FLIP_NONE);
    }

    void renderFood(SDL_Renderer* renderer, SDL_Rect food, int &n) {
        SDL_Surface* surface;
        if (n == 1) surface = IMG_Load("apple1.png");
        else if (n == 2) surface = IMG_Load("kiwi.png");
        else if (n == 3) surface = IMG_Load("orange1.png");
        else if (n == 4) surface = IMG_Load("cherry2.png");
        SDL_Texture* fruit = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_FreeSurface(surface);
        SDL_Rect dest;
        dest.w = 24;
        dest.h = 24;
        dest.x = food.x;
        dest.y = food.y;
        SDL_RenderCopyEx(renderer, fruit, NULL, &dest, 0, nullptr, SDL_FLIP_NONE);
    }

    void renderScore(SDL_Renderer* renderer) {
        SDL_Color Black = {0, 0, 0};
        TTF_Font* font = TTF_OpenFont((char*)"COOLVETI.TTF", 20);
        if (font == NULL) {
            cout << "Font loading error" << endl;
            return;
        }
        SDL_Surface* score = TTF_RenderText_Solid(font, (string("Score: ") + to_string(Score)).c_str(), Black);
        SDL_Texture* scoreTexture = SDL_CreateTextureFromSurface(renderer, score);
        SDL_Rect scoreRect;
        scoreRect.w = 150;
        scoreRect.h = 48;
        scoreRect.x = ((unit*unit) / 2) - (scoreRect.w / 2);
        scoreRect.y = 0;
        SDL_RenderCopy(renderer, scoreTexture, NULL, &scoreRect);
        TTF_CloseFont(font);
    }

    bool checkEatFood(int foodx, int foody, int playerx, int playery) {
        if (playerx == foodx && playery == foody){
            return true;
        }
        return false;
    }

    pair<int, int> makeFood(int playerX, int playerY) {
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

    bool gameOver(int x, int y) {
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

    void renderGameOver(SDL_Renderer* renderer, SDL_Event event) {
        SDL_Color Red = { 255, 0, 0 };
        SDL_Color White = { 255, 255, 255 };
        SDL_Color Black = { 0, 0, 0 };
        SDL_Color Pink = { 0, 128, 255};
        //TTF_Font* font_gameOver = TTF_OpenFont((char*)"arial.ttf", 20);
        TTF_Font* font_score = TTF_OpenFont((char*)"COOLVETI.TTF", 30); //BoobToob.ttf
        TTF_Font* font = TTF_OpenFont((char*)"BoobToob.ttf", 20);

        SDL_Surface* gameover = TTF_RenderText_Solid(font, "Game Over", Red);
        SDL_Surface* retry = TTF_RenderText_Solid(font, "Press Enter to retry", Black);
        SDL_Surface* score = TTF_RenderText_Solid(font_score, (string("Your Score: ") + to_string(Score)).c_str(), White);
        SDL_Surface* highscore = TTF_RenderText_Solid(font_score, (string("High Score: ") + to_string(highScore)).c_str(), White);
        if (Score > highScore_prev) {
            highscore = TTF_RenderText_Solid(font_score, (string("New High Score: ") + to_string(highScore)).c_str(), Pink);
        }
        SDL_Texture* gameoverTexture = SDL_CreateTextureFromSurface(renderer, gameover);
        SDL_Texture* retryTexture = SDL_CreateTextureFromSurface(renderer, retry);
        SDL_Texture* scoreTexture = SDL_CreateTextureFromSurface(renderer, score);
        SDL_Texture* highscoreTexture = SDL_CreateTextureFromSurface(renderer, highscore);

        SDL_Rect gameoverRect;
        SDL_Rect retryRect;
        SDL_Rect scoreRect;
        SDL_Rect highscoreRect;

        gameoverRect.w = 320;
        gameoverRect.h = 120;
        gameoverRect.x = ((unit*unit) / 2)-(gameoverRect.w/2);
        gameoverRect.y = ((unit*unit) / 2)-(gameoverRect.h/2)-70;

        retryRect.w = 300;
        retryRect.h = 50;
        retryRect.x = ((unit*unit) / 2) - ((retryRect.w / 2));
        retryRect.y = (((unit*unit) / 2) - ((retryRect.h / 2))+150);

        scoreRect.w = 300;
        scoreRect.h = 50;
        scoreRect.x = 140;
        scoreRect.y = 190*2-100;

        highscoreRect.w = 200;
        highscoreRect.h = 30;
        highscoreRect.x = 200;
        highscoreRect.y = 190*2-50;

        SDL_RenderCopy(renderer, gameoverTexture, NULL, &gameoverRect);
        SDL_RenderCopy(renderer, retryTexture, NULL, &retryRect);
        SDL_RenderCopy(renderer, scoreTexture, NULL, &scoreRect);
        SDL_RenderCopy(renderer, highscoreTexture, NULL, &highscoreRect);

        TTF_CloseFont(font);
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

    void make_all_pos () {
        for (int i = 0; i < all_obs.size(); i++) {
            vector <pair<int,int>> pos;
            for (int j = 0; j < all_obs[i].w/24; j++) {
                pair <int, int> pa = make_pair(all_obs[i].x + 24*j,all_obs[i].y);
                pos.push_back(pa);
            }
            all_pos_obs.push_back(pos);
        }
    }

    void afterGameOver(int x, int y, pair<int, int> food_pos, SDL_Rect food) {
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

    void updateTail(int &prevX, int &prevY) {
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

};


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
		// The previous position of the player block
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


