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
## Entrada y Salida

 * Hay dos clases
   - `std::istream` (Flujo de entrada)
   - `std::ostream` (Flujo de salida)
 * Por defecto se utilizan para leer y escribir texto:

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
Los objetos en memoria se almacenan como una secuencia de bytes. Por ejemplo:

```cpp
int x = 42;
```

Depende del computador:

 * _little-endian_: `2A 00 00 00`
 * _big-endian_: `00 00 00 2A`

(Vamos a suponer _little-endian_)


---
Por ejemplo un número de coma flotante en simple precisión se representa en
memoria en base al estándar IEEE 754.

```cpp
float x = 42.0;
```

* En IEEE 754 en hexadecimal es: 42280000

* En memoria se representa como: `00 00 28 42`

De igual manera la cadena `"hola"` se representa como la secuencia de los
bytes `'h'`, `'o'`, `'l'`, `'a'`. O en hexadecimal:

 * `68 6f 6c 61`

---
En un archivo sabemos escribir cadenas.

```cpp
#include <fstream>

int main () {
   std::ofstream my_output{"my-file.txt"};
   my_output << "42"   << "\n"
             << "42.0" << "\n"
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
### Salida binaria en C++

Empezamos abriendo el archivo:
 * Con constructor (`std::ofstream my_file{"my-file.bin", std::ios::binary};`)
 * Con función miembro (`my_file.open("my-file.bin", std::ios::binary);`)
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
   int int_value = 42; // ¡¡Número mágico!!
   float float_value = 42.0;
   std::string string_value = "hola";
   file.write(reinterpret_cast<const char*>(&int_value), sizeof(int_value));
   file.write(reinterpret_cast<const char*>(&float_value), sizeof(float_value));
   file.write(string_value.data(), string_value.size()); // ¡¡Conversión implícita!!
```

La función miembro `std::ostream write` pide dos parámetros de tipos:
 * `const char *`
 * `std::size_t`

De manera similar a la función `fwrite' en C.

_Los errores del clang-tidy están solucionados en el código de ejemplo, se explica más adelante cómo solucionarlos_.

---

**Hay dos problemas** en C++ no deben hacerse las conversiones (_casts_) como
en C. Es decir:

```cpp
file.write((const char *)(&int_value), sizeof(int_value));  // Jamás
file.write(reinterpret_cast<const char*>(&int_value), sizeof(int_value)); // Válido
```

Además `clang-tidy` se va a quejar del código de que acabamos de escribir.
 * La conversiones tipo `reinterpret_cast` no son _type-safe_.
 * `int y = 10; float x = *reinterpret_cast<float*>(&x)` es UB.

---

En este caso está justificado:
 * Solo existe una función del tipo `write`
   - Que pide un puntero a una cadena de carácteres (`const char *`)
   - Y el tamaño de la cadena (`std::size_t`).
 * No se puede hacer de ninguna otra forma.

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
 * Obtenemos un archivo `my-file.bin`
 * Si lo leemos con `hexdump`

> hexdump -C my-file.bin

```plain
00000000  2a 00 00 00 00 00 28 42  68 6f 6c 61              |*.....(Bhola|
0000000c
```

---
### Entrada binaria en C++
Es similar a la entrada:
 * `std::ifstream` en vez de `std::ofstream`.
 * `.read()` en vez de `.write()`.

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
   if (not file.read(reinterpret_cast<const char*>(&int_value), sizeof(int_value))) {
      std::cerr << "No se pudo leer el entero\n";
      return 2;
   }
   // NOLINTNEXTLINE (cppcoreguidelines-pro-type-reinterpret-cast)
   if (not file.read(reinterpret_cast<const char*>(&float_value), sizeof(float_value))) {
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
 * Una _template_ es una plantilla.

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

A veces nos encontramos con que repetimos la misma función para distintos
tipos.

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

 * La `T` se sustituye por el tipo que le pasemos.
   - `print_square<int>(10)`
   - `print_square(10)`, se deduce el tipo.
 * Si para un tipo no se define alguna función, falla.
 * `print_square<float>` es equivalente a la función:

```cpp
// T = float
void print_square (float x) {
   std::cout << x * x << std::endl;
}
```

---

* También sirve para estructuras de datos:
 - `std::vector<int>`
 - `std::vector<my_type>`...
 - `std::vector<std::vector<std::string>>`
* Todas las funciones de la clase se definen para el tipo del _template_
* En C, tendríamos que usar `void*` o definirlo para cada posible tipo. Y
  para cada posible operación.

---

<center><h1><b>STL</b></h1><br/>
<b>Standard Template Library</b>
</center>

---

### Estructuras de datos
#### std::vector
#### std::pair
#### std::map
#### std::unordered\_map
#### std::tuple

---

### Algoritmos
#### std::sort
#### std::transform
#### std::filter

### Funciones lambda y predicados
#### Funciones lambda
#### Capturas
#### Funciones Lambda, Estructuras de Datos y Algoritmos

---

## Transparencias
![w:400 center](img/qrcode-transparencias.svg)
<center>
<a href="https://github.com/guluc3m/modern-cpp">github.com/guluc3m/modern-cpp</a>
</center>


---
<!-- header: '**¿Qué es C++ moderno?**' -->

## ¿Qué es C++ moderno?

C++ es más que «C con cosas»
- No todo el código C compila en C++
    - C es un lenguaje inseguro
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

### PROHIBIDO
- _Raw pointers_ - `int* ptr;`
- _C arrays_ - `int[] arr;`
- Manipulación manual de memoria - `new` / `malloc()`
- Librerías de C - `#include <whatever.h>`

Todo esto hace que el código no sea seguro.


---


## Cosas nuevas

---
<!-- header: '**Cosas nuevas**' -->

### [`auto`](https://en.cppreference.com/w/cpp/language/auto)
Permite al compilador inferir el tipo de una variable.
```cpp
auto x = 1;
```

Útil al recorrer vectores:
```cpp
for (auto it = vector.begin(); it != vector.end(); ++it) {
    it->doSomething();
}
```

---

### `enum class`
Evolución de `enum`, quitando ciertas limitaciones.

```cpp
enum class Color {
    Red,
    Green,
    Blue
};

auto col = Color::Red;
```

---

### using
Evolución de `typedef`.

```cpp
using Address = std::uint32_t;
```

### constexpr
Evolución de `#define`, teniendo en cuenta el tipo de la variable.

```cpp
constexpr ANSWER = 42;
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
- `std::static_cast`
- namespaces
- lambdas
- `std::unique_ptr`, `std::shared_ptr`


---

### Funciones
- Parameter passing convention
- Specifiers: contratos autoimpuestos de Jujutsu Kaisen


---

### Clases

- Constructores
- miembros públicos/privados/protegidos
- POR DIOS NO TOQUÉIS LA HERENCIA
- member function specifiers

---

### Contenedores
Estructuras de datos predefinidas.

#### Métodos genéricos
- `.size()`: Devuelve el número de elementos
- `.clear()`: Elimina todos los elementos

---

#### [`std::vector`](https://en.cppreference.com/w/cpp/container/vector)
```cpp
std::vector<int> myVec {1, 2};  // constructor

myVec.push_back(3);  // añadir un elemento
```

#### [`std::map`](https://en.cppreference.com/w/cpp/container/map) / [`std::unordered_map`](https://en.cppreference.com/w/cpp/container/unordered_map)


#### [`std::tuple`](https://en.cppreference.com/w/cpp/container/tuple)


---
<!-- header: '**Estructuración del código**' -->

## Estructuración del código


---
<!-- header: '**Templates**' -->

## Templates




---

<div class="columns">
<div>

#### C/C++

```cpp
Stuff stuff = new Stuff();
```

</div>
<div>

#### C++ moderno

```cpp
auto stuff = Stuff {};
```

</div>
</div>
