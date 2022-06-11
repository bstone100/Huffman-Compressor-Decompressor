#include <gtest/gtest.h>

#include <cstdio>
#include <fstream>

#include "bstream.h"

TEST(BStream, input) {
    std::string filename{"test_bstream_input"};

    const unsigned char val[] = {
        0x58, 0x90, 0xab, 0x08, 0x00, 0x4e, 0xdb, 0x40,
    };
    // Equivalent in binary is:
    // 0 1 01100010 01000010 1 01011000-01000000-0000010-01110110 11011010 00000
    // ^5  ^8  ^9  ^0  ^a  ^b  ^0  ^8  ^0  ^0  ^4  ^e  ^d  ^b  ^4  ^0

    // Write this to a file
    std::ofstream ofs(filename,
                      std::ios::out | std::ios::trunc | std::ios::binary);
    ofs.write(reinterpret_cast<const char *>(val), sizeof(val));
    ofs.close();

    // Read it back in binary format
    std::ifstream ifs(filename, std::ios::in | std::ios::binary);
    BinaryInputStream bis(ifs);

    // Make sure that we reading the binary in the correct order
    EXPECT_EQ(bis.GetBit(), 0);
    EXPECT_EQ(bis.GetBit(), 1);
    EXPECT_EQ(bis.GetChar(), 0x62);  // 01100010
    EXPECT_EQ(bis.GetChar(), 0x42);  // 01000010
    EXPECT_EQ(bis.GetBit(), 1);
    EXPECT_EQ(bis.GetInt(), 0x58400276);  // 01011000-01000000-00000010-01110110

    ifs.close();

    std::remove(filename.c_str());
}

TEST(BStream, output) {
    std::string filename{"test_bstream_output"};

    std::ofstream ofs(filename,
                      std::ios::out | std::ios::trunc | std::ios::binary);

    BinaryOutputStream bos(ofs);

    bos.PutChar('A');  // 0x41
    bos.PutChar('A');  // 0x41
    bos.PutChar('N');  // 0x4e
    bos.PutChar('G');  // 0x47

    bos.PutInt(11);  // 0x0b
    bos.PutInt(22);  // 0x16
    bos.PutInt(33);  // 0x21
    bos.PutInt(44);  // 0x2c

    ofs.close();

    unsigned char val[20];
    std::ifstream ifs(filename, std::ios::in | std::ios::binary);
    ifs.read(reinterpret_cast<char *>(val), sizeof(val));
    ifs.close();

    EXPECT_EQ(val[0], 0x41);
    EXPECT_EQ(val[1], 0x41);
    EXPECT_EQ(val[2], 0x4e);
    EXPECT_EQ(val[3], 0x47);

    EXPECT_EQ(val[7], 0x0b);
    EXPECT_EQ(val[11], 0x16);
    EXPECT_EQ(val[15], 0x21);
    EXPECT_EQ(val[19], 0x2c);

    std::remove(filename.c_str());
}

TEST(BStream, inputoutput) {
    std::string filename{"test_bstream_output"};

    std::ofstream ofs(filename,
                      std::ios::out | std::ios::trunc | std::ios::binary);

    BinaryOutputStream bos(ofs);

    bos.PutChar('Z');  // 0x5a
    bos.PutChar('U');  // 0x55
    bos.PutChar('K');  // 0x4b
    bos.PutChar('O');  // 0x4f
    bos.PutInt(11);    // 0x0b
    bos.Close();

    ofs.close();

    std::ifstream ifs(filename, std::ios::in | std::ios::binary);
    BinaryInputStream bis(ifs);

    EXPECT_EQ(bis.GetChar(), 0x5a);
    EXPECT_EQ(bis.GetChar(), 0x55);
    EXPECT_EQ(bis.GetChar(), 0x4b);
    EXPECT_EQ(bis.GetChar(), 0x4f);
    EXPECT_EQ(bis.GetInt(), 0x0b);

    ifs.close();

    std::remove(filename.c_str());
}

TEST(BStream, bitinputoutput) {
    std::string filename{"test_bstream_output"};

    std::ofstream ofs(filename,
                      std::ios::out | std::ios::trunc | std::ios::binary);

    BinaryOutputStream bos(ofs);

    bos.PutBit(0);
    bos.PutBit(1);
    bos.PutChar('A');
    bos.Close();

    ofs.close();

    std::ifstream ifs(filename, std::ios::in | std::ios::binary);
    BinaryInputStream bis(ifs);

    EXPECT_EQ(bis.GetChar(), 0x50);
    EXPECT_EQ(bis.GetChar(), 0x40);

    ifs.close();
    std::remove(filename.c_str());
}

int main(int argc, char *argv[]) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
