#include <chrono>
#include <iostream>
#include <fstream>

enum Opt
{
    HLT = 0,

    MOV_REG_TO_REG = 1,
    MOV_ADDRESS_TO_REG = 2,
    MOV_REGADDRESS_TO_REG = 3,
    MOV_REG_TO_ADDRESS = 4,
    MOV_REG_TO_REGADDRESS = 5,
    MOV_NUMBER_TO_REG = 6,
    MOV_NUMBER_TO_ADDRESS = 7,
    MOV_NUMBER_TO_REGADDRESS = 8,

    ADD_REG_TO_REG = 9,
    ADD_REGADDRESS_TO_REG = 10,
    ADD_ADDRESS_TO_REG = 11,
    ADD_NUMBER_TO_REG = 12,

    SUB_REG_FROM_REG = 13,
    SUB_REGADDRESS_FROM_REG = 14,
    SUB_ADDRESS_FROM_REG = 15,
    SUB_NUMBER_FROM_REG = 16,

    INC_REG = 17,

    DEC_REG = 18,

    CMP_REG_WITH_REG = 19,
    CMP_REGADDRESS_WITH_REG = 20,
    CMP_ADDRESS_WITH_REG = 21,
    CMP_NUMBER_WITH_REG = 23,

    JMP_REGADDRESS = 24,
    JMP_ADDRESS = 25,

    JC_REGADDRESS = 26,
    JC_ADDRESS = 27,

    JNC_REGADDRESS = 28,
    JNC_ADDRESS = 29,

    JZ_REGADDRESS = 30,
    JZ_ADDRESS = 31,

    JNZ_REGADDRESS = 32,
    JNZ_ADDRESS = 33,

    JA_REGADDRESS = 34,
    JA_ADDRESS = 35,

    JNA_REGADDRESS = 36,
    JNA_ADDRESS = 37,

    PUSH_REG = 38,
    PUSH_REGADDRESS = 39,
    PUSH_ADDRESS = 40,
    PUSH_NUMBER = 41,

    POP_REG = 42,
    POP_REGADDRESS = 43,
    POP_ADDRESS = 44,

    CALL_REGADDRESS = 45,
    CALL_ADDRESS = 46,

    RET = 47,

    MUL_REG = 48,
    MUL_REGADDRESS = 49,
    MUL_ADDRESS = 50,
    MUL_NUMBER = 51,

    DIV_REG = 52,
    DIV_REGADDRESS = 53,
    DIV_ADDRESS = 54,
    DIV_NUMBER = 55,

    AND_REG_WITH_REG = 56,
    AND_REGADDRESS_WITH_REG = 57,
    AND_ADDRESS_WITH_REG = 58,
    AND_NUMBER_WITH_REG = 59,

    OR_REG_WITH_REG = 60,
    OR_REGADDRESS_WITH_REG = 61,
    OR_ADDRESS_WITH_REG = 62,
    OR_NUMBER_WITH_REG = 63,

    XOR_REG_WITH_REG = 64,
    XOR_REGADDRESS_WITH_REG = 65,
    XOR_ADDRESS_WITH_REG = 66,
    XOR_NUMBER_WITH_REG = 67,

    NOT_REG = 68,

    SHL_REG_WITH_REG = 69,
    SHL_REGADDRESS_WITH_REG = 70,
    SHL_ADDRESS_WITH_REG = 71,
    SHL_NUMBER_WITH_REG = 72,

    SHR_REG_WITH_REG = 73,
    SHR_REGADDRESS_WITH_REG = 74,
    SHR_ADDRESS_WITH_REG = 75,
    SHR_NUMBER_WITH_REG = 76,
};

#define readInt(m, i) (                                               \
    {                                                                 \
        i += 4;                                                       \
        unsigned char b[] = {m[i - 1], m[i - 2], m[i - 3], m[i - 4]}; \
        *(unsigned int *)&b;                                          \
    })

#define writeInt(v, m, i) ( \
    {                       \
        i += 4;             \
        m[i - 1] = v;       \
        m[i - 2] = v >> 8;  \
        m[i - 3] = v >> 16; \
        m[i - 4] = v >> 24; \
    })

void loop()
{
    std::ifstream file("mem.bin", std::ios::in | std::ios::binary | std::ios::ate);
    long size = file.tellg();
    char memory [size] = {};
    file.seekg(0, std::ios::beg);
    file.read(memory, size);
    file.close();
    
    unsigned int registers[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

    unsigned int isp = 0;

    unsigned int opt = 0;

    bool zero = false;

    bool carry = false;

    bool run = true;

    std::chrono::steady_clock::time_point lastTime = std::chrono::steady_clock::now();

    int takeTimeIn = 0;

    while (run)
    {
        if (takeTimeIn <= 0)
        {
            takeTimeIn = 10000000;
            std::chrono::steady_clock::time_point time = std::chrono::steady_clock::now();
            double duration = std::chrono::duration_cast<std::chrono::microseconds>(time - lastTime).count();
            if (duration != 0)
            {
                std::string str = std::to_string((1000000 / (duration / takeTimeIn)) / 1000000);
                std::cout << "\e[?25l\r" << str << "MHz (" << std::to_string((duration / takeTimeIn) * 1000) << "ns)";
                std::cout.flush();
            }
            lastTime = std::chrono::steady_clock::now();
        }
        takeTimeIn--;

        opt = memory[isp++];

        switch (opt)
        {
        case Opt::HLT:
        {
            run = false;
            break;
        }

        case Opt::MOV_REG_TO_REG:
        {
            unsigned int reg1 = readInt(memory, isp);
            unsigned int reg2 = readInt(memory, isp);
            registers[reg2] = registers[reg1];
            break;
        }
        case Opt::MOV_ADDRESS_TO_REG:
        {
            unsigned int address = readInt(memory, isp);
            unsigned int value = readInt(memory, address);
            unsigned int reg = readInt(memory, isp);
            registers[reg] = value;
            break;
        }
        case Opt::MOV_REGADDRESS_TO_REG:
        {
            unsigned int reg1 = readInt(memory, isp);
            unsigned int reg2 = readInt(memory, isp);
            unsigned int address = registers[reg1];
            unsigned int value = readInt(memory, address);
            registers[reg2] = value;
            break;
        }
        case Opt::MOV_REG_TO_ADDRESS:
        {
            unsigned int reg = readInt(memory, isp);
            unsigned int address = readInt(memory, isp);
            unsigned int val = registers[reg];
            writeInt(val, memory, address);
            break;
        }
        case Opt::MOV_REG_TO_REGADDRESS:
        {
            break;
        }
        case Opt::MOV_NUMBER_TO_REG:
        {
            unsigned int number = readInt(memory, isp);
            unsigned int reg = readInt(memory, isp);
            registers[reg] = number;
            break;
        }
        case Opt::MOV_NUMBER_TO_ADDRESS:
        {
            break;
        }
        case Opt::MOV_NUMBER_TO_REGADDRESS:
        {
            break;
        }

        case Opt::ADD_REG_TO_REG:
        {
            break;
        }
        case Opt::ADD_REGADDRESS_TO_REG:
        {
            break;
        }
        case Opt::ADD_ADDRESS_TO_REG:
        {
            break;
        }
        case Opt::ADD_NUMBER_TO_REG:
        {
            break;
        }

        case Opt::SUB_REG_FROM_REG:
        {
            break;
        }
        case Opt::SUB_REGADDRESS_FROM_REG:
        {
            break;
        }
        case Opt::SUB_ADDRESS_FROM_REG:
        {
            break;
        }
        case Opt::SUB_NUMBER_FROM_REG:
        {
            break;
        }

        case Opt::INC_REG:
        {
            unsigned int reg = readInt(memory, isp);
            registers[reg]++;
            break;
        }

        case Opt::DEC_REG:
        {
            unsigned int reg = readInt(memory, isp);
            registers[reg]--;
            break;
        }

        case Opt::CMP_REG_WITH_REG:
        {
            break;
        }
        case Opt::CMP_REGADDRESS_WITH_REG:
        {
            break;
        }
        case Opt::CMP_ADDRESS_WITH_REG:
        {
            break;
        }
        case Opt::CMP_NUMBER_WITH_REG:
        {
            unsigned int number = readInt(memory, isp);
            unsigned int value = registers[readInt(memory, isp)];
            zero = number == value;
            carry = number < value;
            break;
        }

        case Opt::JMP_REGADDRESS:
        {
            break;
        }
        case Opt::JMP_ADDRESS:
        {
            unsigned int address = readInt(memory, isp);
            isp = address;
            break;
        }

        case Opt::JC_REGADDRESS:
        {
            break;
        }
        case Opt::JC_ADDRESS:
        {
            unsigned int address = readInt(memory, isp);
            if (carry)
                isp = address;
            break;
        }

        case Opt::JNC_REGADDRESS:
        {
            break;
        }
        case Opt::JNC_ADDRESS:
        {
            unsigned int address = readInt(memory, isp);
            if (!carry)
                isp = address;
            break;
        }

        case Opt::JZ_REGADDRESS:
        {
            break;
        }
        case Opt::JZ_ADDRESS:
        {
            unsigned int address = readInt(memory, isp);
            if (zero)
                isp = address;
            break;
        }

        case Opt::JNZ_REGADDRESS:
        {
            break;
        }
        case Opt::JNZ_ADDRESS:
        {
            unsigned int address = readInt(memory, isp);
            if (!zero)
                isp = address;
            break;
        }

        case Opt::JA_REGADDRESS:
        {
            break;
        }
        case Opt::JA_ADDRESS:
        {
            unsigned int address = readInt(memory, isp);
            if (!zero && !carry)
                isp = address;
            break;
        }

        case Opt::JNA_REGADDRESS:
        {
            break;
        }
        case Opt::JNA_ADDRESS:
        {
            unsigned int address = readInt(memory, isp);
            if (zero != carry)
                isp = address;
            break;
        }

        case Opt::PUSH_REG:
        {
            break;
        }
        case Opt::PUSH_REGADDRESS:
        {
            break;
        }
        case Opt::PUSH_ADDRESS:
        {
            break;
        }
        case Opt::PUSH_NUMBER:
        {
            break;
        }

        case Opt::POP_REG:
        {
            break;
        }

        case Opt::CALL_REGADDRESS:
        {
            break;
        }
        case Opt::CALL_ADDRESS:
        {
            break;
        }

        case Opt::RET:
        {
            break;
        }

        case Opt::MUL_REG:
        {
            break;
        }
        case Opt::MUL_REGADDRESS:
        {
            break;
        }
        case Opt::MUL_ADDRESS:
        {
            break;
        }
        case Opt::MUL_NUMBER:
        {
            break;
        }

        case Opt::DIV_REG:
        {
            break;
        }
        case Opt::DIV_REGADDRESS:
        {
            break;
        }
        case Opt::DIV_ADDRESS:
        {
            break;
        }
        case Opt::DIV_NUMBER:
        {
            break;
        }

        case Opt::AND_REG_WITH_REG:
        {
            break;
        }
        case Opt::AND_REGADDRESS_WITH_REG:
        {
            break;
        }
        case Opt::AND_ADDRESS_WITH_REG:
        {
            break;
        }
        case Opt::AND_NUMBER_WITH_REG:
        {
            break;
        }

        case Opt::OR_REG_WITH_REG:
        {
            break;
        }
        case Opt::OR_REGADDRESS_WITH_REG:
        {
            break;
        }
        case Opt::OR_ADDRESS_WITH_REG:
        {
            break;
        }
        case Opt::OR_NUMBER_WITH_REG:
        {
            break;
        }

        case Opt::XOR_REG_WITH_REG:
        {
            break;
        }
        case Opt::XOR_REGADDRESS_WITH_REG:
        {
            break;
        }
        case Opt::XOR_ADDRESS_WITH_REG:
        {
            break;
        }
        case Opt::XOR_NUMBER_WITH_REG:
        {
            break;
        }

        case Opt::NOT_REG:
        {
            break;
        }

        case Opt::SHL_REG_WITH_REG:
        {
            break;
        }
        case Opt::SHL_REGADDRESS_WITH_REG:
        {
            break;
        }
        case Opt::SHL_ADDRESS_WITH_REG:
        {
            break;
        }
        case Opt::SHL_NUMBER_WITH_REG:
        {
            break;
        }

        case Opt::SHR_REG_WITH_REG:
        {
            break;
        }
        case Opt::SHR_REGADDRESS_WITH_REG:
        {
            break;
        }
        case Opt::SHR_ADDRESS_WITH_REG:
        {
            break;
        }
        case Opt::SHR_NUMBER_WITH_REG:
        {
            break;
        }

        default:
        {
            break;
        }
        }
    }
    std::cout << "\e[?25h\n";
}

int main(int argc, char **argv)
{
    loop();
    return 0;
}
