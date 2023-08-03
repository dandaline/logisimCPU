/**
 * @file ALUtest.cpp
 * @author Laila Staschenuk, lailastaschenuk@outlook.com
 * @version 0.1
 * @date 2022-05-31
 */
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <vector>
#include <ctime>
#include <stdint.h>

// loads each command 28 times into one vector (28*9=253 commands in total)
std::vector<int> cmdCode()
{
    std::vector<int> a;
    for (int i = 0; i < 16; i++)
    {
        if ((i != 2) && (i != 3) && (i != 4) && (i != 6) && (i != 7) && (i != 10) && (i != 13)) // i.e. 0 will result in true, therefor 0 will be loaded into vektor for addition
        {
            for (int j = 0; j < 28; j++)
            {
                a.push_back(i);
            }
        }
    }
    return a;
}

// two's compliment
uint8_t twosComplement(uint8_t val)
{
    return -(unsigned int)val;
}

// write vector to file
void write(std::vector<int> &v, std::ofstream &f)
{
    for (int i = 0; i < v.size(); i++)
    {
        f << std::hex << v[i];
        f << " ";
    }
}

// gerenate random numbers for lhs and rhs, set flags and combine flags with result
void makeCalc(std::vector<int> &c, std::vector<int> &r, std::vector<int> &o)
{
    int lhs, rhs, res, modRhs = 0;
    for (int i = 0; i < c.size(); i++)
    {
        lhs = rand() % 0xff;
        rhs = rand() % 0xff;
        switch (c.at(i))
        {
        case 0: // addition
            res = lhs + rhs;
            if (res > 0xff)
            {
                res = (res & 0xff) + 0x0200;
            }
            break;
        case 1: // subtraction
            res = lhs - rhs;
            if (res < 0x0)
            {
                res = (res & 0xff) + 0x0400;
            }
            break;
        case 5: // test
            res = lhs - rhs;
            if (res < 0x0)
            {
                res = (res & 0xff) + 0x0400;
            }
            else if (res == 0x0)
            {
                res = (res & 0xff) + 0x0100;
            }
            break;
        case 8: // AND
            res = lhs & rhs;
            break;
        case 9: // OR
            res = lhs | rhs;
            break;
        case 11: // NEG aka two's compliment
            res = twosComplement(lhs);
            res = res & 0xff;
            break;
        case 12: // XOR
            res = lhs ^ rhs;
            break;
        case 14: // shift left
            modRhs = rhs & 0b111;
            res = lhs << modRhs;
            res = res & 0xff;
            break;
        case 15: // shift right
            modRhs = rhs & 0b111;
            res = lhs >> modRhs;
            res = res & 0xff;
            break;
        default:
            std::cout << "FATAL ERROR: Command not found!\n";
            break;
        }
        res = res & 0xffff;
        lhs = (lhs << 0x8) + rhs;
        std::cout << std::hex << c[i] << " " << lhs << " " << res << "\n";
        o.push_back(lhs);
        r.push_back(res);
    }
}

int main()
{
    // create vectors
    std::vector<int> voperands;
    std::vector<int> vcmd;
    std::vector<int> vres;
    // create text files and format for logisim 2.*
    std::ofstream operands("operands.txt");
    operands << "v2.0 raw\n";
    std::ofstream cmd("cmd.txt");
    cmd << "v2.0 raw\n";
    std::ofstream res("exRes.txt");
    res << "v2.0 raw\n";
    // initiate seed from clock
    srand(time(0));
    // assign commands to command vector
    vcmd = cmdCode();
    // do calculations, add results and flags to vectors
    makeCalc(vcmd, vres, voperands);
    // write vectors to files
    write(vcmd, cmd);
    write(voperands, operands);
    write(vres, res);
    // close files
    operands.close();
    cmd.close();
    res.close();
}