#include <iostream>
#include <utility>
#include <fstream>

#include "parser.hpp"

using namespace std;
using namespace hack::assembler;

pair<string,string> split_extension(string& filename) {
    auto idx = filename.rfind('.');
    if (idx == string::npos) {
        return make_pair(filename, "");
    }
    else {
        auto dotiter = filename.begin() + idx;
        return make_pair(
            string(filename.begin(), dotiter),
            string(dotiter + 1, filename.end())
        );
    }
}

int main(int argc, char **argv) {
    if (argc < 2) {
        cerr << "./main <input_file.asm>" << endl;
        return EXIT_FAILURE;
    }

    string input_path(argv[1]);
    auto input_splitted = split_extension(input_path);

    if (input_splitted.second != "asm") {
        cerr << "input file name must have extension .asm";
        return EXIT_FAILURE;
    }

    string output_path(input_splitted.first + ".hack");
    ifstream is(input_path);
    ofstream os(output_path);

    Parser p(is, os);
    p.parse_pass1();
    p.parse_pass2();

    return 0;
}
