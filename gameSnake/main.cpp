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

SDL_Rect make_obstacle() {
    SDL_Rect obstacle;
    obstacle.x = 24*(rand() % 16+2);
    obstacle.y = 24*(rand() % 20+2);
    obstacle.w = 24*(rand() % 4 +1);
    obstacle.h = 24;
    return obstacle;
}

void renderObstacle(SDL_Renderer* renderer, vector <SDL_Rect> chuong_ngai_vat) {
    int s = chuong_ngai_vat.size();
    for (int i = 0; i < s; i++) {
        auto surface = IMG_Load("obs1.png");
        SDL_Texture* obs = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_FreeSurface(surface);

        for (int j = 0; j < chuong_ngai_vat[i].w/24; j++) {
            SDL_Rect dest;
            dest.w = 24;
            dest.h = 24;
            dest.x = chuong_ngai_vat[i].x + 24*j;
            dest.y = chuong_ngai_vat[i].y;
            SDL_RenderCopyEx(renderer, obs, NULL, &dest, 0, nullptr, SDL_FLIP_NONE);
        }
    }
}

void renderPlayer(SDL_Renderer* renderer, SDL_Rect player, int x, int y, int scale, vector<int> tailX, vector<int> tailY, int tailLength,
                  bool up, bool down, bool right, bool left) {
	player.w = scale;
	player.h = scale;

	for (int i = 0; i < tailLength; i++) {
		player.x = tailX[i];
		player.y = tailY[i];

        auto surface2 = IMG_Load("ceil.png");
        SDL_Texture* ceil = SDL_CreateTextureFromSurface(renderer, surface2);
        SDL_FreeSurface(surface2);

        SDL_Rect dest2;
        dest2.w = 24;
        dest2.h = 24;
        dest2.x = player.x;
        dest2.y = player.y;

        double direction = 0;
        if (up) direction = 270;
        else if (down) direction = 90;
        else if (right) direction = 0;
        else if (left) direction = 180;

        SDL_RenderCopyEx(renderer, ceil, NULL, &dest2, direction, nullptr, SDL_FLIP_NONE);
	}
	player.x = x;
	player.y = y;

	double direction1 = 0;
	if (up) direction1 = 270;
	else if (down) direction1 = 90;
    else if (right) direction1 = 0;
    else if (left) direction1 = 180;

	auto surface1 = IMG_Load("head.png");
    SDL_Texture* head_snake = SDL_CreateTextureFromSurface(renderer, surface1);
    SDL_FreeSurface(surface1);
    SDL_Rect dest1;
    dest1.w = 24;
    dest1.h = 24;
    dest1.x = x;
    dest1.y = y;
    SDL_RenderCopyEx(renderer, head_snake, NULL, &dest1, direction1, nullptr, SDL_FLIP_NONE);
}

void renderFood(SDL_Renderer* renderer, SDL_Rect food, int &n) {
    SDL_Texture* sprites;
    if (n == 1) {
        auto surface = IMG_Load("apple1.png");
        sprites = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_FreeSurface(surface);
    }
    else if (n == 2) {
        auto surface = IMG_Load("kiwi.png");
        sprites = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_FreeSurface(surface);
    }
    else if (n == 3) {
        auto surface = IMG_Load("orange1.png");
        sprites = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_FreeSurface(surface);
    }
    else if (n == 4) {
        auto surface = IMG_Load("cherry2.png");
        sprites = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_FreeSurface(surface);
    }

    SDL_Rect dest;
    dest.w = 24;
    dest.h = 24;
    dest.x = food.x;
    dest.y = food.y;
    SDL_RenderCopyEx(renderer, sprites, NULL, &dest, 0, nullptr, SDL_FLIP_NONE);
}

void renderScore(SDL_Renderer* renderer, int tailLength, int scale, int wScale, int &Score) {
    SDL_Color White = {255, 255, 255};
    SDL_Color Black = {0, 0, 0};
	// Get the font used for displaying text
	TTF_Font* font = TTF_OpenFont((char*)"COOLVETI.TTF", 20);
	if (font == NULL) {
		cout << "Font loading error" << endl;
		return;
	}
	SDL_Surface* score = TTF_RenderText_Solid(font, (string("Score: ") + to_string(Score)).c_str(), Black);
	SDL_Texture* scoreMessage = SDL_CreateTextureFromSurface(renderer, score);
	SDL_Rect scoreRect;
	scoreRect.w = 150;
	scoreRect.h = 48;
	scoreRect.x = ((scale*wScale) / 2) - (scoreRect.w / 2);
	scoreRect.y = 0;
	SDL_RenderCopy(renderer, scoreMessage, NULL, &scoreRect);
	TTF_CloseFont(font);
}

bool checkCollision(int foodx, int foody, int playerx, int playery) {
	if (playerx == foodx && playery == foody){
		return true;
	}
	return false;
}

pair<int, int> getFoodSpawn(vector<int> tailX, vector<int> tailY, int playerX, int playerY, int scale, int wScale, int tailLength,
                            vector<vector<pair<int,int>>> tong_hop, vector <SDL_Rect> chuong_ngai_vat) {
	bool valid = false;
	int x = 0;
	int y = 0;
	x = scale * ((rand() % 22)+1);
	y = scale * ((rand() % 21)+2);
	valid = true;
	// Check all tail blocks and player block
	for (int i = 0; i < tailLength; i++) {
		if ((x == tailX[i] && y == tailY[i]) || (x == playerX && y == playerY)) {
			valid = false;
		}
	}
	for (int i = 0; i < chuong_ngai_vat.size(); i++) {
        for (int j = 0; j < chuong_ngai_vat[i].w/24; j++) {
            if (x == tong_hop[i][j].first && y == tong_hop[i][j].second) valid = false;
        }
	}
	pair<int, int> foodLoc;
	if (!valid) {
		foodLoc = make_pair(-100, -100);
		return foodLoc;
	}
	foodLoc = make_pair(x, y);
	return foodLoc;
}

bool gameOver(int x, int y, int scale, int wScale, int tailLength, vector<int> tailX, vector<int> tailY,
              vector<vector<pair<int,int>>> tong_hop, vector <SDL_Rect> chuong_ngai_vat) {
    if (x < 24 || y < 48 || x > scale * wScale - 2*scale || y > scale * wScale - 2*scale) return true;
    for (int i = 0; i < tailLength; i++) {
			if (x == tailX[i] && y == tailY[i]) {
                return true;
			}
    }
    for (int i = 0; i < chuong_ngai_vat.size(); i++) {
        for (int j = 0; j < chuong_ngai_vat[i].w/24; j++) {
            if (x == tong_hop[i][j].first && y == tong_hop[i][j].second) return true;
        }
	}
    return false;
}

void renderGameOver(SDL_Renderer* renderer, SDL_Event event, int scale, int wScale, int tailLength, int &Score, int &highScore, int &highScore_prev) {
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
	SDL_Texture* gameoverMessage = SDL_CreateTextureFromSurface(renderer, gameover);
	SDL_Texture* retryMessage = SDL_CreateTextureFromSurface(renderer, retry);
	SDL_Texture* scoreMessage = SDL_CreateTextureFromSurface(renderer, score);
	SDL_Texture* highscoreMessage = SDL_CreateTextureFromSurface(renderer, highscore);

	SDL_Rect gameoverRect;
	SDL_Rect retryRect;
	SDL_Rect scoreRect;
	SDL_Rect highscoreRect;

	gameoverRect.w = 320;
	gameoverRect.h = 120;
	gameoverRect.x = ((scale*wScale) / 2)-(gameoverRect.w/2);
	gameoverRect.y = ((scale*wScale) / 2)-(gameoverRect.h/2)-70;

	retryRect.w = 300;
	retryRect.h = 50;
	retryRect.x = ((scale*wScale) / 2) - ((retryRect.w / 2));
	retryRect.y = (((scale*wScale) / 2) - ((retryRect.h / 2))+150);

	scoreRect.w = 300;
	scoreRect.h = 50;
	scoreRect.x = 140;
	scoreRect.y = 190*2-100;

	highscoreRect.w = 200;
	highscoreRect.h = 30;
	highscoreRect.x = 200;
	highscoreRect.y = 190*2-50;

	SDL_RenderCopy(renderer, gameoverMessage, NULL, &gameoverRect);
	SDL_RenderCopy(renderer, retryMessage, NULL, &retryRect);
	SDL_RenderCopy(renderer, scoreMessage, NULL, &scoreRect);
    SDL_RenderCopy(renderer, highscoreMessage, NULL, &highscoreRect);

	TTF_CloseFont(font);
	// Show game over screen while space has not been pressed
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

int main(int argc, char* argv[]) {
    srand(time(0));
	SDL_Init(SDL_INIT_EVERYTHING);
	int Score = 0;
	int highScore = 0;
	int highScore_prev = 0;
	int scale = 24;
	int wScale = 24;
	SDL_Window* window;
	SDL_Renderer* renderer;
	SDL_Event event;
	initSDL(window, renderer, scale*wScale+1, scale*wScale+1 , WINDOW_TITLE);
	if (TTF_Init() < 0) {
		cout << "Error: " << TTF_GetError() << endl;
	}

	SDL_Rect player;
	player.x = 48;
	player.y = 48;
	player.h = 0;
	player.w = 0;

	int tailLength = 1;

	vector<int> tailX;
	vector<int> tailY;
	tailX.push_back(24);
	tailY.push_back(48);

	vector <SDL_Rect> chuong_ngai_vat;
    for (int i = 0; i < 6; i++) {
        SDL_Rect obstacle = make_obstacle();
        chuong_ngai_vat.push_back(obstacle);
    }
	vector<vector<pair<int,int>>> tong_hop;
	for (int i = 0; i < chuong_ngai_vat.size(); i++) {
        vector <pair<int,int>> toado;
        for (int j = 0; j < chuong_ngai_vat[i].w/24; j++) {
            pair <int, int> pa = make_pair(chuong_ngai_vat[i].x + 24*j,chuong_ngai_vat[i].y);
            toado.push_back(pa);
        }
        tong_hop.push_back(toado);
    }


	int x = 48;
	int y = 48;
	int prevX = 48;
	int prevY = 48;

	bool up = false;
	bool down = false;
	bool right = false;
	bool left = false;

	bool redo = false;

	SDL_Rect food;
	food.w = scale;
	food.h = scale;
	food.x = 0;
	food.y = 0;
	pair<int, int> foodLoc = getFoodSpawn(tailX, tailY, x, y, scale, wScale, tailLength, tong_hop, chuong_ngai_vat);
	food.x = foodLoc.first;
	food.y = foodLoc.second;

	float time = SDL_GetTicks() / 100;
    int n = 1;

	while (true) {
        bool check = false;
		float newTime = SDL_GetTicks() / 100; //This value (75) is the speed at which the blocks are updated
		float delta = newTime - time;

		//cout << "delta la: " << delta << endl;
		time = newTime;
		if (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT) {
				exit(0);
			}
			if (event.type == SDL_KEYDOWN ) {
				if (down == false && event.key.keysym.sym == SDLK_UP)
                    {
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

		if (up) y -= delta * scale;
		else if (left) x -= delta * scale;
		else if (right) x += delta * scale;
		else if (down) y += delta * scale;
		while (redo == true) {
            redo = false;
			foodLoc = getFoodSpawn(tailX, tailY, x, y, scale, wScale, tailLength, tong_hop, chuong_ngai_vat);
			food.x = foodLoc.first;
			food.y = foodLoc.second;
			if (food.x == -100 && food.y == -100) {
				redo = true;
			}
		}
		// Collision detection, has played collided with food?
		if (checkCollision(food.x, food.y, x, y)) {
			// Spawn new food after it has been eaten
			if (n == 1) Score += 10;
            else if (n == 2) Score += 20;
            else if (n == 3) Score += 30;
            else if (n == 4) Score += 40;

            check = true;
			foodLoc = getFoodSpawn(tailX, tailY, x, y, scale, wScale, tailLength, tong_hop, chuong_ngai_vat);
			food.x = foodLoc.first;
			food.y = foodLoc.second;
			if (food.x == -100 && food.y == -100) {
				redo = true;
			}
			tailLength++;
		}
		// Only runs in the frames where the player block has moved
		if (delta * scale == 24 && (up || down || right || left)) {
			// Update tail size and position
			if (tailX.size() != tailLength) {
				tailX.push_back(prevX);
				tailY.push_back(prevY);
			}
			//Loop through every tail block, move all blocks to the nearest block in front
			//This updates the blocks from end (farthest from player block) to the start (nearest to player block)
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

		if (gameOver(x,y,scale, wScale, tailLength, tailX, tailY, tong_hop, chuong_ngai_vat)) {
            highScore_prev = highScore;
			if (Score > highScore) highScore = Score;
			renderGameOver(renderer, event, scale, wScale, tailLength, Score, highScore, highScore_prev);
			Score = 0;
			x = 48;
			y = 48;
			up = false;
			left = false;
			right = false;
			down = false;
			tailX.clear();
			tailY.clear();
            tailX.push_back(24);
            tailY.push_back(48);
			tailLength = 1;
			redo = false;
			foodLoc = getFoodSpawn(tailX, tailY, x, y, scale, wScale, tailLength, tong_hop, chuong_ngai_vat);
			food.x = foodLoc.first;
			food.y = foodLoc.second;
			if (food.x == -100 && food.y == -100) {
				redo = true;
			}
			chuong_ngai_vat.clear();
			tong_hop.clear();
            for (int i = 0; i < 6; i++) {
                SDL_Rect obstacle = make_obstacle();
                chuong_ngai_vat.push_back(obstacle);
            }
            for (int i = 0; i < chuong_ngai_vat.size(); i++) {
                vector <pair<int,int>> toado;
                for (int j = 0; j < chuong_ngai_vat[i].w/24; j++) {
                    pair <int, int> pa = make_pair(chuong_ngai_vat[i].x + 24*j,chuong_ngai_vat[i].y);
                    toado.push_back(pa);
                }
                tong_hop.push_back(toado);
            }
		}

		// Render everything
        renderObstacle(renderer, chuong_ngai_vat);

        if (check) {
            n = (rand() % 4+1);
            renderFood(renderer, food, n);
        }
        else {
            renderFood(renderer, food, n);
        }

		renderPlayer(renderer, player, x, y, scale, tailX, tailY, tailLength, up, down, right, left);
		renderScore(renderer, tailLength, scale, wScale, Score);

		SDL_SetRenderDrawColor(renderer, 0, 153, 76, 255);

		SDL_RenderDrawLine(renderer, 24, 48, 24 * 23, 48);
		SDL_RenderDrawLine(renderer, 24, 48, 24, 24 * 23);
		SDL_RenderDrawLine(renderer, 24 * 23, 48, 24*23, 24 * 23);
		SDL_RenderDrawLine(renderer, 24, 24 * 23, 24*23, 24 * 23);

		SDL_RenderPresent(renderer);

		SDL_SetRenderDrawColor(renderer, 204, 200, 220, 255);
		SDL_RenderClear(renderer);
	}
	quitSDL(window, renderer);

	return 0;

}


