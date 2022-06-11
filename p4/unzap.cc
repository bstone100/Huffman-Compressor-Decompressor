#include <iostream>
#include <fstream>
#include <string>

#include "huffman.h"



int main(int argc, char *argv[]) {
    if (argc != 3) {
        std:: cerr << "Usage: " << argv[0] << " <zapfile> <outputfile>" << std::endl;
        exit(1);
    }
    std::string in_file = argv[1];
    std::string out_file = argv[2];
    std::ifstream ifs(in_file, std::ios::in | std::ios::binary);
    if (!ifs.is_open()) {
        std::cerr << "Error: cannot open zap file " << in_file << std::endl;
        exit(1);
    }
    std::ofstream ofs(out_file,
                      std::ios::out | std::ios::trunc);
    Huffman huf;
    huf.Decompress(ifs, ofs);
    ifs.close();
    ofs.close();
}
