// Librería
#include "safemap.h"
#include <sys/mman.h> // Para munmap

SafeMap::SafeMap() : sv_{} {}

SafeMap::SafeMap(std::string_view sv) : sv_{sv} {}

SafeMap::SafeMap(SafeMap&& otro) noexcept : sv_{otro.sv_} {
  otro.sv_ = {};
}

SafeMap& SafeMap::operator=(SafeMap&& otro) noexcept {
  if (this != &otro) {
    if (!sv_.empty()) {
      munmap((void*)sv_.data(), sv_.size());
    }
    sv_ = otro.sv_;
    otro.sv_ = {};
  }
  return *this;
}

SafeMap::~SafeMap() {
  if (!sv_.empty()) {
    munmap((void*)sv_.data(), sv_.size());
  }
}

std::string_view SafeMap::getSV() const {
  return sv_;
}

SafeMap::operator bool() const {
  return !sv_.empty();
}

void SafeMap::reset(std::string_view sv) {
  if (!sv_.empty()) {
    munmap((void*)sv_.data(), sv_.size());
  }
  sv_ = sv;
}
