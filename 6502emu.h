struct Memory;
struct CPU;

typedef unsigned char uint8_t ;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;
//typedef unsigned long uint64_t;

struct Memory {
    static const uint32_t MAX_MEMORY = 1024 * 64;
    uint8_t memory [MAX_MEMORY];

    void initialise();
    
    uint8_t read8(uint16_t address);

    void write8(uint16_t address, uint8_t value);
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
   
    void reset();

    uint8_t getIMM(Memory& memory);

    uint8_t getZP0(Memory& memory);

    uint8_t getZPX(Memory& memory);

    uint8_t getZPY(Memory& memory);

    uint16_t getABS(Memory& memory);

    uint16_t getABX(Memory& memory);

    uint16_t getABY(Memory& memory);

    uint16_t getIZX(Memory& memory);

    uint16_t getIZY(Memory& memory);

    void execute(Memory& memory);

    void printRegisters();

    void LDA(uint8_t value);
};
