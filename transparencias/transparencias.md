---
marp: true
title: Analysis, Design and Implementation of a Didactic and Generic Assembly Language Simulator
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
<!-- header: '**¿Qué es C++ moderno?**' -->

## ¿Qué es C++ moderno?

C++ es "C con cosas"
- Puedes programar como en C → C/C++
    - Es un lenguaje inseguro (aunque en ocasiones esto sea necesario)
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