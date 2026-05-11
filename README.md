<h1 align="center">
  👾 CHIP-8 Emulator 👾
</h1>

<p align="center">
  <strong>Yet another CHIP-8 Emulator written in modern C++ and SDL2.</strong>
</p>

## 📖 About

This project was developed as an introduction to emulator development.
It faithfully implements all CHIP-8 instructions (with the exception of sound) and features an SDL2-based display capable of smooth rendering.

## ✨ Features

- **Standard C++**: Written using modern C++ features.
- **SDL2 Rendering**: Uses SDL2 for fast and reliable graphics drawing.
- **Full Instruction Set**: Emulates all CHIP-8 opcodes (sans sound).

## 🚀 Getting Started

### Prerequisites

To build and run this emulator, you will need:
- Windows (the project includes a Visual Studio `.sln` file)
- [Visual Studio](https://visualstudio.microsoft.com/)
- [SDL2 Library](https://libsdl.org/)

### Building

1. Clone the repository to your local machine.
2. Open `chip8emu-cpp.sln` in Visual Studio.
3. Build the solution (e.g., using `Ctrl + Shift + B`).

## 🕹️ Usage

Run the compiled executable and pass the path to a CHIP-8 ROM as the first argument:

```sh
chip8-emu.exe <path_to_rom_image>
```

## ⌨️ Controls

The original CHIP-8 has a 16-key hex keypad (0-F). This emulator maps it to the standard QWERTY keyboard using the rows `1234`, `QWER`, `ASDF`, and `ZXCV`.

### Key Mapping Table

| Emulator Key | CHIP-8 Key |   | Emulator Key | CHIP-8 Key |
|:------------:|:----------:|---|:------------:|:----------:|
| **1**        | `1`        |   | **Q**        | `4`        |
| **2**        | `2`        |   | **W**        | `5`        |
| **3**        | `3`        |   | **E**        | `6`        |
| **4**        | `C`        |   | **R**        | `D`        |
| **A**        | `7`        |   | **Z**        | `A`        |
| **S**        | `8`        |   | **X**        | `0`        |
| **D**        | `9`        |   | **C**        | `B`        |
| **F**        | `E`        |   | **V**        | `F`        |

### Layout Comparison

**Standard QWERTY layout:**
```
1 2 3 4
Q W E R
A S D F
Z X C V
```

**Emulated CHIP-8 Keypad:**
```
1 2 3 C
4 5 6 D
7 8 9 E
A 0 B F
```
