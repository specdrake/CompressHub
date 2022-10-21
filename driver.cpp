#include <iostream>
#include <unordered_map>
#include <string>
#include "huffman.cpp"
using namespace std;
int main(void)
{
    // Node* root;
    // unordered_map<char,string> table;
    compress("uncompressed.txt", "tree.bin");
    decompress("binary.bin", "tree.bin", "decompressed.txt");
}
