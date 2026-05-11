# chip8emu-cpp

Yet another CHIP-8 Emulator written in C++.

Wrote this as an intro for emulator development.\
It has all instructions implemented, sans sound.

## Usage

```
chip8-emu.exe <rom-inage>
```

It uses keyboard rows from 1234 to ZXCV for the keypad.

## Testing

Unit tests for the core logic are located in the `tests/` directory.

### Visual Studio
The unit tests are included in the `chip8emu-cpp.sln` solution as a separate `tests` project. You can build and run them directly from Visual Studio.

### Linux (Manual)
To run the tests on Linux, you can use `g++`:
```bash
g++ -std=c++20 -Ichip8emu-cpp tests/test_stack.cpp -o tests/test_stack
./tests/test_stack
```
