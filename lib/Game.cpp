#include "Game.hpp"
#include "Print.hpp"
#include "Shapes.hpp"

#include <chrono>
#include <iostream>
#include <random>
#include <sys/select.h>
#include <thread>

State::State() : board{} { advance(); }

void State::print(std::ostream &out) {
  clear(out);
  out << "Move: h, l         Advance: j" << "\n";
  out << "Rotate: k          Quit: q" << "\n";
  out << "Score: " << score << "\n";
  auto view = board;
  auto rot = (curTetromino.dir + 3) % 4;
  for (int c = 0; c < 4; c++) {
    int col = curTetromino.x + SHAPES[curTetromino.type][rot][c][0];
    int row = curTetromino.y + SHAPES[curTetromino.type][rot][c][1];
    if (col >= 0 && col < board[0].size() && row >= 0 && row < board.size())
      view[row][col] = true;
  }
  for (auto &line : view) {
    for (auto cell : line)
      out << (cell ? "XX" : "__") << " ";
    out << '\n';
  }
}

void State::drop() {
  if (curTetromino.y == board.size() - 1)
    advance();
  else
    curTetromino.y++;
}

void State::rotate() {
  curTetromino.dir = (Direction)((curTetromino.dir + 1) % 4);
}

void State::move(Direction dir) {
  if (dir != left && dir != right)
    return;
  auto rot = (curTetromino.dir + 3) % 4;
  int dx = (dir == right) ? 1 : -1;
  for (int c = 0; c < 4; c++) {
    int col = curTetromino.x + dx + SHAPES[curTetromino.type][rot][c][0];
    if (col < 0 || col >= (int)board[0].size())
      return;
  }
  curTetromino.x += dx;
}

void State::advance() {
  static std::mt19937 gen{std::random_device{}()};
  static std::uniform_int_distribution<int> dist{0, Type::count - 1};
  curTetromino.x = 4;
  curTetromino.y = 0;
  curTetromino.dir = up;
  curTetromino.type = (Type)dist(gen);
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
  bool running = true;
  auto lastFall = std::chrono::steady_clock::now();
  while (running) {
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
        state.drop();
        break;

      case 'k':
        state.rotate();
        break;

      case 'q':
        running = false;
        break;
      }
    }

    auto now = std::chrono::steady_clock::now();

    // TODO: state.level * modifier
    if (now - lastFall >= std::chrono::milliseconds(1000)) {
      // TODO: move block
      state.drop();
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
