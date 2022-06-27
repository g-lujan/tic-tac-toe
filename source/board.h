#ifndef _BOARD_H_
#define _BOARD_H_

#include <SDL.h>
#include <array>
#include <optional>
#include <string>

struct Cell {
  bool drawn{false};
  char character{' '};
  // default three-way comparison operator, as per C++20
  auto operator<=>(const Cell &) const = default;
};

class Board {
public:
  Board(SDL_Renderer *renderer,
        const std::pair<std::string, std::string> &textures);
  bool set_cell(const SDL_Point coord);
  void draw(SDL_Renderer *renderer);
  std::optional<char> winner();
  void reset();

  bool player_round() const { return _player_round; }
  std::pair<int, int> scores() const { return _scores; }

private:
  int WIDTH{600}, HEIGHT{600};
  std::pair<int, int> _scores;
  std::array<std::array<Cell, 3>, 3> _board{};
  SDL_Texture *_texture_p1;
  SDL_Texture *_texture_p2;
  bool _player_round{true};
  int _available_cells{9};

  void _increment_score(const char player) {
    if (player == 'X') {
      _scores.first++;
    } else if (player == 'O') {
      _scores.second++;
    }
  }
};

#endif
