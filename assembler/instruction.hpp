#ifndef _INSTRUCTION_HPP
#define _INSTRUCTION_HPP

#include <string>
#include <algorithm>
#include <map>
#include <stdint.h>

namespace hack {
namespace assembler {

class SymbolTable;

class Instruction {
  public:
    virtual uint16_t to_binary(SymbolTable&) = 0;
};

/*
** An AInstruction is either a constant uint16_t value, or 
** a label that is yet to be resolved to a memory location.
*/
class AInstruction : public Instruction {
  private:
    uint16_t address;
    std::string label;

  public:
    AInstruction(uint16_t address) : address(address) {}
    AInstruction(std::string label) : label(label) {}

    AInstruction(AInstruction &&other) {
        std::swap(address, other.address);
        std::swap(label, other.label);
    }

    virtual uint16_t to_binary(SymbolTable&);
};

/*
** To have a type-safe way of referencing the different types of instructions
*/
enum class Dest {
    null,
    M,
    D,
    DM,
    A,
    AM,
    AD,
    ADM
};

enum class Jump {
    null,
    JGT,
    JEQ,
    JGE,
    JLT,
    JNE,
    JLE,
    JMP
};

enum class Comp {
    zero        = 0b0101010,
    one         = 0b0111111,
    negone      = 0b0111010,
    D           = 0b0001100,
    A           = 0b0110000,
    M           = 0b1110000,
    notD        = 0b0001101,
    notA        = 0b0110001,
    notM        = 0b1110001,
    negD        = 0b0001111,
    negA        = 0b0110011,
    negM        = 0b1110011,
    Dplus1      = 0b0011111,
    Aplus1      = 0b0110111,
    Mplus1      = 0b1110111,
    Dmin1       = 0b0001110,
    Amin1       = 0b0110010,
    Mmin1       = 0b1110010,
    DplusA      = 0b0000010,
    DplusM      = 0b1000010,
    DminA       = 0b0010011,
    DminM       = 0b1010011,
    AminD       = 0b0000111,
    MminD       = 0b1000111,
    DandA       = 0b0000000,
    DandM       = 0b1000000,
    DorA        = 0b0010101,
    DorM        = 0b1010101
};

/*
** Encapsulates an CInstruction, which has the assembly form of dest=comp;jump.
*/
class CInstruction : public Instruction {
  private:
    Jump jump;
    Dest dest;
    Comp comp;

  public:
    CInstruction(Jump jump, Dest dest, Comp comp) : jump(jump), dest(dest), comp(comp) {}
    CInstruction(CInstruction &&other) {
        std::swap(jump, other.jump);
        std::swap(dest, other.dest);
        std::swap(comp, other.comp);
    }

    virtual uint16_t to_binary(SymbolTable&);

    static std::map<std::string, Dest> destmap;
    static std::map<std::string, Jump> jumpmap;
    static std::map<std::string, Comp> compmap;
};

}
}

#endif
