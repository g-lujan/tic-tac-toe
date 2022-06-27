#include "text.h"

using std::pair;
using std::string;

namespace Text {

string score_text(const char winner, const pair<int, int> &scores) {
  string score_text = "WINNER: ";
  score_text.push_back(winner);
  score_text.append("\n\n---SCORES---");
  score_text.append("\nPLAYER X: " + std::to_string(scores.first));
  score_text.append("\nPLAYER O: " + std::to_string(scores.second));
  return score_text;
}

SDL_Texture *draw(SDL_Renderer *renderer, TTF_Font *font, const string& text) {
  SDL_Texture *ftexture = NULL;
  if (!font) {
    SDL_Log("Failed the load the font!\n");
    SDL_Log("SDL_TTF Error: %s \n", TTF_GetError());
    return ftexture;
  }
  SDL_Surface *text_surface = TTF_RenderText_Blended_Wrapped(font, text.c_str(), FONT_COLOR, 200);
  if (text_surface == NULL) {
    SDL_Log("Failed to render text surface!\n");
    SDL_Log("SDL_TTF Error: %s \n", TTF_GetError());
  } else {
    ftexture = SDL_CreateTextureFromSurface(renderer, text_surface);
    if (ftexture == NULL) {
      SDL_Log("Unable to create texture from rendered text!\n");
    } else {
      SDL_FreeSurface(text_surface);
    }
  }
  return ftexture;
}

} // namespace Text
