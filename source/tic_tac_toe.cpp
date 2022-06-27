#include "board.h"
#include "text.h"
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <array>
#include <iostream>
#include <optional>
#include <random>

using std::array;
using std::make_pair;
using std::optional;
using std::pair;
using std::string;

namespace {
const unsigned int WIDTH = 600;
const unsigned int HEIGHT = 600;
const string X_SPRITE = "resources/X.png";
const string O_SPRITE = "resources/O.png";
enum class Difficulty { Easy };
} // namespace

// forward declarations
SDL_Point enemy_play(Difficulty difficulty);
void end_round(SDL_Renderer *renderer, TTF_Font *font, const char winner, Board& board);

void game_loop(SDL_Renderer *renderer) {
  SDL_Event event;
  SDL_Point coord;
  bool successful_play = true;
  Board board(renderer, make_pair(X_SPRITE, O_SPRITE));
  TTF_Font *font = TTF_OpenFont(Text::FONT_PATH.c_str(), Text::FONT_SIZE);
  while (1) {
    if (SDL_PollEvent(&event) && event.type == SDL_QUIT) {
      break;
    }
    if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE) {
      break;
    }
    SDL_PumpEvents(); // make sure we have the latest mouse state.
    if (board.player_round()) {
      Uint32 buttons = SDL_GetMouseState(&coord.x, &coord.y);
      if ((buttons & SDL_BUTTON_LMASK) != 0) {
        successful_play = board.set_cell(coord);
      }
    } else {
      successful_play = board.set_cell(enemy_play(Difficulty::Easy));
    }
    if (!successful_play) {
      continue;
    }
    board.draw(renderer);
    auto winner = board.winner();
    if (winner.has_value()) {
      end_round(renderer, font, winner.value(), board);
    }
    SDL_RenderPresent(renderer);
  }
}

// gcc unused attribute to avoid compiler warnings => SDL requires this
// signature for main
int main(__attribute__((unused)) int argc,
         __attribute__((unused)) char *argv[]) {
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    SDL_Log("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
  }
  if (TTF_Init() < 0) {
    SDL_Log("TTF could not initialize! SDL_Error: %s\n", SDL_GetError());
  }
  SDL_Window *window = NULL;
  SDL_Renderer *renderer = NULL;
  SDL_CreateWindowAndRenderer(WIDTH, HEIGHT, 0, &window, &renderer);
  if (window == NULL) {
    printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
  } else {
    game_loop(renderer);
  }
  SDL_DestroyWindow(window);
  SDL_Quit();
  return 0;
}

SDL_Point enemy_play(Difficulty difficulty) {
  if (difficulty == Difficulty::Easy) {
    SDL_Delay(500);
    std::random_device rd;  // obtain a random number from hardware
    std::mt19937 gen(rd()); // seed the generator
    std::uniform_int_distribution<> distr_x(0, WIDTH);  // define the range
    std::uniform_int_distribution<> distr_y(0, HEIGHT); // define the range
    return {distr_x(gen), distr_y(gen)};
  }
  return {0, 0};
}

void end_round(SDL_Renderer *renderer, TTF_Font *font, const char winner,
               Board& board) {
  SDL_RenderPresent(renderer);
  SDL_Delay(1000);
  SDL_RenderClear(renderer);
  SDL_Texture *score_texture =
      Text::draw(renderer, font, Text::score_text(winner, board.scores()));
  board.reset();
  SDL_Rect score_dest = {100, 100, 400, 200};
  SDL_RenderCopy(renderer, score_texture, NULL, &score_dest);
  SDL_RenderPresent(renderer);
  SDL_Delay(2000);
  SDL_RenderClear(renderer);
}
