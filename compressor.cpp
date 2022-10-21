#include "huffman.cpp"
#include <iostream>
#include <bitset>
#include <vector>
#include <string>
#include <fstream>
int main(void)
{
  std::ifstream infile;
  infile.open("uncompressed.txt");
  string text;
  std::getline(infile, text);
  /* const std::string text = "My name is Anurag Hooda."; */
  const std::string encoded = buildHuffmanTree(text);
  std::cout << std::endl;
  std::cout << "Original : " << text << std::endl;
  std::cout << "Encoded : " << encoded << std::endl;
  std::cout << "------------------------------\n";

  /* std::bitset<256> bs(encoded); */
  std::vector<std::bitset<8>> vbs;
  short bitc = 0;
  std::bitset<8> temp ("00000000");
  for(char c : encoded)
  {
    if(c == '0' || c == '1')
    {
      if(c=='0')
        temp.set(bitc, 0);
      else
        temp.set(bitc, 1);
      bitc++;
    }
    if(bitc == 8)
    {
      bitc = 0;
      vbs.push_back(temp);
      temp.set(0);
    }
  }
  std::cout << vbs.size() << std::endl;
  for (auto d : vbs)
    std::cout << d << std::endl;
  /* std::cout << bs << std::endl; */

  std::ofstream output("binary.bin", std::ios::binary);
  for (auto d : vbs){
    unsigned long n = d.to_ulong();
    unsigned char cc = static_cast<unsigned char>(n);
    output.write(reinterpret_cast<const char*>(&cc), sizeof(cc));
  }
  return 0;
}

