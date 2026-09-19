#include "Print.hpp"

#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>

#include <stdio.h>
#include <termios.h>

RawTerminal::RawTerminal() {
  termios saved;
  if (tcgetattr(0, &saved) != 0)
    return;
  termios raw = saved;
  raw.c_lflag &= ~(ICANON | ECHO);
  raw.c_cc[VMIN] = 1;
  raw.c_cc[VTIME] = 0;
  if (tcsetattr(0, TCSANOW, &raw) != 0)
    return;
  saved_ = new termios(saved);
}

RawTerminal::~RawTerminal() {
  if (saved_ == nullptr)
    return;
  tcsetattr(0, TCSANOW, saved_);
  delete saved_;
}

void clear(std::ostream &out) { out << "\x1b[2J\x1b[H"; }

auto getChar(std::istream &in) -> char {
  char c;
  std::streambuf *pbuf = in.rdbuf();
  if (pbuf->sgetc() == EOF)
    std::exit(0);
  c = pbuf->sbumpc();
  return c;
}
