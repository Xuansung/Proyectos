//Librería
#include "safefd.h"

SafeFD::SafeFD() : fd_(-1) {}

SafeFD::SafeFD(int fd) : fd_(fd) {}

SafeFD::~SafeFD() {
  if (fd_ >= 0) {
    close(fd_);
  }
}

SafeFD::SafeFD(SafeFD&& otro) noexcept : fd_(otro.fd_) {
  otro.fd_ = -1;
}

SafeFD& SafeFD::operator=(SafeFD&& otro) noexcept {
  if (this != &otro) {
    if (fd_ >= 0) {
      close(fd_);
    }
    fd_ = otro.fd_;
    otro.fd_ = -1;
  }
  return *this;
}

bool SafeFD::is_valid() const noexcept {
  return fd_ >= 0;
}

int SafeFD::getFD() const noexcept {
  return fd_;
}