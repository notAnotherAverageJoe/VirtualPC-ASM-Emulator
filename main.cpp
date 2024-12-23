#include <iostream>
#include <vector>
#include <unordered_map>
#include <string>

enum Opcode
{
    LOAD,
    ADD,
    SUBTRACT,
    STORE,
    JUMP,
    PRINT,
    HALT
};

struct Instruction
{
    Opcode op;
    int arg1;
    int arg2;
    int arg3;
    std::string strArg;
};

class VirtualCPU
{
private:
    std::vector<int> registers;
    std::vector<int> memory;
    std::vector<Instruction> program;
    int programCounter;
    bool running;

public:
    VirtualCPU(int registerCount, int memorySize)
        : registers(registerCount, 0), memory(memorySize, 0), programCounter(0), running(true) {}

    void loadProgram(const std::vector<Instruction> &prog)
    {
        program = prog;
        programCounter = 0;
        running = true;
    }

    void execute()
    {
        while (running)
        {
            Instruction instr = program[programCounter];
            programCounter++;
            switch (instr.op)
            {
            case LOAD:
                if (!instr.strArg.empty())
                {

                    std::cout << "Loaded string: " << instr.strArg << " into Register[" << instr.arg1 << "]" << std::endl;
                }
                else
                {
                    registers[instr.arg1] = instr.arg2;
                }
                break;

            case ADD:
                registers[instr.arg1] += registers[instr.arg2];
                break;
            case SUBTRACT:
                registers[instr.arg1] -= registers[instr.arg2];
                break;
            case STORE:
                memory[instr.arg2] = registers[instr.arg1];
                break;
            case JUMP:
                programCounter = instr.arg1;
                break;
            case PRINT:
                if (!instr.strArg.empty())
                {
                    std::cout << "String: " << instr.strArg << std::endl;
                }
                else
                {
                    std::cout << "Register[" << instr.arg1 << "] = " << registers[instr.arg1] << std::endl;
                }
                break;

            case HALT:
                running = false;
                break;
            default:
                std::cerr << "Unknown opcode!" << std::endl;
                running = false;
            }
        }
    }
};
std::vector<Instruction> assembleProgram()
{
    return {
        {LOAD, 0, 10, 0}, // LOAD R0, 10
        {LOAD, 1, 20, 0}, // LOAD R1, 20
        {ADD, 0, 1, 0},   // ADD R0, R1
        {PRINT, 0, 0, 0}, // PRINT R0
        {STORE, 0, 0, 0}, // STORE R0 to MEM[0]
        {HALT, 0, 0, 0}   // HALT
    };
}

std::vector<Instruction> subProgram()
{
    return {
        {LOAD, 0, 50, 0}, // load 50 in R0
        {LOAD, 1, 25, 0}, // load 25 into R1
        {SUBTRACT, 1, 0, 0},
        {PRINT, 0, 0, 0},
        {STORE, 0, 0, 0},
        {HALT, 0, 0, 0} // stop the prog
    };
}

int main()
{
    // Initialize Virtual CPU with 4 registers and 16 memory slots
    VirtualCPU cpu(4, 16);

    // Assemble the program
    auto program = assembleProgram();
    auto subtractProgram = subProgram();

    // Load and execute the program
    cpu.loadProgram(program);
    cpu.execute();
    cpu.loadProgram(subtractProgram);
    cpu.execute();

    return 0;
}
