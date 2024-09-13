#include <iostream>
#include <fstream>
#include <gsl/gsl>

bool read (std::ifstream & file, int & value) {
  // NOLINTNEXTLINE (cppcoreguidelines-pro-type-reinterpret-cast)
  return static_cast<bool>(file.read(reinterpret_cast<char *>(&value), sizeof(value)));
}

int main (int argc, char * argv[]) {
  gsl::span<char *> args{argv, static_cast<std::size_t>(argc)};
  if (argc != 2) {
    std::cerr << "USO: " << args[0] << " fichero-de-entrada\n";
    return 1;
  }
  std::ifstream file{args[1], std::ios::binary};
  if (not file) {
    std::cerr << "Couldn't open `" << args[1] << "'\n";
    return 2;
  }
  int value{};
  while (read(file, value)) {
    read(file, value);
    std::cout << value << "\n";
  }
  file.close();
  return 0;
}
