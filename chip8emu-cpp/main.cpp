#include <fstream>

#include "cpu.hpp"

int main()
{
    std::ifstream io("ibm_logo.ch8", std::ios::binary | std::ios::ate);
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
}