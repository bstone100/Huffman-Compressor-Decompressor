#ifndef HUFFMAN_H_
#define HUFFMAN_H_

#include <array>
#include <cstddef>
#include <cctype>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <bitset>
#include <map>

#include "bstream.h"
#include "pqueue.h"

class HuffmanNode {
 public:
  explicit HuffmanNode(char ch, size_t freq,
                       HuffmanNode *left = nullptr,
                       HuffmanNode *right = nullptr)
      : ch_(ch), freq_(freq), left_(left), right_(right) { }


  bool IsLeaf() {
    // Node is a leaf if it doesn't have any children
    return left_ == nullptr && right_ == nullptr;
  }

  bool operator < (const HuffmanNode &n) const {
    // In case of equality, make it deterministic based on character
    if (freq_ == n.freq_)
      return ch_ < n.ch_;
    // Otherwise compare frequencies
    return freq_ < n.freq_;
  }

  size_t freq() { return freq_; }
  char data() { return ch_; }
  HuffmanNode* left() { return left_; }
  HuffmanNode* right() { return right_; }

 private:
  char ch_;
  size_t freq_;
  HuffmanNode *left_, *right_;
};

class Huffman {
 public:
  static void Compress(std::ifstream &ifs, std::ofstream &ofs);

  static void Decompress(std::ifstream &ifs, std::ofstream &ofs);

 private:
  // Helper methods...

  static void PrintPreorder(HuffmanNode *h, BinaryOutputStream &bos) {
    if (h == nullptr) {
      return;
    }
    if (h->IsLeaf() == false) {
      // 0 for not leaf
      bos.PutBit(0);
      PrintPreorder(h->left(), bos);
      PrintPreorder(h->right(), bos);
    } else {
      // 1 and byte for leaf
      bos.PutBit(1);
      bos.PutChar(h->data());
    }
  }

  static void FillTable(HuffmanNode *h, std::map<char, std::string> &table, std::string buffer) {
    if (h == nullptr) {
      return;
    }
    if (h->IsLeaf() == false) {
      // add 0 going left, add 1 going right
      FillTable(h->left(), table, buffer + "0");
      FillTable(h->right(), table, buffer + "1");
    } else {
      // buffer now contains path to leaf
      table.insert({h->data(), buffer});
    }
  }

  static HuffmanNode* ReadPreorder(BinaryInputStream &bis) {
    // remakes tree by going through binary stream
    if (bis.GetBit() == 0) 
      return new HuffmanNode(0, 0, ReadPreorder(bis), ReadPreorder(bis));
    else 
      return new HuffmanNode(bis.GetChar(), 0);
  }

};

// To be completed below
void Huffman::Compress(std::ifstream &ifs, std::ofstream &ofs) {
  BinaryOutputStream bos(ofs);
  PQueue<HuffmanNode> pq;
  std::map<char, int> initial;

  char temp;

  // total number of characters
  int count = 0;

  // make map of char and frequency
  while (ifs.get(temp)) {
    if (initial.find(temp) == initial.end()) {
      initial.insert({temp, 1});
    } else {
      initial[temp]++;
    }
    count++;
  }

  // push vector contents to pqueue of HuffmanNodes
  for (std::map<char, int>::iterator it = initial.begin(); it != initial.end(); it++) {
    // this works properly
    pq.Push(HuffmanNode(it->first, it->second));
  }

  // build tree
  // result is single HuffmanNode that is root of tree
  while (pq.Size() > 1) {
    HuffmanNode *temp1 = new HuffmanNode(pq.Top());
    pq.Pop();
    HuffmanNode *temp2 = new HuffmanNode(pq.Top());
    pq.Pop();
    pq.Push(HuffmanNode(0, temp1->freq() + temp2->freq(), temp1, temp2));
  }

  // outputting the huffman tree with pre order traversal
  HuffmanNode *temp3 = new HuffmanNode(pq.Top());
  PrintPreorder(temp3, bos);
  

  // make map of char and path
  std::map<char, std::string> table;
  FillTable(temp3, table, "");
  // exit(0);

  // output number of characters in binary
  bos.PutInt(count);

  // output the input file using the encoded binary
  ifs.clear();
  ifs.seekg(0);
  while (ifs.get(temp)) {
    // output bits for the path
    std::string path = table[temp];
    for (unsigned int i = 0; i < path.length(); i++) {
      if (path[i] == '0') {
        bos.PutBit(0);
      } else if (path[i] == '1') {
        bos.PutBit(1);
      }
    }
  }
}

void Huffman::Decompress(std::ifstream &ifs, std::ofstream &ofs) {
  // read ifstream
  BinaryInputStream bis(ifs);

  // rebuild tree
  HuffmanNode *root = ReadPreorder(bis);
  
  // amount of characters
  int chars = bis.GetInt();

  HuffmanNode *h = root;

  // for every character
  for (int i = 0; i < chars; i++) {
    // find leaf
    while (h->IsLeaf() == false) {
      if (bis.GetBit() == 0) {
        h = h->left();
      } else {
        h = h->right();
      }
    }
    // found leaf
    //print char
    ofs << h->data();
    // go back to top of tree
    h = root;
  }
}



#endif  // HUFFMAN_H_
