
#ifndef SAFEMAP_H
#define SAFEMAP_H

#include <string_view>
// Clase SafeMap
class SafeMap {
 public:
  SafeMap();
  SafeMap(void* mem, int tam);
  ~SafeMap();
  // Eliminar operaciones de copia
  SafeMap(const SafeMap&) = delete;
  SafeMap& operator=(const SafeMap&) = delete;

  // Permitir movimientos
  SafeMap(SafeMap&& otro) noexcept;
  SafeMap& operator=(SafeMap&& otro) noexcept;

  [[nodiscard]] std::string_view getSV() const noexcept;

 private:
  std::string_view sv_;
  void* mem_;
  int tam_;
};

#endif // SAFEMAP_H