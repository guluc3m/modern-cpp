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


<!-- _paginate: skip -->
![bg contain opacity:.15](img/gul_logo.svg)
# Introducción a C++ moderno
Por Jose Antonio Verde Jiménez y  
Luis Daniel Casais Mezquida


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
<!-- header: '' -->


- Más que «C con cosas»
   - Más rápido
   - Más seguro (memoria, tipado, ...)
   - Mejores abstracciones
   - Soporte para programación genérica y funcional
- Usar las cosas nuevas es _opt-in_

---
<!-- header: '**C++ Moderno**' -->

### PROHIBIDO
- _Raw pointers_ - `int* ptr;`
- _C arrays_ - `int[] arr;`
- Manipulación manual de memoria - `new` / `malloc()`
- Librerías de C - `#include <whatever.h>`

Todo esto hace que el código no sea seguro.


---
### Estándares
Especificaciones ISO de una versión del lenguaje.
- Documentación en [cppreference.com](https://en.cppreference.com)
- Los compiladores son los encargados de implementarlos
    - Nadie les "obliga"
    - Puedes ver la compatibilidad en [cppreference](https://en.cppreference.com/w/cpp/compiler_support)
- Uno nuevo cada 3 años
    - Actualmente: C++23 (Sept. 2024)

Se considera "C++ moderno" a partir de C++17.


---
### [CppCoreGuidelines](https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines)

- Guía de codificación de C++
  - Más seguro
  - Pilla más errores a la hora de compilar
  - Por el mismísmo e inigualable [Bjarne Stroustrup](https://www.stroustrup.com/)
- [`gsl::span`](https://github.com/microsoft/GSL/blob/main/docs/headers.md#user-content-H-span-span) vs. [`std::span`](https://en.cppreference.com/w/cpp/container/span)


---
### [clang-tidy](https://clang.llvm.org/extra/clang-tidy/)

- Herramienta de [clang]((https://clang.llvm.org/))
  - Comprueba que el código se adhiera a una guía de codificación
  - Permite capturar muchos errores en tiempo de compilación
  - Homogeneiza el código escrito por varias personas
- Añade una capa más de seguridad sobre C++
- Cualquier proyecto que se respete debe tener una guía de codificación


---
### [clang-format](https://clang.llvm.org/docs/ClangFormat.html)

- Aplica automáticamente un formato al código
- Todo el código fuente tiene la misma estructura

Si no, cada uno escribe el código como le parezca:

```cpp
   bool is_prime(int x){for(int i=2;i*i<=x;++i){
if(x%i==0)return false;} return true;}
 std::vector<int> prime_numbers ( int   from,   int to )
{ std::vector<int>   result
; for (   int i = from ;    i < to; ++i)
  { if (is_prime(i)) {    result.push_back(i)   }
  }
; return result;}
```

---

Con clang-format:
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

## Cosas nuevas
<!-- header: '' -->



### [`auto`](https://en.cppreference.com/w/cpp/language/auto)
Permite al compilador inferir el tipo de una variable.
```cpp
auto x = 1; // int
```

Útil al recorrer vectores:
```cpp
for (auto it = vector.begin(); it != vector.end(); ++it) {
    it->doSomething();
}
```

---
<!-- header: '**Cosas nuevas**' -->

### [`enum class`](https://en.cppreference.com/w/cpp/language/enum)
Evolución de `enum`, quitando ciertas limitaciones.

```cpp
enum class Color {
   Red,
   Green,
   Blue
};

auto col = Color::Red;
```


### [`using`](https://en.cppreference.com/w/cpp/language/using_declaration)
Evolución de `typedef`.

```cpp
using Address = std::uint32_t;
```

---
### [Ranged for loops](https://en.cppreference.com/w/cpp/language/range-for)
Una forma más elegante de recorrer contenedores.

```cpp
for (auto & elem : myVec) { }
```

Puedes desempaquetar valores:
```cpp
for (auto & [key, value] : myMap) { }
```

Recuerda usar `const` si no vas a modificar los elementos.


---

### [Excepciones](https://en.cppreference.com/w/cpp/language/exceptions)
Existen. Casi mejor que ni las toques.

Tienes `try`-`catch`:

```cpp
try {
   foo[index];
} catch (std::out_of_range &e) {
   throw std::exception("cosa mala")
}
```

---
### [Constructores](https://en.cppreference.com/w/cpp/language/constructor)
- Permiten inicializar los miembros antes de ejecutar el cuerpo (lista después de `:`).

E.g.:
```cpp
class Foo {
   public:
      Foo(int a, int b) : _a {a}, _b {b} { /* body */ }

   private:
      int _a;
      int _b;
}

auto bar = Foo {1, 2};
```


---

## Casting
<!-- header: '' -->

Consiste en convertir información de un tipo de dato a otro. E.g. `int` → `long`

En C:
```c
int foo = 69;
long var = (int) foo;
```

Sin embargo, esto es **completamente inseguro**.

C++ provee alternativas, las cuales _destacan_:
- [`reinterpret_cast`](https://en.cppreference.com/w/cpp/language/reinterpret_cast)
- [`static_cast`](https://en.cppreference.com/w/cpp/language/static_cast)


---
<!-- header: '**Casting**' -->

#### [`reinterpret_cast`](https://en.cppreference.com/w/cpp/language/reinterpret_cast)
Permite _reinterpretar_ los datos entre punteros de distintos tipos.

En C:

```c
float x = 42.0;
float *p_x = &x;

unsigned *p_y = (unsigned*)p_x;
// `p_y' apunta a la misma dirección que `p_x' (que es `x').
// Pero para `p_x' la dirección es de tipo `float',
// y para `p_y' la dirección es de tipo `unsigned int'

unsigned y = *p_y;
printf("0x%08X\n", y); // Imprime 0x42280000
```

---

En C++:

```cpp
float x = 42.0;
float *p_x = &x;

unsigned *p_y = reinterpret_cast<unsigned*>(p_x);

unsigned y = *p_y;
std::cout << std::hex << x << "\n"; // Imprime 42
```

Hay que tener mucho cuidado:
- Hay requisitos de tamaño y de alineamiento
- No se puede utilizar con todos los tipos
- [CppCoreGuidelines](https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines) **lo prohíbe** (se debe justificar su uso)

---

#### [`static_cast`](https://en.cppreference.com/w/cpp/language/static_cast)
Hace una conversión «real» entre tipos.
- `static_cast<int>(42.3)` devuelve `42`
- `static_cast<float>(32)` devuelve `32.0`

Si existe un operador de conversión, se utiliza:
- `static_cast<bool>(my_file)` devuelve o `true` o `false`

Hay que tener cuidado con ciertas conversiones ([clang-tidy](https://clang.llvm.org/extra/clang-tidy/) ayuda):
```cpp
static_cast<unsigned>(-3);          // No se puede representar
static_cast<float>(33'554'432);     // Pierde precisión
static_cast<int>(1.2E100);          // Demasiado grande
static_cast<int>(7'000'000'000L);   // Demasiado grande
```




---
## Entrada y Salida
<!-- header: '' -->

Hay dos clases principales:
   - [`std::istream`](https://en.cppreference.com/w/cpp/io/basic_istream) (Flujo de entrada)
   - [`std::ostream`](https://en.cppreference.com/w/cpp/io/basic_ostream) (Flujo de salida)

Por defecto se utilizan para leer y escribir texto.

Librerías dependiendo del uso:
- [`<iostream>`](https://en.cppreference.com/w/cpp/header/iostream): stdin/stdout
- [`<fstream>`](https://en.cppreference.com/w/cpp/header/fstream): Ficheros


---
<!-- header: '**Entrada y Salida**' -->

```cpp
#include <iostream>  // Incluye flujos de entrada y salida
                     // (I/O Stream)
#include <string>    // Contiene el tipo std::string

int main () {
   // Redirige la cadena "¿Cuántos años tienes?\n" al flujo
   // de salida estándar `std::cout'.
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
### Entrada y Salida binaria
Los objetos en memoria se almacenan como una secuencia de bytes.

Por ejemplo:
```cpp
int x = 42;
```

Dependiendo del computador, puede ser:

- _little-endian_: `2A 00 00 00`
- _big-endian_: `00 00 00 2A`

(Vamos a suponer _little-endian_)


---
Un número de coma flotante de simple precisión se representa en memoria en base al estándar IEEE 754.
```cpp
float x = 42.0;
```

- En hexadecimal es 42280000<sub>16</sub>
- En memoria se representa como `00 00 28 42`

De igual manera la cadena `"hola"` se representa como la secuencia bytes `'h'`, `'o'`, `'l'`, `'a'`.
- En memoria, `68 6f 6c 61`

---

En un archivo sabemos escribir _strings_:

```cpp
std::ofstream my_output{"my-file.txt"};

my_output << 42     << "\n"
          << 42.0   << "\n"
          << "hola" << "\n";

my_output.close();
```

Lo que resulta en `my-file.txt`:
```plain
42
42.0
hola
```

¿Y si en vez de escribir cadenas, escribimos los bytes _a pelo_?



---

### Salida binaria en C++

Empezamos abriendo el archivo, de forma binaria:
```cpp
#include <fstream>
#include <iostream>
```

```cpp
std::ofstream file{"my-file.bin", std::ios::binary};
// También:
//    std::ofstream file;
//    file.open("my-file.bin", std::ios::binary);

if (not file) {   // Comprobamos que se abrió bien
   std::cerr << "No se pudo abrir el archivo\n";
   return -1;
}
```

---

Ahora podemos escribir distintos valores:

```cpp
int int_value = 42; // ¡Número mágico!
float float_value = 42.0;
std::string string_value = "hola";

file.write(
   reinterpret_cast<const char*>(&int_value),
   sizeof(int_value)
); // ¡Reinterpret cast!
file.write(
   reinterpret_cast<const char*>(&float_value),
   sizeof(float_value)
); // ¡Reinterpret cast!
file.write(
   string_value.data(),
   string_value.size()
); // Conversión implícita
```

_peeeero..._

---

**¡Clang-tidy se queja!**

La función miembro [`std::ostream::write`](https://en.cppreference.com/w/cpp/io/basic_ostream/write) pide:
- Un puntero a una cadena de carácteres (`const char *`)
- El tamaño de la cadena ([`std::size_t`](https://en.cppreference.com/w/cpp/types/size_t)).

(similar a la función [`fwrite`](https://en.cppreference.com/w/c/io/fwrite) en C)

En este caso está justificado el uso del [`reinterpret_cast`](https://en.cppreference.com/w/cpp/language/reinterpret_cast):
- No se puede hacer de ninguna otra forma
- Hay que silenciar el clang-tidy (con `NOLINTNEXTLINE`), justificándolo

---

```cpp
// NOLINTNEXTLINE (cppcoreguidelines-pro-type-reinterpret-cast)
file.write(
   reinterpret_cast<const char*>(&int_value),
   sizeof(int_value)
);

// NOLINTNEXTLINE (cppcoreguidelines-pro-type-reinterpret-cast)
file.write(
   reinterpret_cast<const char*>(&float_value),
   sizeof(float_value)
);

file.write(string_value.data(), string_value.size());

file.close(); // ¡Recordad cerrar el archivo!
```

- Sólo está justificado silenciarlo en lectura y escritura
- En cualquier otro caso, tiene que estar MUY justificado

Ejemplo completo en [`ejemplos/0-serialización`](https://github.com/guluc3m/modern-cpp/blob/main/ejemplos/0-serializaci%C3%B3n/serialise.cpp).

---

Si ejecutamos el programa anterior obtenemos un archivo `my-file.bin`.

Podemos leerlo con [`hexdump`](https://man7.org/linux/man-pages/man1/hexdump.1.html):

```bash
$ hexdump -C my-file.bin
```

```plain
00000000  2a 00 00 00 00 00 28 42  68 6f 6c 61              |*.....(Bhola|
0000000c
```


---
### Entrada binaria en C++
Es similar a la salida:
- [`std::ifstream`](https://en.cppreference.com/w/cpp/io/basic_ifstream) en vez de [`std::ofstream`](https://en.cppreference.com/w/cpp/io/basic_ofstream)
- [`.read()`](https://en.cppreference.com/w/cpp/io/basic_istream/read) en vez de [`.write()`](https://en.cppreference.com/w/cpp/io/basic_oftream/write)

<br/>

Recordad manejar los errores, y cerrar el archivo.

---

```cpp
std::ifstream file{"my-file.bin", std::ios::binary};
if (not file) { /* ... */ }

float float_value;

if (
   not file.read(
      reinterpret_cast<char*>(&float_value),
      sizeof(float_value)
   )
) { /* ... */ }

file.close();
```

Ejemplo completo en [`ejemplos/2-deserialización`](https://github.com/guluc3m/modern-cpp/blob/main/ejemplos/2-deserializaci%C3%B3n/deserialise.cpp).



---
<!-- header: '' -->

## Standard Template Library
Conjunto de contenedores y algoritmos genéricos para ayudar en la programación.


---
<!-- header: '**Standard Template Library**' -->

### Templates
A veces tenemos que implementar la misma función para distintos tipos:

```cpp
void print_square (int x) {
   std::cout << x * x << std::endl;
}

void print_square (long x) {
   std::cout << x * x << std::endl;
}
```
```cpp
print_square(2);   // Funciona, es `int'
print_square(2L);  // Funciona, es `long'
print_square(2.0); // NO FUNCIONA, es `double'
```


---

Podemos usar una «plantilla» asumiendo un tipo genérico:
```cpp
template <typename T>
void print_square (T x) {
   std::cout << x * x << std::endl;
}
```

 - La `T` se sustituye por el tipo que le pasemos.
   - `print_square<int>(10)`
   - También se deduce el tipo, e.g. `print_square(10)`
 - Si para un tipo no se define alguna función, falla


`print_square<float>` es equivalente a:

```cpp
void print_square (float x) {
   std::cout << x * x << std::endl;
}
```

---

Generalicemos las funciónes de lectura y escritura:

```cpp
template <typename T>
void write (std::ostream & out, T const & value) {
   out.write(
      reinterpret_cast<const char*>(&value),
      sizeof(T)
   );
}

// Especializamos para std::string
void write (std::ostream & out, std::string const & value) {
   out.write(
      value.data(),
      static_cast<std::streamsize>(value.size())
   );
}
```

---

```cpp
template <typename T>
bool read (std::istream & in, T & value) {
   return static_cast<bool>(
      in.read(reinterpret_cast<char *>(&value),
      sizeof(T))
   );
}

bool read (std::istream & in, std::string & str, int length) {
   str = std::string(length, 0);
   return static_cast<bool>(in.read(str.data(), length));
}
```
---

Y ya lo podemos usar en cualquier momento:

```cpp
constexpr int solution{42};
```

```cpp
std::ofstream output_file{"data.bin", std::ios::binary};
if (not output_file) { /* ... */ }

write(output_file, solution);
write(output_file, static_cast<float>(solution));
write(output_file, std::string{"hola"});

output_file.close();
```

---

```cpp
std::ifstream input_file{"data.bin", std::ios::binary};
if (not input_file) { /* ... */ }

int int_value{};
float float_value{};
std::string string_value{};

if (
   not read(input_file, int_value) or
   not read(input_file, float_value) or
   not read(input_file, string_value, 4))
{ /* ... */ }

std::cout << int_value    << "\n"
          << float_value  << "\n"
          << string_value << "\n";

input_file.close();
```

Ejemplo completo en [`ejemplos/3-templates`](https://github.com/guluc3m/modern-cpp/blob/main/ejemplos/3-templates/templates.cpp).

---

### Contenedores

Estructuras de datos genéricas.

Implementan dos funciones miembro básicas:
- `.size()`: Devuelve el tamaño de la estructura
- `.clear()`: Vacía la estructura

---

#### [`std::vector`](https://en.cppreference.com/w/cpp/container/vector)

Constructores:
```cpp
std::vector<float> tus_notas(3, 0.0);  // {0.0, 0.0, 0.0}

std::vector<std::string> ciudades {
   "Madrid",
   "Nueva York",
   "París"
};
```


Se pueden recorrer con [_ranged for loops_](https://en.cppreference.com/w/cpp/language/range-for):
```cpp
for (auto const & ciudad : ciudades) {
   std::cout << ciudad << "\n";
}
```

---

Funciones Miembro:
- [`operator[]`](https://en.cppreference.com/w/cpp/container/vector/operator_at) y [`.at()`](https://en.cppreference.com/w/cpp/container/vector/at): Acceso al elemento
- [`.push_back()`](https://en.cppreference.com/w/cpp/container/vector/push_back) y [`.emplace_back()`](https://en.cppreference.com/w/cpp/container/vector/emplace_back): Añaden un elemento al final del vector
- [`.reserve()`](https://en.cppreference.com/w/cpp/container/vector/reserve): Pre-reservan espacio para el vector

```cpp
#include <vector>
```
```cpp
ciudades.push_back("Murcia");

tus_notas[1] = 0.1;
```

---

#### [`std::tuple`](https://en.cppreference.com/w/cpp/utility/tuple)

Una colección de datos de múltiples tipos.

Útiles para retornar de funciones, ya que se pueden desacoplar elegantemente.

```cpp
#include <tuple>
```
```cpp
std::tuple<std::string, int, double> foo() {
   return {"hola", 42, 42.0};
}

auto [str_v, int_v, flt_v] = foo();
```

---

#### [`std::map`](https://en.cppreference.com/w/cpp/container/map) / [`std::unordered_map`](https://en.cppreference.com/w/cpp/container/unordered_map)

Contiene pares clave-valor con claves únicas.

- [`std::map`](https://en.cppreference.com/w/cpp/container/map) implementa un árbol de búsqueda binario
- [`std::unordered_map`](https://en.cppreference.com/w/cpp/container/unordered_map) implementa un _hash map_

```cpp
#include <map>
```
```cpp
std::map<int, std::string> ciudades {
   {1, "Madrid"},
   {2, "Nueva York"},
   {3, "París"}
};
```

---

Funciones miembro:

- [`operator[]`](https://en.cppreference.com/w/cpp/container/map/operator_at): Acceso al valor asociado a la clave.
- [`.at()`](https://en.cppreference.com/w/cpp/container/map/at): Devuelve el valor asociado a la clave.  
  Si no existe, lanza una excepción
- [`.contains()`](https://en.cppreference.com/w/cpp/container/map/contains): Comprueba si una clave existe

```cpp
ciudades[4] = "Murcia";
std::string mi_fav = ciudades.at(1);
if (ciudades.contains(3)) { /* ... */ }
```

También se pueden iterar y desacoplar fácilmente:

```cpp
for (auto const & [id, nombre] : ciudades) {
   std::cout << nombre << " " << id "\n";
}
```

---

### [Algoritmos](https://en.cppreference.com/w/cpp/algorithm)
Funciones genéricas para cualquier contenedor.

- Ordenar, [`std::sort(inicio, fin, predicado)`](https://en.cppreference.com/w/cpp/algorithm/sort)
- Mapear, [`std::transform(inicio, fin, salida, operación)`](https://en.cppreference.com/w/cpp/algorithm/transform)
- Reducir, [`std::accumulate(inicio, fin, primero, función)`](https://en.cppreference.com/w/cpp/algorithm/accumulate)
- Filtrar, [`std::copy_if(inicio, fin, salida, predicado)`](https://en.cppreference.com/w/cpp/algorithm/copy)

---

Las puedes concatenar:
```cpp
bool mayor_a_menor (int x, int y) { return x > y; }
int cuadrado (int x) { return x * x; }
int producto (int a, int b) { return a * b; }

std::vector<int> valores{1, 3, 4, 5, 2};

std::sort(valores.begin(), valores.end(), mayor_a_menor);
std::transform(
   valores.begin(),
   valores.end(),
   valores.begin(),
   cuadrado
);
int result = std::accumulate(
   valores.begin(),
   valores.end(),
   1,
   producto
);
```

---

### Funciones lambda y predicados
Funciones sin nombre, que se crean en el momento.

Útiles en combinación con algoritmos:
```cpp
// bool mayor_a_menor (int x, int y) { return x > y; }

std::sort(
   valores.begin(),
   valores.end(),
   [](int a, int b){ return a > b; }
);
```

---

```cpp
[capturas] (parámetros) -> retorno { /* código */ }
```
 - El retorno es opcional si el compilador lo puede deducir
 - Las capturas son más complicadas...
   - Su propósito es cómo se tratan las variables que se referencian en el código

```cpp
auto filtrar_menores_de (std::vector<int> const & v, int n) {
   std::vector<int> resultado;
   std::copy_if(v.begin, v.end(), std::back_inserter(resultado),
      [=] (int x) {  // [=] captura n, copiándola
         return x < n;
      }
   );
   return resultado;
}
```

---
<!-- header: '' -->

## Funciones

### Paso de parámetros

- `T x`: Copia `x`
- `const T x`: Copia `x`, no se puede modificar
- `T & x`: Referencia a `x`, mutable.
- `T const & x`: Referencia a `x`, inmutable
- `T && x`: Paso por movimiento


Depende del tipo, se pasa por valor o referencia constante:
- Si es pequeño (`int`, `char`, `long`, `my_vector3`): **copia**
- Si es grande, o se tienen que copiar muchos datos (`std::vector`, `std::string`): **referencia constante**


---
<!-- header: '**Funciones**' -->


### Especificadores
Contratos con las funciones, para que sean más óptimas.

- [`noexcept`](https://en.cppreference.com/w/cpp/language/noexcept_spec): La función no va a lanzar ninguna excepción. Si lo hace... _cagaste_
- [`[[nodiscard]]`](https://en.cppreference.com/w/cpp/language/attributes/nodiscard): Vas a capturar lo que devuelve la función
- [`inline`](https://en.cppreference.com/w/cpp/language/inline): Quita el _overhead_ de una llamada a función. No lo uses con funciones recursivas
- [`constexpr`](https://en.cppreference.com/w/cpp/language/constexpr): Son funciones que, por lo general, se calculan en tiempo de compilación. No las uses con bloques `try`-`catch`.

---

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




---
<!-- header: '' -->

## Estructuración del código

---
<!-- header: '**Estructuración del código**' -->

### namespace
Permiten agrupar funciones y clases por funcionalidad.

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

### _Header files_ (`.hpp`) y _source files_ (`.cpp`)

En el _header_ van las _declaraciones_. En el _source_ van las _definiciones_ (implementación).
- En el _header_ puede haber definiciones pero, por defecto, son _inline_
- Constructores y macros en _header_
- Usa _guards_ en los _header_ (`#ifndef LIB_HPP`, `#define LIB_HPP`) para prevenir duplicados
- Al definir funciones miembro en el _source_, hay que especificar la clase: `MyClass::my_method() {}`


---

```cpp
// lib.hpp

#ifndef LIB_HPP
#define LIB_HPP

inline say_hello() { std::cout << "hello\n"; }
int foo(int bar);

class MyClass() {
    public:
        MyClass(float y): _y {y} { }

        int get();

    private:
        int _x = 0;
        float _y;
};

#endif
```

---

```cpp
// lib.cpp

int foo(int bar) {
    return bar + myVar;
}

int myVar = 0;

MyClass::get() { return _x + _y; }
```



---

<!-- _paginate: skip -->
![bg contain opacity:.15](img/gul_logo.svg)
# ¡Ánimo!


<br>
<br>

_Grupo de Usuarios de Linux_
[@guluc3m](https://twitter.com/guluc3m) | [gul.uc3m.es](https://gul.uc3m.es)
