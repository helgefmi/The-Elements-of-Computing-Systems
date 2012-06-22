#include <sstream>
#include <algorithm>
#include <iostream>
#include <string>
#include <stdexcept>
#include <cctype>
#include "parser.hpp"
#include "instruction.hpp"

using namespace std;
using namespace hack::assembler;

namespace hack {
namespace assembler {

namespace util {
    uint16_t str2int(string& str) {
        uint16_t ret;
        stringstream(str) >> ret;
        return ret;
    }

    bool is_label(string &str) {
        return !str.empty() && isalpha(str[0]) && all_of(begin(str) + 1, end(str), [](char c) {
            return isalnum(c) || c == '.' || c == '$' || c == ':' || c == '_';
        });
    }

    string dec2bin16(uint16_t t) {
        string ret(16, '0');
        for (int i = 0; i < 16; ++i) {
            if (t & (1 << i)) {
                ret[15 - i] = '1';
            }
        }
        return ret;
    }
}

Instruction *Parser::parse_ainstr(string &line) {
    auto label = line.substr(1);
    if (label.empty()) {
        throw runtime_error("Empty A instruction");
    }

    /* It's either all digits (\d+) */
    if (all_of(begin(label), end(label), [](char c) { return isdigit(c); })) {
        return new AInstruction(util::str2int(label));
    }
    /* Or a label ([a-zA-Z][a-zA-Z0-9.$:_]*) */
    else {
        if (!util::is_label(label)) {
            throw runtime_error("Invalid A instruction; invalid format on label: " + label);
        }
        return new AInstruction(label);
    }
}

Instruction *Parser::parse_cinstr(string &line) {
    Dest dest = Dest::null;
    Jump jump = Jump::null;
    string org_line(line); // Since we're messing with `line`, it's good to have the original for the exceptions

    /* Just so we can assume that there's only one of ; and = in the rest of this function. */
    if (count(begin(line), end(line), ';') > 1 || count(begin(line), end(line), '=') > 1) {
        throw runtime_error("Invalid C instruction; found more than one of ; or =: " + line);
    }

    /* Parse `dest` */
    auto eq_idx = line.find('=');
    if (eq_idx != string::npos) {
        auto dstr = line.substr(0, eq_idx);

        sort(dstr.begin(), dstr.end()); // Sorting so we don't have to put all the combinations into `destmap`
        if (CInstruction::destmap.count(dstr) == 0) {
            throw runtime_error("Invalid C instruction: " + org_line + ", couldn't parse `dest`: " + dstr);
        }

        dest = CInstruction::destmap[dstr];
        line = line.substr(eq_idx + 1);
    }

    /* Parse `jump` */
    auto sc_idx = line.find(';');
    if (sc_idx != string::npos) {
        auto jstr = line.substr(sc_idx + 1);
        if (CInstruction::jumpmap.count(jstr) == 0) {
            throw runtime_error("Invalid C instruction: " + org_line + ", couldn't parse `jump`: " + jstr);
        }

        jump = CInstruction::jumpmap[jstr];
        line = line.substr(0, sc_idx);
    }

    /* Parse `comp` */
    if (CInstruction::compmap.count(line) == 0) {
        throw runtime_error("Invalid C instruction: " + org_line + ", couldn't parse `comp`: " + line);
    }
    Comp comp = CInstruction::compmap[line];

    return new CInstruction(jump, dest, comp);
}

void Parser::parse_linstr(string &line) {
    if (line[0] != '(' || line[line.size() - 1] != ')') {
        throw runtime_error("Invalid L instruction; label doesn't seem to be between ( and ): " + line);
    }

    auto label = line.substr(1, line.size() - 2); // Might be empty.
    if (!util::is_label(label)) {
        throw runtime_error("Invalid L instruction; invalid format on label: " + label);
    }

    /*
    ** Sets the label to point to the current place in ROM memory.
    ** This works since each instruction occupies exactly 1 element of the memory.
    */
    symtbl.set(label, instructions.size());
}


Instruction *Parser::parse_line(string &line) {
    /* Remove comments */
    line.erase(search_n(begin(line), end(line), 2, '/'), end(line));

    /* Remove whitespace */
    auto new_end = remove_if(begin(line), end(line), [](const char c) { return isspace(c); });
    line.erase(new_end, line.end());

    if (!line.empty()) {
        if (line[0] == '@') {
            return parse_ainstr(line);
        }
        else if (line[0] == '(') {
            parse_linstr(line);
        }
        else {
            return parse_cinstr(line);
        }
    }
    return nullptr;
}

void Parser::parse_pass1() {
    while (!is.eof()) {
        string line;
        getline(is, line);

        Instruction *instr = parse_line(line);
        if (instr) {
            instructions.push_back(unique_ptr<Instruction>(instr));
        }
    }
}

void Parser::parse_pass2() {
    for (auto &i : instructions) {
        uint16_t bin = i->to_binary(symtbl);
        os << util::dec2bin16(bin) << endl;
    }
}

}
}
