#include <vector>
#include <string>
#include "instruction.hpp"
#include "symboltable.hpp"

using namespace std;

namespace hack {
namespace assembler {

uint16_t CInstruction::to_binary(SymbolTable&) {
    auto jumpint = static_cast<int>(jump);
    auto destint = static_cast<int>(dest);
    auto compint = static_cast<int>(comp);
    return jumpint | (destint << 3) | (compint << 6) | (0b011 << 13) | (1 << 15);
}

uint16_t AInstruction::to_binary(SymbolTable &table) {
    if (!label.empty()) {
        return table.get_or_create(label);
    }
    else {
        return address;
    }
}

/* Initializer functions for CInstruction::jumpmap and CInstruction::destmap */
static map<string, Jump> create_jumpmap() {
    return {
        {"JGT", Jump::JGT},
        {"JEQ", Jump::JEQ},
        {"JGE", Jump::JGE},
        {"JLT", Jump::JLT},
        {"JNE", Jump::JNE},
        {"JLE", Jump::JLE},
        {"JMP", Jump::JMP},
    };
}

static map<string, Dest> create_destmap() {
    return {
        {"M", Dest::M},
        {"D", Dest::D},
        {"DM", Dest::DM},
        {"A", Dest::A},
        {"AM", Dest::AM},
        {"AD", Dest::AD},
        {"ADM", Dest::ADM},
    };
}

static map<string, Comp> create_compmap() {
    return {
        {"0", Comp::zero},
        {"1", Comp::one},
        {"-1", Comp::negone},
        {"D", Comp::D},
        {"A", Comp::A},
        {"M", Comp::M},
        {"!D", Comp::notD},
        {"!A", Comp::notA},
        {"!M", Comp::notM},
        {"-D", Comp::negD},
        {"-A", Comp::negA},
        {"-M", Comp::negM},
        {"D+1", Comp::Dplus1},
        {"A+1", Comp::Aplus1},
        {"M+1", Comp::Mplus1},
        {"D-1", Comp::Dmin1},
        {"A-1", Comp::Amin1},
        {"M-1", Comp::Mmin1},
        {"D+A", Comp::DplusA},
        {"D+M", Comp::DplusM},
        {"D-A", Comp::DminA},
        {"D-M", Comp::DminM},
        {"A-D", Comp::AminD},
        {"M-D", Comp::MminD},
        {"D&A", Comp::DandA},
        {"D&M", Comp::DandM},
        {"D|A", Comp::DorA},
        {"D|M", Comp::DorM},
    };
}

map<string, Jump> CInstruction::jumpmap = create_jumpmap();
map<string, Dest> CInstruction::destmap = create_destmap();
map<string, Comp> CInstruction::compmap = create_compmap();

}
}
