#ifndef _PARSER_HPP
#define _PARSER_HPP

#include <fstream>
#include <vector>
#include <memory>
#include "symboltable.hpp"
#include "instruction.hpp"

namespace hack {
namespace assembler {

class Parser {
  private:
    std::ifstream &is;
    std::ofstream &os;
    std::vector<std::unique_ptr<Instruction>> instructions;

    Instruction *parse_line(std::string&);
    SymbolTable symtbl;

    Instruction *parse_ainstr(std::string&);
    Instruction *parse_cinstr(std::string&);
    void parse_linstr(std::string&);

  public:
    Parser(std::ifstream &is, std::ofstream &os) : is(is), os(os), symtbl(SymbolTable()) {}

    /*
    ** 1. Walks through `is`, pushing instructions in `instructions`
    ** 2. Detects and saves the Instruction Memory location of goto-labels
    */
    void parse_pass1();

    /*
    ** Walks through `instructions` and emits the binary code to `os`.
    ** Symbol handling is done by checking if the symbol was defined in pass1 (goto-labels),
    ** and if not, handle the symbol as a "variable symbol".
    */
    void parse_pass2();
};

}
}

#endif
