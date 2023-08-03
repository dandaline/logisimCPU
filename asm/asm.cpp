/**
 * @file asm.cpp
 * @author Laila Staschenuk, lailastaschenuk@outlook.com
 * @version 1.2
 * @date 2022-07-01
 */
#include <iostream>
#include <fstream>
#include <map>
#include <sstream>
#include <math.h>

std::map<std::string, unsigned short int> reg{
    {"PC", 0b000},
    {"RA", 0b001},
    {"RB", 0b010},
    {"RC", 0b011},
    {"RD", 0b100},
    {"RE", 0b101},
    {"RF", 0b110},
    {"RG", 0b111}};

std::map<std::string, unsigned short int> operations{
    {"nop", 0x0},
    {"st", 0x8}, // MEMORY
    {"ld", 0x9},
    {"ldRomL", 0xa},
    {"ldRomH", 0xb},
    {"asciiIOL", 0xc},
    {"asciiIOH", 0xd},
    {"ldi", 0x40}, // CONSTANTS
    {"mov", 0x48},
    {"jz", 0x50}, // JUMPS
    {"jnz", 0x51},
    {"jc", 0x52},
    {"jnc", 0x53},
    {"jzr", 0x54},
    {"jnzr", 0x55},
    {"jnr", 0x56},
    {"jnnr", 0x57},
    {"add", 0x70}, // ALU
    {"sub", 0x71},
    {"inc", 0x72},
    {"dec", 0x73},
    {"mult", 0x74},
    {"tst", 0x75},
    {"&", 0x78},
    {"|", 0x79},
    {"!", 0x7a},
    {"~", 0x7b},
    {"^", 0x7c},
    {"<<", 0x7e},
    {">>", 0x7f}};

// assign constant to bits 15 - 8
void constAssign(unsigned short int &o, unsigned short int c)
{
    o += c << 8;
}

// assign register to position in instruction word bits 15 - 7
void regAssign(unsigned short int &o, bool r1, bool r2, bool r3, unsigned short int vr1, unsigned short int vr2, unsigned short int vr3)
{
    if (r1)
    {
        o += vr1 << 13;
    }
    if (r2)
    {
        o += vr2 << 10;
    }
    if (r3)
    {
        o += vr3 << 7;
    }
}

// write array to file
void writeFile(unsigned short int aRom[], size_t count, std::ofstream &outFile)
{
    for (int i = 0; i < count; i++)
    {
        outFile << std::hex << aRom[i];
        outFile << " ";
    }
}

// generates all 128 ASCII symbols and assigns them as pairs to position 192 throughout 255 in array
void generateASCII(unsigned short int aRom[], size_t count)
{
    int low = 0;
    int high = 1;
    for (int i = 192; i < count; i++)
    {
        aRom[i] = (low + (high << 8));
        low += 2;
        high += 2;
    }
}

void stringToRom(unsigned short int aRom[])
{
    std::string strings = "Errate Zahl [0...127]:\n Richtig :) \nFast, gleich nochmal!\n:( Your PC ran into a problem and needs to restart!\n";
    int low = 0;
    int high = 1;
    for (int i = 192; i < 192 + ceil(strings.size() / 2); i++)
    {
        if (low + 1 <= strings.size())
        {
            aRom[i] = strings[low];
            low += 2;
        }
        if (high + 1 <= strings.size())
        {
            aRom[i] += (strings[high] << 8);
            high += 2;
        }
    }
}

/** works with one line at a time and scans through the words separated by white space
 * TO MODIFY THE LANGUAGE AND EXPAND YOUR CPU'S INSTRUCTION SET
 * 1. add pair of operation and number to map datastructure "operations" (line22)
 * 2. add switch case with according number
 * 3. add syntax to taste by streaming word by word and assigning to registers or constants
 *
 * HOW THIS FUNCTION WORKS
 * 1. switch case is determined by instruction mnemonic at beginning of each line
 * 2. mnemonic is added to instruction word
 * 3. constants and/or registers are added by reading the next expected words from a line
 * The syntax is very strict for allows comments after last ort to interpret (i.e. st RA RB my comment -> "my" and "comment" are not part of the stream and will be ignored)
 * 4. Assign fuctions give registers/constants their position within instruction word
 * 5. instruction word "op" is returned
 */
unsigned short int interpreter(const std::string &l, int atLine)
{
    unsigned short int op = 0b0;
    std::stringstream stream(l);
    std::string keyWord;
    stream >> keyWord;
    unsigned short int mne = operations[keyWord];
    op = mne;
    {
        std::string r1, r2, r3;
        std::string constant;
        switch (mne)
        {
        case 0x0:
            break;
        case 0x8: // store
            stream >> r2;
            stream >> r1;
            regAssign(op, 1, 1, 0, reg[r1], reg[r2], reg[r3]);
            break;
        case 0x9: // load
            stream >> r3;
            stream >> r2;
            regAssign(op, 0, 1, 1, reg[r1], reg[r2], reg[r3]);
            break;
        case 0xa ... 0xb: // loadRomL/H
            stream >> r3;
            stream >> r1;
            regAssign(op, 1, 0, 1, reg[r1], reg[r2], reg[r3]);
            break;
        case 0xc ... 0xd:
            stream >> r1;
            stream >> r2;
            regAssign(op, 1, 1, 0, reg[r1], reg[r2], reg[r3]);
            break;
        case 0x40 ... 0x47: // LDI
            stream >> constant;
            stream >> r3;
            constAssign(op, stoi(constant, 0, 16)); // convert hex string to integer
            op += reg[r3];
            break;
        case 0x48: // move
            stream >> r1;
            stream >> r3;
            regAssign(op, 1, 0, 1, reg[r1], reg[r2], reg[r3]);
            break;
        case 0x50 ... 0x57: // Jumps
            stream >> constant;
            constAssign(op, stoi(constant, 0, 16)); // convert hex string to integer
            break;
        case 0x70 ... 0x7f:                                                       // ALU
            if ((mne == 0x7a) || (mne == 0x7b || (mne == 0x72) || (mne == 0x73))) // unary operations
            {
                stream >> r2;
                stream >> r3;
                regAssign(op, 0, 1, 1, reg[r1], reg[r2], reg[r3]);
            }
            else // binary operations
            {
                stream >> r1;
                stream >> r2;
                stream >> r3;
                regAssign(op, 1, 1, 1, reg[r1], reg[r2], reg[r3]);
            }
            break;
        default:
            std::cout << "FATAL ERROR: mnemonic conversion failed at line: "
                      << atLine + 1 << " op-code: " << op << "\n";
            break;
        }
        std::cout << std::dec << atLine + 1 << std::hex << " op-code: " << op << "\n";
        return op;
    }
}

// reads out input file and passes each line one by one to interpreter function
int lineExtract(std::istream &programFile, unsigned short int aaRom[])
{
    std::string line;
    size_t i = 0;
    while (programFile)
    {
        std::getline(programFile, line);
        if (line != "")
        {
            aaRom[i] = interpreter(line, i);
            i++;
        }
    }
    std::cout << "total instruction count: " << std::dec << i << " /(256-64) \n";
    return i;
}

int main()
{
    std::cout << "\nRUN ASSAMBLER\n";
    // read program to string
    std::string programName;
    std::cout << "enter full programm name with .txt extension: ";
    std::cin >> programName;
    std::cout << std::endl;
    // std::ifstream programFile("program.txt");
    std::ifstream programFile(programName);

    // create ROM-Array
    unsigned short int rom[0x100] = {0};

    // create text file and format for logisim 2.*
    std::ofstream romFile("rom.txt");
    romFile << "v2.0 raw\n";

    // generate ASCII in array, IF YOU DON'T WANT ASCII SYMBOLS OR NOT ALL OF THEM, COMMENT OUT THIS FUNCTION TO OMMIT
    // generateASCII(rom, sizeof(rom) / sizeof(unsigned short int));
    stringToRom(rom);

    // interpret code and compile to hex file
    int InstrCount = lineExtract(programFile, rom);
    writeFile(rom, sizeof(rom) / sizeof(unsigned short int), romFile);

    // close files
    romFile.close();
    std::cout << "\nASSAMBLER SUCCESS:\nrom.txt file with " << InstrCount << " instructions from addr 0x0 to 0xbf\n"
              << "all 128 ASCII symbols stored from 0xc0 to 0xff in 2 byte pairs\n";
}