# Template for starting development for stm32f1x.

Based on CMSIS, uses cmake+make bulding system.

## Requirements:
* Arm Toolchain: arm-none-eabi-gcc
* gdb-multiarch or arm-none-eabi-gdb debugger
* cmake >= 3.0
* make
* build-essential
* st-link utility for Linux
* git (recommended for clonning project)

## Usage:

Clone project via git or http download (not recommended).

Open terminal, cd to root project directory.

Run `cmake .` to generate Makefile, then run `make` or `make bin` or `make hex` depending on what format of firmware you want.

Flash firmware with command `make flash`.

To clean build artifacts run `make clean`. Also after modifying CMakeLists.txt you need to clean cmake cache via deleting CMakeCache.txt file (but NOT CMakeLists!)

## Usage in VScode:

Required VS extensions:
* CMake
* Cmake Tools
* C/C++ (IntelliSense, debugging, and code browsing)
* Cortex-Debug
* C/C++ Themes (optional)

Firstly, open working directory and setup toochain.

Visit https://code.visualstudio.com/docs/cpp/cmake-linux for details.

Setup variant: Debug or Release.

To build project run: "Terminal->Run Build task->Cmake: build"
Project can be rebuild with "Cmake: clean rebuild"

Also you can build artifacts (bin/hex/elf) from CMake extension tab.

# Lab 4 work: Connecting SSD1306 drive to SPI

## Project Description
The PicSimLab project is designed to connect an SSD1306 OLED display to a PIC18F4620 microcontroller via the built-in SPI.

## Functionality
- Libraries for working with SPI
- SSD1306 disk driver
- Chessboard display
- Test graphic patterns
- Automatic mode change

## Project structure
- `psim/spi_display.pzw` - PicSimLab project
- `src/spi_display.c' - the main source code
- `src/ssd1306.h' - file header
- `Makefile` - for creating a project

## Connection diagram
- SCK (SPI clock frequency) → RC3
- SDO (MOSI) → RC5
- CS (chip selection) → RA0
- DC (data/command) → RA1
- RES (reset) → RA2

## Compilation
of `
bashly do
