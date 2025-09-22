// Librería
#include "safemap.h"
#include <sys/mman.h> // Para munmap

SafeMap::SafeMap() : mem_{nullptr}, tam_{0}, sv_{} {}

SafeMap::SafeMap(void* mem, int tam) : mem_{mem}, tam_{tam}, sv_{static_cast<const char*>(mem), tam} {}

SafeMap::~SafeMap() {
  if (mem_) {
    munmap(mem_, tam_);
  }
}

SafeMap::SafeMap(SafeMap&& otro) noexcept : mem_{otro.mem_}, tam_{otro.tam_}, sv_{otro.sv_} {
  otro.mem_ = nullptr;
  otro.tam_ = 0;
  otro.sv_ = {};
}

SafeMap& SafeMap::operator=(SafeMap&& otro) noexcept {
  if (this != &otro) {
    if (mem_) {
      munmap(mem_, tam_);
    }
    mem_ = otro.mem_;
    tam_ = otro.tam_;
    sv_ = otro.sv_;
    otro.mem_ = nullptr;
    otro.tam_ = 0;
    otro.sv_ = {};
  }
  return *this;
}

std::string_view SafeMap::getSV() const noexcept{
  return sv_;
}

