<!-- [![Review Assignment Due Date](https://classroom.github.com/assets/deadline-readme-button-22041afd0340ce965d47ae6ef1cefeee28c7c493a6346c4f15d667ab976d596c.svg)](https://classroom.github.com/a/b842RB8g)
# Tugas Besar 1 IF2010 Pemrograman Berorientasi Objek

Project Structure ini hanyalah referensi, Anda dapat menyesuaikannya dengan kebutuhan tim Anda
 -->

# NIMONSPOLY
## IF2010 Object Oriented Programming

This program is made for IF2010 Object Oriented Programming course @ Institut Teknologi Bandung.

## Meet the developers – [BCC] BurntCheesecake 

Made Branenda Jordhy<br>
13524026<br>
[GitHub Account](https://github.com/ethj0r)<br>

Muhammad Nur Majiid<br>
13524028<br>
[GitHub Account](https://github.com/MAJIIDMN)<br>

Jason Edward Salim<br>
13524034<br>
[GitHub Account](https://github.com/jsndwrd)<br>

Bryan Pratama Putra Hendra<br>
13524067<br>
[GitHub Account](https://github.com/Bryannpph)<br>

Athilla Zaidan Zidna Fann<br>
13524068<br>
[GitHub Account](https://github.com/AthillaZaidan)<br>

## Directory

1. `config/`: game configuration files.
2. `data/`: runtime data (if needed).
3. `include/`: header files (`.hpp`).
4. `src/`: source files (`.cpp`).
5. `build/`: CMake build directory (generated).
6. `bin/`: executable output directory (generated).
7. `CMakeLists.txt`: CMake build configuration.
8. `README.md`: project overview.

## Requirements

- CMake (>= 3.16 recommended).
- A C++17 compiler (`clang++`, `g++`, or MSVC).
- Raylib.

## Instructions

### Installation

Make sure you have a C++17 compiler and CMake installed.

Install Raylib:
- macOS (Homebrew): `brew install raylib`

### Configure & Build

```bash
cmake -S . -B build
cmake --build build
```

Output:
- Linux/macOS: `bin/game`
- Windows: `bin/game.exe`

### Run

```bash
./bin/game
```

(Alternative)

```bash
cmake --build build --target run
```

### Clean

Delete the build output directories:
- `build/`
- `bin/`

GUI is always enabled. There is no CLI runtime mode.
