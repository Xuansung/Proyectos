#ifndef SAFEFD_H
#define SAFEFD_H

#include <unistd.h>

class SafeFD {
 public:
  SafeFD();
  explicit SafeFD(int fd);
  ~SafeFD();
  // Eliminar copia
  SafeFD(const SafeFD&) = delete;
  SafeFD& operator=(const SafeFD&) = delete;

  // Permitir movimientos
  SafeFD(SafeFD&& otro) noexcept;
  SafeFD& operator=(SafeFD&& otro) noexcept;

  [[nodiscard]] bool is_valid() const noexcept;
  [[nodiscard]] int getFD() const noexcept;
 private:
  int fd_;
};

#endif // SAFEFD_H