#include "Game.hpp"
#include "Print.hpp"
#include "Shapes.hpp"

#include <chrono>
#include <iostream>
#include <random>
#include <sys/select.h>
#include <thread>

State::State() : board{} { spawnTetromino(); }

void State::print(std::ostream &out) {
  clear(out);
  out << "Move: h, l         Advance: j" << "\n";
  out << "Rotate: k          Quit: q" << "\n";
  out << "Score: " << score << "\n";
  auto view = board;
  for (int c = 0; c < TETRIMINO_SIZE; c++) {
    int col = curTetromino.x + SHAPES[curTetromino.type][curTetromino.dir][c][0];
    int row = curTetromino.y + SHAPES[curTetromino.type][curTetromino.dir][c][1];
    if (col >= 0 && col < board[0].size() && row >= 0 && row < board.size())
      view[row][col] = true;
  }
  for (auto &line : view) {
    for (auto cell : line)
      out << (cell ? "XX" : "__") << " ";
    out << '\n';
  }
}

void State::rotate() {
  curTetromino.dir = (Direction)((curTetromino.dir + 1) % 4);
}

void State::move(Direction dir) {
  int dx = (dir == right) - (dir == left);
  int dy = (dir == down) ? 1 : 0;
  if (dx == 0 && dy == 0)
    return;
  for (int c = 0; c < TETRIMINO_SIZE; c++) {
    int col = curTetromino.x + dx + SHAPES[curTetromino.type][curTetromino.dir][c][0];
    int row = curTetromino.y + dy + SHAPES[curTetromino.type][curTetromino.dir][c][1];
    bool out = row >= (int)board.size() || col < 0 || col >= (int)board[0].size();
    bool hit = !out && row >= 0 && board[row][col];
    if (out || hit) {
      if (dir == down)
        advance();
      return;
    }
  }
  curTetromino.x += dx;
  curTetromino.y += dy;
}

void State::spawnTetromino() {
  static std::mt19937 gen{std::random_device{}()};
  static std::uniform_int_distribution<int> dist{0, Type::count - 1};
  curTetromino.x = 4;
  curTetromino.y = 0;
  curTetromino.dir = up;
  curTetromino.type = (Type)dist(gen);
}

void State::clearRows() {
  int w = board.size() - 1;
  int cleared = 0;
  for (int r = (int)board.size() - 1; r >= 0; r--) {
    bool full = true;
    for (auto cell : board[r])
      full = full && cell;
    if (full) {
      cleared++;
      continue;
    }
    board[w--] = board[r];
  }
  for (; w >= 0; w--)
    board[w].fill(false);
  switch (cleared) {
  case 1:
    score += 100 * level;
    break;
  case 2:
    score += 300 * level;
    break;
  case 3:
    score += 500 * level;
    break;
  case 4:
    score += 800 * level;
    break;
  }
}

void State::advance() {
  for (int c = 0; c < TETRIMINO_SIZE; c++) {
    int col = curTetromino.x + SHAPES[curTetromino.type][curTetromino.dir][c][0];
    int row = curTetromino.y + SHAPES[curTetromino.type][curTetromino.dir][c][1];
    if (row < 0)
      gameOver = true;
    else if (col >= 0 && col < board[0].size() && row < (int)board.size())
      board[row][col] = true;
  }
  clearRows();
  if (!gameOver)
    spawnTetromino();
}

auto keyAvailable() -> bool {
  fd_set fds;
  FD_ZERO(&fds);
  FD_SET(0, &fds);
  timeval tv{0, 0};
  return select(1, &fds, nullptr, nullptr, &tv) > 0;
}

void doTheThing(State &state, std::ostream &out, std::istream &in) {
  RawTerminal raw;
  auto lastFall = std::chrono::steady_clock::now();
  while (!state.gameOver) {
    state.print(out);
    if (keyAvailable()) {
      char key;
      if (!in.read(&key, sizeof(char)))
        break;

      switch (key) {
      case 'h':
        state.move(Direction::left);
        break;

      case 'l':
        state.move(Direction::right);
        break;

      case 'j':
        state.move(Direction::down);
        break;

      case 'k':
        state.rotate();
        break;

      case 'q':
        state.gameOver = true;
        break;
      }
    }

    auto now = std::chrono::steady_clock::now();

    // TODO: state.level * modifier
    if (now - lastFall >= std::chrono::milliseconds(1000)) {
      // TODO: move block
      state.move(Direction::down);
      lastFall = now;
    }

    // TODO: state.level * modifier
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
  }
}

void initGame(std::ostream &out, std::istream &in) {
  State state{};
  doTheThing(state, out, in);
  // clear(out);
  out << "\nGame over\n";
}
