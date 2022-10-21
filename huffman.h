#ifndef HUFFMAN_H
#define HUFFMAN_H

#include <iostream>
#include <string>
#include <queue>
#include <unordered_map>

// A Tree node
struct Node
{  
  char ch;
  int freq;
  Node *left, *right;
};

struct comp
{
  bool operator()(Node* l, Node* r)
  {
    // highest priority item has lowest frequency
    return l->freq > r->freq;
  }
};
Node* createNode(char ch, int freq, Node* left, Node* right);
void encode(Node* root, std::string str, std::unordered_map<char, std::string> &huffmanCode);
void decode(Node* root, int &index, std::string str, std::string& targetstr);
void readHuffmanTable(std::unordered_map<char, std::string>& setTable, std::string tfname);//unordered_map<char, string>& setTable);
void writeHuffmanTable(std::unordered_map<char,std::string>& table, std::string tfname);
void buildTreeFromTable(char c, std::string code, Node*& root);
std::string buildHuffmanTree(std::string text, Node*& setRoot);
std::string decodeString(Node* root, std::string encoded);
void hdecompress(std::string ifname, std::string tfname, std::string ofname, bool single);
void hcompress(std::string ifname, std::string ofname, std::string tfname, bool single);
long long filesize(const char* filename);
#endif // HUFFMAN_H

