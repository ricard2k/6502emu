#include "6502emu.h"

int main(int argc, char const *argv[])
{
    Memory memory;
    memory.initialise();
    CPU cpu;
    cpu.reset();
    while (true)
    {
        cpu.execute(memory);
    }
    
    return 0;
}
