#include <fstream>

#include "cpu.hpp"

int main(int argc, char** argv)
{
    if (argc < 2)
    {
        std::cout << std::format("Usage: {} <rom_file>", argv[0]);
        return 1;
    }

    std::ifstream io(argv[1], std::ios::binary | std::ios::ate);
    const auto size = io.tellg();
    io.seekg(0, std::ios::beg);

    std::vector<uint8_t> program(size);
    io.read((char*)program.data(), size);

    try
    {
        chip8_emu::Cpu cpu;
        cpu.Load(program);
        cpu.Run();
    }
    catch (const std::exception& err)
    {
        std::cout << "Unexpected error: " << err.what() << std::endl;
    }

    return 0;
}