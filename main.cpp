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
    MULTIPLY,
    DIVIDE,
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

    // needed to add a getter for memory to access
    const std::vector<int> &getMemory() const
    {
        return memory;
    }

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
                if (instr.arg3 >= 0)
                {
                    registers[instr.arg1] += registers[instr.arg2] += registers[instr.arg3];
                    break;
                }
                else
                { // better to be safe then sorry here and get bizzare behavior. Checks for arg 3 >0  if not add the two
                    registers[instr.arg1] += registers[instr.arg2];
                    break;
                }

            case SUBTRACT:
                registers[instr.arg1] -= registers[instr.arg2];
                break;
            case MULTIPLY:
                registers[instr.arg1] *= registers[instr.arg2];
                break;
            case DIVIDE:
                if (registers[instr.arg2] != 0)
                {
                    registers[instr.arg1] /= registers[instr.arg2];
                    std::cout << "Divided Register[" << instr.arg1 << "] by Register[" << instr.arg2 << "]" << std::endl;
                }
                else
                {
                    std::cout << "Error: Division by zero!" << std::endl;
                    running = false;
                }
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
        {LOAD, 0, 10, 0}, // load R0 with 10
        {LOAD, 1, 20, 0}, // Load r1 with 20
        {LOAD, 2, 40, 0},
        {ADD, 0, 1, 2},   // add R0, R1  10 + 20 = 30
        {PRINT, 0, 0, 0}, // print out R0
        {STORE, 0, 0, 0}, // store R0 to MEM[0]
        {HALT, 0, 0, 0}   // stops prog
    };
}

std::vector<Instruction> subProgram()
{
    return {
        {LOAD, 0, 50, 0},    // load 50 in R0
        {LOAD, 1, 25, 0},    // load 25 into R1
        {SUBTRACT, 0, 1, 0}, // 50 - 25
        {PRINT, 0, 0, 0},    // Register[0] = 25
        {STORE, 0, 12, 0},   // now we take the data in R0 and save it into MEM[12]
        {HALT, 0, 0, 0}      // stop the prog
    };
}

std::vector<Instruction> multProgram()
{
    return {
        {LOAD, 1, 10, 0},    // Load 10 into register 1
        {LOAD, 5, 10, 0},    // Load 10 into register 5
        {MULTIPLY, 1, 5, 0}, // call multiply of the values in register 1 and 5
        {PRINT, 1, 0, 0},    // print the result stored into register 1
        {STORE, 1, 6, 0},    // save it to memoery in one of the 16 slots
        {HALT, 0, 0, 0}};    // stop the prog
}
std::vector<Instruction> divProgram()
{
    return {
        {LOAD, 0, 40, 0},  // Load 40 into register 1
        {LOAD, 1, 2, 0},   // load 2 into register 2
        {DIVIDE, 0, 1, 0}, // divide 40 / 2 = 20
        {PRINT, 0, 0, 0},  // print the result stored in register 1
        {STORE, 0, 9, 0},  // store the data of register 1 into mem[9]
        {HALT, 0, 0, 0}};  // stop program
}

int main()
{
    // Initialize Virtual CPU with 4 registers and 16 memory slots - Getter for memory at the end
    VirtualCPU cpu(4, 16);

    // Assemble the program
    auto program = assembleProgram();
    auto subtractProgram = subProgram();
    auto multiplyProgram = multProgram();
    auto divideProgram = divProgram();

    // Load and execute the program
    cpu.loadProgram(program);
    cpu.execute();
    cpu.loadProgram(subtractProgram);
    cpu.execute();
    cpu.loadProgram(multiplyProgram);
    cpu.execute();
    cpu.loadProgram(divideProgram);
    cpu.execute();

    // proof of functionality -> print out memory contents using the getter
    std::cout
        << "Memory contents after program execution:" << std::endl;
    const auto &memory = cpu.getMemory(); // getter method -> LINE 40
    for (int i = 0; i < memory.size(); ++i)
    {
        std::cout << "MEM[" << i << "] = " << memory[i] << std::endl;
    }

    return 0;
}
