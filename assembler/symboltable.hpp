#ifndef _SYMBOLTABLE_HPP
#define _SYMBOLTABLE_HPP

#include <stdint.h>
#include <string>
#include <map>

namespace hack {
namespace assembler {

class SymbolTable {
  private:
    std::map<std::string, uint16_t> symbols;
    uint16_t _next_variable_address;

  public:
    SymbolTable() : _next_variable_address(16) {
        symbols["SP"]       = 0x0000;
        symbols["LCL"]      = 0x0001;
        symbols["ARG"]      = 0x0002;
        symbols["THIS"]     = 0x0003;
        symbols["THAT"]     = 0x0004;
        symbols["SCREEN"]   = 0x4000;
        symbols["KBD"]      = 0x6000;

        symbols["R0"] = 0;
        symbols["R1"] = 1;
        symbols["R2"] = 2;
        symbols["R3"] = 3;
        symbols["R4"] = 4;
        symbols["R5"] = 5;
        symbols["R6"] = 6;
        symbols["R7"] = 7;
        symbols["R8"] = 8;
        symbols["R9"] = 9;
        symbols["R10"] = 10;
        symbols["R11"] = 11;
        symbols["R12"] = 12;
        symbols["R13"] = 13;
        symbols["R14"] = 14;
        symbols["R15"] = 15;
    }

    bool has(std::string&);
    void set(std::string&, uint16_t);
    uint16_t get_or_create(std::string&);
};

}
}
#endif
