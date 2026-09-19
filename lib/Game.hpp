#ifndef GAME_HPP_
#define GAME_HPP_

#include "Shapes.hpp"
#include <array>
#include <iostream>

class State {
  int score{0};
  std::array<std::array<bool, 10>, 20> board{};

public:
  bool gameOver{false};
  State();
  void rotate();
  void move(Direction dir);
  void print(std::ostream &out);
  auto level() const { return linesCleared / 10 + 1; }
  // TODO: did i read the spec correctly? seems kinda slow...
  auto time() const { return (0.8 - ((level() - 1) * 0.007)); }

private:
  Tetromino curTetromino;
  void advance();
  void clearRows();
  void spawnTetromino();
  int linesCleared{0};
};

void initGame(std::ostream &out, std::istream &in);

#endif
