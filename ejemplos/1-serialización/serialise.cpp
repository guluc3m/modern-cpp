// Este programa lee una lista de números enteros de entrada estándar y los
// almacena en un fichero serializados. El nombre del fichero es el primer y
// único argumento del programa.
//
// EJEMPLO DE USO:
//    $ mkdir build && cd build && cmake .. && make
//    $ ./serialise data.bin
//    1 2 3
//    4 5 6 7
//    8
//
//    <Ctrl-D>
//
// Si hacemos hexdump del archivo
//    $ hexdump data.bin
//    0000000 0001 0000 0002 0000 0003 0000 0004 0000
//    0000010 0005 0000 0006 0000 0007 0000 0008 0000
//    0000020
//
// Vemos que se han escrito los números enteros como secuencias de 4 bytes
// cada uno. `0001 0000' es como se escribe en «little-endian» el valor `1'.
//
// AUTOR: José Antonio Verde Jiménez
// FECHA: 2024-09-12

#include <fstream>
#include <iostream>
#include <gsl/gsl>

// Separamos la función write. Tenemos que añadir el NOLINTNEXTLINE para que
// clang-tidy no se queje. La cosa es que cuando hagamos esto tenemos que
// justificar por qué lo hacemos. En este caso se justifica diciendo la función
// miembro file.write pide como primer parámetro un puntero a una secuencia
// de chars (bytes). El `reinterpret_cast<char const*>' lo que hace es
// reinterpretar la dirección de value de tipo `int*' como si fuera una
// secuencia de bytes `const char*'.
auto write (std::ofstream & file, int value) {
  // NOLINTNEXTLINE (cppcoreguidelines-pro-type-reinterpret-cast)
  file.write(reinterpret_cast<char const *>(&value), sizeof(value));
}

int main (int argc, char * argv[]) {
  // Para que no se queje clang-tidy, guardamos argv en un span. Que es una
  // vista sobre una secuencia.
  gsl::span<char *> args{argv, static_cast<std::size_t>(argc)};
  if (argc != 2) {
    std::cerr << "USO: " << args[0] << " fichero-de-salida\n";
    return 1;
  }
  // Abre el archivo en modo binario y comprueba si se ha abierto bien.
  std::ofstream file{args[1], std::ios::binary};
  if (not file) {
    std::cerr << "Couldn't open `" << args[1] << "'\n";
    return 2;
  }
  // Lee números de la entrada estándar y almacénalos en el archivo.
  int number = 0;
  while (std::cin >> number) {
    write(file, number);
  }
  file.close();
  return 0;
}
