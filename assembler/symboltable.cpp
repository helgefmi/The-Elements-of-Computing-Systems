#include <stdexcept>
#include "symboltable.hpp"

using namespace std;
using namespace hack::assembler;

namespace hack {
namespace assembler {

bool SymbolTable::has(std::string &label) {
    return symbols.count(label) == 1;
}

void SymbolTable::set(std::string &label, uint16_t memory) {
    if (has(label)) {
        throw runtime_error("Tried to set predefined label");
    }
    symbols[label] = memory;
}

uint16_t SymbolTable::get_or_create(std::string &label) {
    if (!has(label)) {
        symbols[label] = _next_variable_address++;
    }
    return symbols[label];
}

}
}
