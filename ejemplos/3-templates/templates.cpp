#include <iostream>
#include <fstream>

template <typename T>
void write (std::ostream & out, T const & value) {
  // NOLINTNEXTLINE (cppcoreguidelines-pro-type-reinterpret-cast)
  out.write(reinterpret_cast<const char*>(&value), sizeof(T));
}

// Podemos especializar para std::string
void write (std::ostream & out, std::string const & value) {
  out.write(value.data(), static_cast<std::streamsize>(value.size()));
}

template <typename T>
bool read (std::istream & in, T & value) {
  // NOLINTNEXTLINE (cppcoreguidelines-pro-type-reinterpret-cast)
  return static_cast<bool>(in.read(reinterpret_cast<char *>(&value), sizeof(T)));
}

bool read (std::istream & in, std::string & str, int length) {
  str = std::string(length, 0);
  return static_cast<bool>(in.read(str.data(), length));
}

constexpr int solution{42};

bool write_data () {
  // Write
  std::ofstream output_file{"data.bin", std::ios::binary};
  if (not output_file) {
    std::cerr << "No se pudo crear data.bin\n";
    return false;
  }
  write(output_file, solution);
  write(output_file, static_cast<float>(solution));
  write(output_file, std::string{"hola"});
  output_file.close();
  return true;
}

bool read_data () {
  // Read
  std::ifstream input_file{"data.bin", std::ios::binary};
  if (not input_file) {
    std::cerr << "No se pudo abrir data.bin\n";
    return false;
  }
  int int_value{};
  float float_value{};
  std::string string_value{};
  if (not read(input_file, int_value) or not read(input_file, float_value)
      or not read(input_file, string_value, 4))
  {
    std::cerr << "No se pudo leer los contenidos de data.bin\n";
    return false;
  }
  std::cout << int_value << "\n" << float_value << "\n" << string_value << "\n";
  input_file.close();
  return true;
}

int main () {
  return (write_data() and read_data()) ? 0 : 1;
}
