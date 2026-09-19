#ifndef GAME_HPP_
#define GAME_HPP_

#include <iostream>
#include <vector>

enum Direction { left, right, up, down };

class Tetromino {
public:
  int x{0}, y{0};
  Direction dir;
};

class TetrominoI : public Tetromino {};
class TetrominoO : public Tetromino {};
class TetrominoT : public Tetromino {};
class TetrominoS : public Tetromino {};
class TetrominoZ : public Tetromino {};
class TetrominoJ : public Tetromino {};
class TetrominoL : public Tetromino {};

class State {
  int score{0};
  bool board[20][10];

public:
  State();
  void move(Direction dir);
  void tick();
  void print(std::ostream &out);
private:
  Tetromino curTetromino;
  void advance();
};

void initGame(std::ostream &out, std::istream &in);

#endif
