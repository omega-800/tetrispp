#include "Game.hpp"
#include "Print.hpp"

#include <chrono>
#include <iostream>
#include <sys/select.h>
#include <thread>

State::State() : board{} { }

void State::print(std::ostream &out) {
  clear(out);
  for (auto &line : board) {
    for (auto cell : line)
      out << (cell ? "XX" : "__") << " ";
    out << '\n';
  }
}

auto keyAvailable() -> bool {
  fd_set fds;
  FD_ZERO(&fds);
  FD_SET(0, &fds);
  timeval tv{0, 0};
  return select(1, &fds, nullptr, nullptr, &tv) > 0;
}

void doTheThing(State & state, std::ostream &out, std::istream &in) {
  RawTerminal raw;
  bool running = true;
  auto lastFall = std::chrono::steady_clock::now();
  while (running) {
    if (keyAvailable()) {
      char key;
      if (!in.read(&key, sizeof(char)))
        break;

      switch (key) {
      case 'h':
        // Move left
        break;

      case 'l':
        // Move right
        break;

      case 'j':
        // Soft drop
        break;

      case 'k':
        // Rotate
        break;

      case 'q':
        running = false;
        break;
      }
    }

    auto now = std::chrono::steady_clock::now();

    if (now - lastFall >= std::chrono::milliseconds(500)) {
      // TODO: move block
      state.print(out); 
      lastFall = now;
    }

    std::this_thread::sleep_for(std::chrono::milliseconds(10));
  }
}

void initGame(std::ostream &out, std::istream &in) {
  State state{};
  doTheThing( state, out, in); 
  // clear(out);
  out << "\nGame over\n";
}
