#include <iostream>
#include <fstream>
#include <string>

#include "huffman.h"



int main(int argc, char *argv[]) {
    if (argc != 3) {
        std:: cerr << "Usage: " << argv[0] << " <inputfile> <zapfile>" << std::endl;
        exit(1);
    }
    std::string in_file = argv[1];
    std::string out_file = argv[2];
    std::ifstream ifs(in_file, std::ios::in);
    if (!ifs.is_open()) {
        std::cerr << "Error: cannot open input file " << in_file << std::endl;
        exit(1);
    }
    std::ofstream ofs(out_file,
                      std::ios::out | std::ios::trunc | std::ios::binary);
    Huffman huf;
    huf.Compress(ifs, ofs);
    ifs.close();
    ofs.close();
}
