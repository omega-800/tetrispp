#ifndef PRINT_HPP_
#define PRINT_HPP_

#include <iostream>

#include "Game.hpp"

auto getChar(std::istream &in) -> char;
void clear(std::ostream &out);

class RawTerminal {
  struct termios *saved_{nullptr};

public:
  RawTerminal();
  ~RawTerminal();
};

#endif
