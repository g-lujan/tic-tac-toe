#include "board.h"
#include <SDL_image.h>
#include <string>

using std::make_pair;
using std::optional;
using std::pair;
using std::string;
using std::size_t;

// forward declaration
static SDL_Texture *load_texture(SDL_Renderer *renderer, const string &path);

Board::Board(SDL_Renderer *renderer,
             const std::pair<std::string, std::string> &textures) {
  _texture_p1 = load_texture(renderer, textures.first);
  _texture_p2 = load_texture(renderer, textures.second);
}

bool Board::set_cell(const SDL_Point coord) {
  const int line = coord.x < HEIGHT / 3 ? 0 : coord.x < 2 * HEIGHT / 3 ? 1 : 2;
  const int col = coord.y < WIDTH / 3 ? 0 : coord.y < 2 * WIDTH / 3 ? 1 : 2;
  if (_board[line][col].drawn) {
    return false;
  }
  _board[line][col].character = _player_round ? 'X' : 'O';
  _player_round = !_player_round;
  _available_cells--;
  return true;
}

void Board::draw(SDL_Renderer *renderer) {
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {
      if (_board[i][j].drawn || _board[i][j].character == ' ') {
        continue;
      }
      SDL_Rect destination =
          {WIDTH / 3 * i, HEIGHT / 3 * j, WIDTH / 3, HEIGHT / 3};
      SDL_RenderCopy(renderer,
                     _board[i][j].character == 'X' ? _texture_p1 : _texture_p2,
                     NULL, &destination);
      _board[i][j].drawn = true;
    }
  }
}

optional<char> Board::winner() {
  // check diagonals
  if (_board[0][0].drawn && _board[0][0] == _board[1][1] &&
      _board[1][1] == _board[2][2]) {
    _increment_score(_board[1][1].character);
    return _board[1][1].character;
  }
  if (_board[0][2].drawn && _board[0][2] == _board[1][1] &&
      _board[1][1] == _board[2][0]) {
    _increment_score(_board[1][1].character);
    return _board[1][1].character;
  }
  // check cols
  for (int line = 0; line < 3; line++) {
    if (_board[line][0].drawn && _board[line][0] == _board[line][1] &&
        _board[line][1] == _board[line][2]) {
      _increment_score(_board[line][0].character);
      return _board[line][0].character;
    }
  }
  // check lines
  for (int col = 0; col < 3; col++) {
    if (_board[0][col].drawn && _board[0][col] == _board[1][col] &&
        _board[1][col] == _board[2][col]) {
      _increment_score(_board[0][col].character);
      return _board[0][col].character;
    }
  }
  if(_available_cells == 0){
	  return '-';
  }
  return {};
}

void Board::reset() {
  _player_round = true;
  _available_cells = 9;
  for (auto &line : _board) {
    for (auto &cell : line) {
      cell.drawn = false;
      cell.character = ' ';
    }
  }
}

static SDL_Texture *load_texture(SDL_Renderer *renderer, const string &path) {
  SDL_Surface *surface = IMG_Load(path.c_str());
  if (!surface) {
    SDL_Log("IMG_LOAD: %s\n", IMG_GetError());
  }
  SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
  SDL_FreeSurface(surface);
  return texture;
}
