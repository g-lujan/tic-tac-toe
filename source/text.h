#ifndef _TEXT_H_
#define _TEXT_H_

#include <SDL.h>
#include <SDL_ttf.h>
#include <string>

namespace Text {
const std::string FONT_PATH = "resources/8bitOperatorPlus-Regular.ttf";
const int FONT_SIZE = 24;
const SDL_Color FONT_COLOR{255, 165, 0, 255};
std::string score_text(const char winner, const std::pair<int, int> &scores);
SDL_Texture* draw(SDL_Renderer *renderer, TTF_Font *font, const std::string& text);
} // namespace Text

#endif
