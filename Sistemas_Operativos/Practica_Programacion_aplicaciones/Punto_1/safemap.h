
#ifndef SAFEMAP_H
#define SAFEMAP_H

#include <string_view>
// Clase SafeMap
class SafeMap {
 public:
  SafeMap();
  explicit SafeMap(std::string_view sv);

  // Eliminar operaciones de copia
  SafeMap(const SafeMap&) = delete;
  SafeMap& operator=(const SafeMap&) = delete;

  // Permitir movimientos
  SafeMap(SafeMap&& otro) noexcept;
  SafeMap& operator=(SafeMap&& otro) noexcept;

  ~SafeMap();

  std::string_view getSV() const;

  explicit operator bool() const;
  
  void reset(std::string_view sv = {});

 private:
  std::string_view sv_;

};

#endif // SAFEMAP_H