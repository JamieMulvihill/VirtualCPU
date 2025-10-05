// VirtualCPU.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>

struct Register
{
    uint8_t Reg;
};

struct Memory
{
    uint8_t Addresses[65536];
};

class CPU
{
private:
    Memory Memory;

    Register Registers[4];

    uint16_t PC = 0;    // Program Counter 
    uint16_t SP = 0xFFFF;    // Stack Pointer
    uint8_t flags = 0;  // Status flags
    bool bIsRunning = false;   // Is CPU running?
    int8_t NUM_REGISTERS = 4;
    int16_t NUM_ADDESSES = 65536;

public:
    CPU()
    {
        for (int i = 0; i < NUM_REGISTERS; i++)
        {
            Registers[i].Reg = 0;
        }

        for (int i = 0; i < NUM_ADDESSES; i++)
        {
            Memory.Addresses[i] = 0;
        }
    }

    void SetIsRunning(bool InIsRunning) { bIsRunning = InIsRunning; }

    void Start()
    {
        bIsRunning = true;
    }

    // Execution Functions
    uint8_t Fetch()
    {
        // reads byte at PC, increments PC
        uint8_t Byte = Memory.Addresses[PC];
        std::cout << "Fetch: PC=" << PC << " Byte=0x" << std::hex << (int)Byte << std::endl;
        PC++;
        return Byte;
    }

    void Decode(uint8_t Opcode)
    {
        std::cout << "Decode: Opcode=0x" << std::hex << (int)Opcode << std::endl;

        // figures out what instruction to run
        switch (Opcode)
        {
        case 0x00: // HALT top the CPU
        {
            bIsRunning = false;
            std::cout << "  HALT: Stopping CPU" << std::endl;
            break;
        }
        case 0x01: // LOAD_IMM Load immediate value into register
        {
            uint8_t ByteA = Fetch();
            uint8_t ByteB = Fetch();
            std::cout << "  LOAD_IMM: R" << (int)ByteA << " = " << (int)ByteB << std::endl;
            Registers[ByteA].Reg = ByteB;
            break;
        }
        case 0x02: // ADD Add two registers, store in first register
        {
            uint8_t Dest1 = Fetch();
            uint8_t Source1 = Fetch();
            std::cout << "  ADD: R" << (int)Dest1 << " = R" << (int)Dest1
                << "(" << (int)Registers[Dest1].Reg << ")"
                << " + R" << (int)Source1
                << "(" << (int)Registers[Source1].Reg << ")" << std::endl;
            Registers[Dest1].Reg = Registers[Dest1].Reg + Registers[Source1].Reg;
            break;
        }
        case 0x03: // SUB Subtract two registers
        {
            uint8_t Dest2 = Fetch();
            uint8_t Source2 = Fetch();
            std::cout << "  SUB: R" << (int)Dest2 << " = R" << (int)Dest2
                << "(" << (int)Registers[Dest2].Reg << ")"
                << " - R" << (int)Source2
                << "(" << (int)Registers[Source2].Reg << ")" << std::endl;
            Registers[Dest2].Reg = Registers[Dest2].Reg - Registers[Source2].Reg;
            break;
        }
        case 0x04: // PRINT Print register value 
        {
            uint8_t Byte = Fetch();
            std::cout << "  PRINT: R" << (int)Byte << " = " << (int)Registers[Byte].Reg << std::endl;
            std::cout << "  Output: " << (int)Registers[Byte].Reg << std::endl;
            std::cout << Registers[Byte].Reg << std::endl;
            break;
        }
        }
    }

    void Execute(uint8_t Opcode)
    {
        // performs the instruction
    }

    void Step()
    {
        // does one fetch-decode-execute cycle
        uint8_t Byte = Fetch();
        Decode(Byte);
    }

    void Run()
    {
        //  loops step() until halted
        while (bIsRunning)
        {
            Step();
        }
    }

    // Setup
    void Reset()
    {
        // initializes all registers and state
        PC = 0;
        SP = 0xFFFF;
        flags = 0;
        bIsRunning = false;

        for (int i = 0; i < NUM_REGISTERS; i++) 
        {
            Registers[i].Reg = 0;
        }
    }

    void LoadProgram(uint8_t* program, uint16_t address, size_t size)
    {
        // loads code into memory
        for (size_t i = 0; i < size; i++) 
        {
            WriteMemory(address + i, program[i]);
        }
    }

    uint8_t ReadMemory(uint16_t address)
    {
        // reads byte from memory
        return Memory.Addresses[address];
    }

    void WriteMemory(uint16_t address, uint8_t value)
    {
        // writes byte to memory
        Memory.Addresses[address] = value;
    }
    void SetFlag(uint8_t flag)
    {

    }

    void ClearFlag(uint8_t flag)
    {
        // manipulate status flags
    }

    void Push(uint8_t value)
    {

    }

    uint8_t  Pop()
    {
        // stack operations
        return 0;
    }

    void DumpRegisters()
    {
        // prints register state
        std::cout << "PC: 0x" << std::hex << PC << std::endl;
        std::cout << "SP: 0x" << std::hex << SP << std::endl;
        std::cout << "Flags: 0x" << std::hex << (int)flags << std::endl;
        for (int i = 0; i < NUM_REGISTERS; i++) 
        {
            std::cout << "R" << i << ": 0x" << std::hex << (int)Registers[i].Reg << std::endl;
        }
    }

    void DumpMemory(uint16_t start, uint16_t end)
    {
        // prints memory region
    }
};


int main()
{
    CPU CPU;

    uint8_t program[] = {
        0x01, 0x00, 0x05,    // LOAD 5 into register 0
        0x01, 0x01, 0x03,    // LOAD 3 into register 1  
        0x02, 0x00, 0x01,    // ADD R0 + R1, store in R0
        0x04, 0x00,          // PRINT R0
        0x00                // HALT
    };

    CPU.LoadProgram(program, 0x0000, sizeof(program));

    CPU.SetIsRunning(true);

    CPU.Run();

    CPU.DumpRegisters();

    return 0;
}
