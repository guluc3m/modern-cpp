---
marp: true
title: Modern C++
theme: default
paginate: true
size: 4:3
math: mathjax
style: |
    img[alt~="center"] {
        display: block;
        margin: 0 auto;
    }
    .columns {
        display: grid;
        grid-template-columns: repeat(2, minmax(0, 1fr));
        gap: 1rem;
    }
---

<!-- Instalar clang-tidy y clang-format -->
<!-- readability-identifier-length -->
<!-- argc y argv -->


<!-- _paginate: skip -->
![bg contain opacity:.15](img/gul_logo.svg)
# Introducción a C++ moderno
Por Luis Daniel Casais Mezquida y  
Jose Antonio Verde Jiménez

<br>

_Grupo de Usuarios de Linux_
[@guluc3m](https://twitter.com/guluc3m) | [gul.uc3m.es](https://gul.uc3m.es)

---

## Transparencias
![w:400 center](img/qrcode-transparencias.svg)
<center>
<a href="https://github.com/guluc3m/modern-cpp">github.com/guluc3m/modern-cpp</a>
</center>

---
## C++ Moderno

<!-- TODO: Mejorar estas dos partes -->

- C++ 17 en adelante
- C++ es más que «C con cosas»
- Cuanto menos C mejor
  - Más seguro
  - Más rápido
  - Mejores abstracciones
  - Fuertemente tipado
  - Programación funcional
  - Más más moderno
  - Los punteros no son necesarios
- C++ trae mejoras de _type safety_
- Usar las cosas nuevas es _opt-in_

---

### Estándares
Especificaciones de una versión del lenguaje.
- Documentación en [cppreference.com](https://en.cppreference.com)
- Los compiladores son los encargados de implementarlos
    - Nadie les "obliga"
    - Puedes ver la compatibilidad en [cppreference](https://en.cppreference.com/w/cpp/compiler_support).
- Uno nuevo cada 3 años
    - Actualmente: C++23

Se considera "C++ moderno" a partir de C++11.


---
### CppCoreGuidelines

- Guía de codificación
  - Más seguro
  - Pilla más errores a la hora de compilar
  - Por el mismísmo e inigualable **Bjarne Stroustrup**
- `gsl::span` vs. `std::span`.

---
### clang-tidy

- Herramienta de clang
  - Comprueba que el código se adhiera a una guía de codificación
  - Permite capturar muchos errores en tiempo de compilación
  - Homogeniza el código escrito por varias personas
- Añade una capa más de seguridad sobre C++
- Cualquier proyecto que se respete debe tener una guía de codificación.

---
### clang-format

- Aplica un formato al código
- Todo el código fuente tiene la misma estructura
- Si no, cada uno escribe el código como le parezca:

```cpp
   bool is_prime(int x){for(int i=2;i*i<=x;++i){
if(x%i==0)return false;} return true;}
 std::vector<int> prime_numbers ( int   from,   int to )
{ std::vector<int>   result
; for (   int i = from ;    i < to; ++i)
  { if (is_prime(i)) {    result.push_back(i)   }
  }
; return result;
}
```

---

```cpp
bool is_prime (int x) {
   for (int i = 2; i * i <= x; ++i) {
      if (x % i == 0) { return true; }
   }
   return false;
}

std::vector<int> prime_numbers (int from, int to) {
   std::vector<int> result;
   for (int i = from; i < to; ++i) {
      if (is_prime(i)) { result.push_back(i); }
   }
   return result;
}
```

---
## Entrada y Salida

 - Hay dos clases
   - `std::istream` (Flujo de entrada)
   - `std::ostream` (Flujo de salida)
 - Por defecto se utilizan para leer y escribir texto:

```cpp
#include <iostream>  // Incluye flujos de entrada y salida (I/O Stream)
#include <string>    // Contiene el tipo std::string

int main () {
   // Redirige la cadena "¿Cuántos años tienes?\n" al flujo de
   // salida estándar `std::cout'.
   std::cout << "¿Cuántos años tienes?\n";
   std::string age;
   // Lee la edad en la variable `age' de entrada estándar.
   std::cin >> age;
   // Se pueden imprimir varios tipos:
   std::cout << "Tienes " << age << " años\n";
   return 0;
}
```

---
## Entrada y Salida binaria
Los objetos en memoria se almacenan como una secuencia de bits. Por ejemplo:

```cpp
int x = 42;
```

Depende del computador:

- _little-endian_:  `2A 00 00 00`
- _big-endian_: `00 00 00 2A`

(Vamos a suponer _little-endian_)


---
Por ejemplo un número de coma flotante de simple precisión se representa en memoria en base al estándar IEEE 754.

```cpp
float x = 42.0;
```

- En IEEE 754 en hexadecimal es: 42280000<sub>16</sub>
- En memoria se representa como: `00 00 28 42`

De igual manera la cadena `"hola"` se representa como la secuencia de los bytes `'h'`, `'o'`, `'l'`, `'a'`. O, en hexadecimal:

- `68 6f 6c 61`

---

En un archivo sabemos escribir cadenas.

```cpp
#include <fstream>

int main () {
   std::ofstream my_output{"my-file.txt"};
   my_output << 42     << "\n"
             << 42.0   << "\n"
             << "hola" << "\n";
   my_output.close();
   return 0;
}
```

El archivo queda:

```plain
42
42.0
hola
```

¿Y si en vez de escribir cadenas, escribimos los bytes _a pelo_?

---

### Casts
#### `reinterpret_cast`
`reinterpret_cast` nos permite convertir entre punteros de distintos tipos.

- En C:

```c
#include <stdio.h>
int main () {
   float x = 42.0;
   float *p_x = &x;
   // `p_y' apunta a la misma dirección que `p_x' (que es `x').
   // Pero para `p_x' la dirección es de tipo `float'.
   // Y para `p_y' la dirección es de tipo `unsigned int'
   unsigned *p_y = (unsigned*)p_x;
   unsigned y = *p_y;
   printf("0x%08X\n", y); // Imprime 0x42280000
}
```

- Sin embargo es **completamente inseguro**.

---

- En C++, está `reinterpret_cast` que hace lo mismo.
- Los casts en C++ destacan más.

```cpp
#include <iostream>
int main () {
   float x = 42.0;
   float *p_x = &x;
   unsigned *p_y = reinterpret_cast<unsigned*>(p_x);
   unsigned y = *p_y;
   std::cout << std::hex << std::uppercase << x << "\n";
}
```

- Hay que tener mucho cuidado
  - Hay requisitos de alineamiento
  - Hay requisitos de tamaño
  - No se puede utilizar con todos los tipos.
  - CppCoreGuidelines lo prohíbe
  - Se debe justificar su uso

---
#### `static_cast`
- Hace una conversión «real» entre tipos.
   - `static_cast<int>(42.3)` devuelve `42`.
   - `static_cast<float>(32)` devuelve `32.0`.
- Si existe un operador de conversión, se utiliza:
   - `static_cast<bool>(my_file)` devuelve o `true` o `false`.
- Al igual que `reinterpret_cast`, destaca.
- Hay que tener cuidado con ciertas conversiones (clang-tidy ayuda).
```cpp
static_cast<unsigned>(-3);          // No se puede representar
static_cast<float>(33'554'432);     // Pierde, precisión 25 vs 23 bits
static_cast<int>(1.2E100);          // Demasiado grande
static_cast<int>(7'000'000'000L);   // Demasiado grande
```

---
### Salida binaria en C++

Empezamos abriendo el archivo:
- Con constructor
- Con función miembro
```cpp
#include <fstream>
#include <iostream>

int main () {
std::ofstream file{"my-file.bin", std::ios::binary};
// También:
//    std::ofstream file;
//    file.open("my-file.bin", std::ios::binary);
if (not file) {   // Comprobamos que se abrió bien.
   std::cerr << "No se pudo abrir el archivo\n";
   return -1;
}
```

---

Ahora que tenemos el archivo abierto, podemos escribir distintos valores:

```cpp
int int_value = 42; // ¡Número mágico!
float float_value = 42.0;
std::string string_value = "hola";
file.write(reinterpret_cast<const char*>(&int_value), sizeof(int_value));
file.write(reinterpret_cast<const char*>(&float_value), sizeof(float_value));
file.write(string_value.data(), string_value.size()); // ¡Conversión implícita!
```

La función miembro `std::ostream write` pide dos parámetros de tipos:
 - `const char *`
 - `std::size_t`

De manera similar a la función `fwrite' en C.

_Los errores del clang-tidy están solucionados en el código de ejemplo, se explica más adelante cómo solucionarlos_.

---

En este caso está justificado el uso del `reinterpret_cast`:
 - Solo existe una función del tipo `write`
   - Que pide un puntero a una cadena de carácteres (`const char *`)
   - Y el tamaño de la cadena (`std::size_t`).
 - No se puede hacer de ninguna otra forma.

Así que lo comentamos:

```cpp
   // Justificamos por qué hemos silenciado al clang-tidy para la siguiente
   // línea de código. En principio, en lectura y escritura es donde únicamente
   // está justificado callar al clang-tidy. En otros casos, tiene que estar
   // MUY justificado. Y se añade a la memoria de cara a la práctica.
   // NOLINTNEXTLINE (cppcoreguidelines-pro-type-reinterpret-cast)
   file.write(reinterpret_cast<const char*>(&int_value), sizeof(int_value));
   // NOLINTNEXTLINE (cppcoreguidelines-pro-type-reinterpret-cast)
   file.write(reinterpret_cast<const char*>(&float_value), sizeof(float_value));
   file.write(string_value.data(), string_value.size());
```

---
Hay un ejemplo de justificación en (`ejemplos/1-serialización`)

Terminamos cerrando el archivo.

```cpp
   file.close();
   return 0;
}
```

Si ejecutamos el programa anterior (`ejemplos/0-serialización`).
 - Obtenemos un archivo `my-file.bin`
 - Si lo leemos con `hexdump`

> hexdump -C my-file.bin

```plain
00000000  2a 00 00 00 00 00 28 42  68 6f 6c 61              |*.....(Bhola|
0000000c
```

---
### Entrada binaria en C++
Es similar a la entrada:
 - `std::ifstream` en vez de `std::ofstream`.
 - `.read()` en vez de `.write()`.

```cpp
   // ejemplos/2.deserialización
   std::ifstream file{"my-file.bin", std::ios::binary};
   if (not file) {
      std::cerr << "No se pudo abrir my-file.bin en modo lectura\n";
      return 1;
   }
   int int_value;
   float float_value;
   std::string string_value = "    ";
   // NOLINTNEXTLINE (cppcoreguidelines-pro-type-reinterpret-cast)
   if (not file.read(reinterpret_cast<char*>(&int_value), sizeof(int_value))) {
      std::cerr << "No se pudo leer el entero\n";
      return 2;
   }
   // NOLINTNEXTLINE (cppcoreguidelines-pro-type-reinterpret-cast)
   if (not file.read(reinterpret_cast<char*>(&float_value), sizeof(float_value))) {
      std::cerr << "No se pudo leer el float\n";
      return 2;
   }
   if (not file.read(string_value.data(), string_value.size())) {
      return 2;
   }
```
---
## STL y cppreference
<!-- TODO -->

---
### Templates
 - Una _template_ es una plantilla.

<div class="columns">
<div>

```cpp
void print_square (int x) {
   std::cout << x * x << std::endl;
}

void print_square (long x) {
   std::cout << x * x << std::endl;
}
```

</div>
<div>

A veces nos encontramos con que repetimos la misma función para distintos tipos.

```cpp
print_square(2);   // Funciona, es `int'
print_square(2L);  // Funciona, es `long'
print_square(2.0); // NO FUNCIONA, es `double'
```

</div>
</div>

Para ello usamos templates

---

```cpp
template <typename T>
void print_square (T x) {
   std::cout << x * x << std::endl;
}
```

 - La `T` se sustituye por el tipo que le pasemos.
   - `print_square<int>(10)`
   - `print_square(10)`, se deduce el tipo.
 - Si para un tipo no se define alguna función, falla.
 - `print_square<float>` es equivalente a la función:

```cpp
// T = float
void print_square (float x) {
   std::cout << x * x << std::endl;
}
```

---

- También sirve para estructuras de datos:
 - `std::vector<int>`
 - `std::vector<my_type>`...
 - `std::vector<std::vector<std::string>>`
- Todas las funciones de la clase se definen para el tipo del _template_
- En C, tendríamos que usar `void*` o definirlo para cada posible tipo. Y para cada posible operación.

---

Podemos generalizar la función de lectura y escritura de antes:

```cpp
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
```
---

Y ya lo podemos usar en cualquier momento.

```cpp
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
```

---

<center><h1><b>STL</b></h1><br/>
<b>Standard Template Library</b>
</center>

---

<center> <link href=https://en.cppreference.com/w/>https://en.cppreference.com/w/</link></center>

---

### Estructuras de datos
#### `std::vector`

Constructores:
```cpp
constexpr inf = std::numeric_limits<int>::max();
std::vector<float> mis_notas{8.5, 7.6, 0.5, 9.1};
std::vector<float> tus_notas(4, 0.0);  // Vector con cuatro ceros.
std::vector<std::vector<int>> vuelos {
   {  0, inf,   1,   4,   2}
   {inf,   0,   3,   4,   3}
   {  1,   3,   0, inf,   3}
   {  2,   8,   2,   0,   1}
   {  3,   2,   1,   4,   0}};
std::vector<std::string> ciudades{"Madrid", "Nueva York", "París", "Roma", "Tokio"};
for (auto const & ciudad : ciudades)
   std::cout << ciudad << "\n";
```

Funciones Miembro
 - `.push_back()` y `.emplace_back()`
 - `.reserve()`
 - `operator[]`
 - Más en cppreference

---

#### `std::tuple` y `std::pair`

`std::pair` es una `std::tuple` de dos elementos. Y tiene los miembros:
 - `.first`
 - `.second`
```cpp
using mi_tipo = std::tuple<std::string, int, double>;
mi_tipo mi_objeto = mi_tipo{"hola", 42, 42.0};
// Descomponer
auto [str_v, int_v, flt_v] = mi_objeto;
// También
auto str_v = std::get<1>(mi_objeto);
auto int_v = std::get<2>(mi_objeto);
auto flt_v = std::get<3>(mi_objeto);
// Con vectors
std::vector<std::pair<std::string, int>> ciudades {
   {"Madrid", 1}, {"Nueva York", 2}, {"París", 3}, {"Roma", 4}, {"Tokio", 5}};
// Descomponer al iterar
for (auto const & [nombre, id] : ciudades) {
   std::cout << nombre << " " << id "\n";
}
```

---

#### `std::map` y `std::unordered_map`

Como los diccionarios de Python.

- `std::map`, árbol de búsqueda binario
- `std::unordered_map`, _hash map_

---

### Algoritmos
https://en.cppreference.com/w/cpp/algorithm
 - Ordenar, `std::sort(inicio, fin, predicado)`
 - Mapear, `std::transform(inicio, fin, salida, operación)`
 - Reducir, `std::accumulate(inicio, fin, primero, función)`
 - Filtrar, `std::copy_if(inicio, fin, salida, predicado)`

```cpp
bool mayor_a_menor (int x, int y) { return x > y; }
int cuadrado (int x) { return x * x; }
int producto (int a, int b) { return a * b; }
int main () {
   std::vector<int> valores{1, 3, 4, 5, 2};
   // Ordena los valores, haz el cuadrado de ellos, y multiplícalos.
   std::sort(valores.begin(), valores.end(), mayor_a_menor);
   std::transform(valores.begin(), valores.end(), valores.begin(), cuadrado);
   std::cout << std::accumulate(valores.begin(), valores.end(), 1, producto); << "\n";
   return 0;
}
```

---

### Funciones lambda y predicados
En vez de definir una las funciones por separado:

```cpp
bool mayor_a_menor (int x, int y) { return x > y; }
int cuadrado (int x) { return x * x; }
int producto (int a, int b) { return a * b; }
```

Podemos usar **funciones lambda**, o funciones anónimas

```cpp
std::sort(valores.begin(), valores.end(),
   [](int a, int b){ return a > b; });
std::transform(valores.begin(), valores.end(), valores.begin(),
   [](int x){ return x * x });
std::cout << std::accumulate(valores.begin(), valores.end(), 1,
   [](int a, int b) { return a * b; }); << "\n";
```

---

```cpp
[capturas] (parámetros) -> retorno {
   código
}
```
 - El retorno es opcional si el compilador lo puede deducir.
 - Las capturas son más complicadas.
   - Su propósito es cómo se tratan las variables que se referencian en el código

```cpp
std::vector<int> filtrar_menores_de (std::vector<int> const & v, int n) {
   std::vector<int> resultado;
   std::copy_if(v.begin, v.end(), std::back_inserter(resultado),
      [=] (int x) {
         // n se referencia dentro de la función [=] dice que se copie.
         return x < n;
      });
   return resultado;
}
```

---

## Paso por Valor y Referencia

- `int x`: Copia x
- `const int x`: Copia x, no se puede modificar
- `int & x`: Referencia a x, mutable.
- `int const & x`: Referencia constante a x, inmutable
- `int && x`: Paso por movimiento.

Depende del tipo, se pasa por valor o referencia constante:
- Si es pequeño (`int`, `char`, `long`, `my_vector3`)
  - Por copia
- Si es grande, o se tienen que copiar muchos datos (`std::vector`, `std::string`)
  - Por referencia constante

**NO SE USAN PUNTEROS**

---

## Estructuración, Espacios de Nombre

---

### namespace
Los espacios de nombre nos permiten agrupar funciones y clases por
funcionalidad.

```cpp
namespace mates {
   constexpr float π = 3.1415'9265'35;
   int cuadrado (int x);
   int raiz (int x);
}
```

- Se puede llamar desde fuera como `mates::cuadrado`.
- Se puede incluir todos los elementos del espacio de nombres.

```cpp
int pitagoras (int x, int y) {
   using namespace mates;
   return raiz(cuadrado(x) + cuadrado(y));                                       
}
```

---

### .hpp y .cpp

- En el `.hpp` van las declaraciones. En el `.cpp` va la implementación.
- Puede haber definiciones, por defecto, son _inline_.
- Al igual que dentro de una clase.

```cpp
class Vector3 {
   public:
      // Son inline
      Vector3(int x, int y, int z) { x_ = x; y_ = y; z_ = z; }
      Vector3 operator+ (Vector3 rhs) { return {x_ + rhs.x_, y_ + rhs.y_,
                                                z_ + rhs.z_}; }
   private:
      int x_, y_, z_;
}
```

---

### constexpr

- Son funciones que, por lo general, se calculan en tiempo de compilación. Si es posible.

```cpp
constexpr bool is_prime (int n) {
   for (int i = 2; i * i <= n; ++i) {
      if (n % i == 0) { return false; }
   }
   return true;
}

template <int length>
constexpr auto calc_primes () {
   std::array<int, length> result;
   int num = 2;
   for (int i = 0; i < length; num++) {
      if (is_prime(num)) { result[i++] = num; }
      else { num; }
   }
   return result;
}
```
