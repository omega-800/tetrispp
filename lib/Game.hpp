#ifndef GAME_HPP_
#define GAME_HPP_

#include "Shapes.hpp"
#include <array>
#include <iostream>

class State {
  int score{0}, level{1};
  std::array<std::array<bool, 10>, 20> board{};

public:
  State();
  void rotate();
  void move(Direction dir);
  void print(std::ostream &out);

private:
  Tetromino curTetromino;
  void advance();
  void clearRows();
  void spawnTetromino();
};

void initGame(std::ostream &out, std::istream &in);

#endif
