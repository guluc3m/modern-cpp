// Primer ejemplo de serialización. Genera un archivo `my-file.bin' con los
// valores 42 (int), 42.0 (float), "hola" (string) serializados.
// AUTOR: José Antonio Verde Jiménez
// FECHA: 2024-09-12

#include <fstream>
#include <iostream>

constexpr int world_solution = 42;

int main () {
   std::ofstream file{"my-file.bin", std::ios::binary};
   // También:
   //    std::ofstream file;
   //    file.open("my-file.bin", std::ios::binary);
   if (not file) {   // Comprobamos que se abrió bien.
      std::cerr << "No se pudo abrir el archivo\n";
      return -1;
   }
   int int_value = world_solution;
   auto float_value = static_cast<float>(world_solution);
   std::string string_value = "hola";
   // NOLINTNEXTLINE (cppcoreguidelines-pro-type-reinterpret-cast)
   file.write(reinterpret_cast<const char*>(&int_value), sizeof(int_value));
   // NOLINTNEXTLINE (cppcoreguidelines-pro-type-reinterpret-cast)
   file.write(reinterpret_cast<const char*>(&float_value), sizeof(float_value));
   file.write(string_value.data(), string_value.size());
   file.close();
   return 0;
}
