#include <cstdio>
#include <fstream>

int main() {
    std::string filename{"test_bstream_input"};

  const unsigned char val[] = {
    0x58, 0x90, 0xab, 0x08,
    0x00, 0x4e, 0xdb, 0x40,
  };
  // Equivalent in binary is:
  // 0101100010010000101010110000100000000000010011101101101101000000
  // ^5  ^8  ^9  ^0  ^a  ^b  ^0  ^8  ^0  ^0  ^4  ^e  ^d  ^b  ^4  ^0

  // Write this to a file
  std::ofstream ofs(filename, std::ios::out |
                    std::ios::trunc |
                    std::ios::binary);
  ofs.write(reinterpret_cast<const char *>(val), sizeof(val));
  ofs.close();
}
