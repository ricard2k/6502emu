#include <format>
#include <iostream>
#include <string>

typedef unsigned char uint8_t ;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;
//typedef unsigned long uint64_t;

struct Memory {
    static const uint32_t MAX_MEMORY = 1024 * 64;
    uint8_t memory [MAX_MEMORY];

    void initialise() {
        memory[0xFFFC] = 0x4C; // JMP 0080
        memory[0xFFFD] = 0x80;
        memory[0xFFFE] = 0x00;

        memory[0x0080] = 0xA9; // LDA 03   
        memory[0x0081] = 0x03;
        memory[0x0082] = 0x4C; // JMP 0080
        memory[0x0083] = 0x80;
        memory[0x0084] = 0x00;
    }
    
    uint8_t read8(uint16_t address) {
        return memory[address];
    }

    void write8(uint16_t address, uint8_t value) {
        memory[address] = value;
    }
};



struct CPU {

    uint8_t A, X, Y; // registers
    uint16_t PC; // program counter
    uint8_t SP; // stack pointer

    bool C : 1; // carry flag
    bool Z : 1; // zero flag
    bool I : 1; // interrupt disable flag
    bool D : 1; // decimal mode flag
    bool B : 1; // break command flag
    bool V : 1; // overflow flag
    bool N : 1; // negative flag
   
    void reset() {
        A = X = Y = 0;
        SP = 0xFD;
        PC = 0xFFFC;
        C = Z = I = D = B = V = N = 0;
        std::printf("Reset\n");
        printRegisters();
    }

    uint8_t getIMM(Memory& memory) {
        uint8_t data = memory.read8(PC);
        PC++;
        return data;
    }

    uint8_t getZP0(Memory& memory) {
        uint8_t addr = memory.read8(PC);
        PC++;
        return addr;
    }

    uint8_t getZPX(Memory& memory) {
        uint8_t addr = memory.read8(PC);
        PC++;
        addr += X;
        return addr;
    }

    uint8_t getZPY(Memory& memory) {
        uint8_t addr = memory.read8(PC);
        PC++;
        addr += Y;
        return addr;
    }

    uint16_t getABS(Memory& memory) {
        uint16_t addr = memory.read8(PC);
        PC++;
        addr |= (memory.read8(PC) << 8);
        PC++;
        return addr;
    }

    uint16_t getABX(Memory& memory) {
        uint16_t addr = memory.read8(PC);
        PC++;
        addr |= (memory.read8(PC) << 8);
        PC++;
        addr += X;
        return addr;
    }

    uint16_t getABY(Memory& memory) {
        uint16_t addr = memory.read8(PC);
        PC++;
        addr |= (memory.read8(PC) << 8);
        PC++;
        addr += Y;
        return addr;
    }

    uint16_t getIZX(Memory& memory) {
        uint8_t t = memory.read8(PC);
        PC++;
        uint16_t addr = memory.read8(t);
        t++;
        addr |= (memory.read8(t) << 8);
        return addr;
    }

    uint16_t getIZY(Memory& memory) {
        uint8_t t = memory.read8(PC);
        PC++;
        uint16_t addr = memory.read8(t);
        t++;
        addr |= (memory.read8(t) << 8);
        addr += Y;
        return addr;
    }

    void execute(Memory& memory) {

        uint8_t opcode = memory.read8(PC);
        PC++;
        switch(opcode) {
            case 0xA9: //LDA IMM
            {   
                uint8_t value = getIMM(memory);
                std::printf("LDA #%02X\n", value);
                LDA(value);
            } break;
            case 0xA5: //LDA ZP0
            {       
                uint8_t zpAddr = getZP0(memory);
                std::printf("LDA %02X\n", zpAddr);
                uint8_t value = memory.read8(zpAddr);
                LDA(value);
            } break;
            case 0xB5: //LDA ZPX
            {   
                uint8_t zpAddr = getZPX(memory);
                std::printf("LDA %02X, X\n", zpAddr);
                uint8_t value = memory.read8(zpAddr);
                LDA(value);
            } break;
            case 0xAD: //LDA ABS
            {   
                uint16_t absAddr = getABS(memory);
                std::printf("LDA %04X\n", absAddr);
                uint8_t value = memory.read8(absAddr);
                LDA(value);
            } break;
            case 0xBD: //LDA ABX
            {   
                uint16_t absAddr = getABX(memory);
                std::printf("LDA %04X, X\n", absAddr);
                uint8_t value = memory.read8(absAddr);
                LDA(value);
            } break;
            case 0xB9: //LDA ABY
            {   
                uint16_t absAddr = getABY(memory);
                std::printf("LDA %04X, Y\n", absAddr);
                uint8_t value = memory.read8(absAddr);
                LDA(value);
            } break;
            case 0xA1: //LDA IZX
            {   
                uint16_t absAddr = getIZX(memory);
                std::printf("LDA (%04X, X)\n", absAddr);
                uint8_t value = memory.read8(absAddr);
                LDA(value);
            } break;
            case 0xB1: //LDA IZY
            {   
                uint16_t absAddr = getIZY(memory);
                std::printf("LDA (%04X), Y\n", absAddr);
                uint8_t value = memory.read8(absAddr);
                LDA(value);
            } break;
            case 0x4C: //JMP ABS
            {   
                uint16_t absAddr = getABS(memory);
                std::printf("JMP %04X\n", absAddr);
                PC = absAddr;
            } break;
            case 0x6C: //JMP IND
            {   
                uint16_t absAddr = getABS(memory);
                std::printf("JMP ($%04X)\n", absAddr);
                uint16_t lo = memory.read8(absAddr);
                uint16_t hi = memory.read8(absAddr + 1);
                uint16_t addr = (hi << 8) | lo;
                PC = addr;
            } break;
            default: {
                printf("Instruction not handled %02X\n", opcode);
            } break;
        }
        printRegisters();
    }

    void printRegisters() {
        std::printf("PC:%04X, SP:%02X A:%02X, X:%02X, Y:%02X, C:%d, Z:%d, I:%d, D:%d, V:%d, N:%d\n", 
            PC, SP, A, X, Y, C, Z, I, D, V, N);
    }

    void LDA(uint8_t value) {
        A = value;
        Z = (A == 0);
        N = (A & 0b10000000) > 0;
    }
};

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
