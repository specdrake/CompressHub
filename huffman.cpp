#include "huffman.h"
#include <bitset>
#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <unordered_map>
using namespace std;
Node* createNode(char ch, int freq, Node* left, Node* right)
{
  Node* node = new Node();
  node->ch = ch;
  node->freq = freq;
  node->left = left;
  node->right = right;
  return node;
}

void encode(Node* root, string str,
      unordered_map<char, string> &huffmanCode)
{
  if (root == nullptr)
    return;

  // found a leaf node
  if (!root->left && !root->right) {
    huffmanCode[root->ch] = str;
  }

  encode(root->left, str + "0", huffmanCode);
  encode(root->right, str + "1", huffmanCode);
}

char decode(Node* root, int &index, string str)
{
    if (root == nullptr) {
        return '\0';
    }

    // found a leaf node
    if (!root->left && !root->right)
    {
        // cout << root->ch;
        // string temp = to_string(root->ch);
        // targetstr.append(temp);
        return root->ch;
    }

    index++;

    if (str[index] =='0')
        return decode(root->left, index, str);
    else
        return decode(root->right, index, str);
}
//char decodeV(Node* root, int &index, vector<char> str)
//{
//    if (root == nullptr) {
//        return '\0';
//    }

//    // found a leaf node
//    if (!root->left && !root->right)
//    {
//        // cout << root->ch;
//        // string temp = to_string(root->ch);
//        // targetstr.append(temp);
//        return root->ch;
//    }

//    index++;

//    if (str[index] =='0')
//        return decodeV(root->left, index, str);
//    else
//        return decodeV(root->right, index, str);
//}

void writeHuffmanTable(unordered_map<char,string>& table, string tfname)
{
  // cout << "--------Writing Huffman Code Table-------" << endl;
  uint16_t size = 0;
  int len;
  vector<unsigned char>first;
  vector<unsigned char>second;
  vector<vector<bitset<8>>>third;
  for(auto entry : table)
  {
      size+=2;
      len = entry.second.length(); 
      size+=len/8 + 1; 
      first.push_back((unsigned char)entry.first);
      second.push_back((unsigned char)len);
      // packing code into bytes
      auto encoded = entry.second;
      std::vector<std::bitset<8>> vbs;
      short bitc = 0;
      std::bitset<8> temp ("00000000");
      // cout << len;
      for(int i = 0; i < len; i++)
      {
        char c = encoded[i];
        if(c == '0' || c == '1')
        {
          if(c=='0')
            temp.set(bitc, 0);
          else
            temp.set(bitc, 1);
          bitc++;
        }
        if(bitc == 8 || i == len - 1)
        {
          // if(bitc != 8)
          // {
          //   for(int j = bitc; j < 8; j++)
          //   {
          //     // cout << "j: " << j << endl;
          //     temp.set(j, 0);
          //   }
          //   vbs.push_back(temp);
          //   temp = 8 - bitc;
          //   vbs.push_back(temp);
          // }
          // else { 
            vbs.push_back(temp);
            temp.set(0);
          // }
          bitc = 0;
      }
    }
    // for(auto d : vbs)
    //   cout << d << endl;
    third.push_back(vbs);
    // cout << entry.first << " : " << len << " : " << entry.second << endl;
  }
  // Now writing to a file
  fstream outfile;
  outfile.open(tfname, fstream::out | fstream::in | fstream::binary | fstream::trunc);
  // Writing first 2 bytest as size of table
  outfile.write(reinterpret_cast<const char*>(&size), 2);
  // cout << size << endl;
  uint16_t rsize;
  outfile.seekg(0);
  outfile.read(reinterpret_cast<char*>(&rsize),2);
  // cout << "--" << rsize << "--\n";
  // Writing the table itself
  // cout << first.size() << endl;
  // cout << second.size() << endl;
  // cout << third.size() << endl;
  int nrows = first.size();
  for(int j = 0; j < nrows; j++)
  {      
    outfile.write(reinterpret_cast<const char *>(&first[j]), 1);
    outfile.write(reinterpret_cast<const char *>(&second[j]), 1);
    int nbr = (int) second[j] / 8 + 1;
    // cout << "XXX : " << (int) second[j] << endl;
    for(int k = 0; k < nbr; k++)
    {
      outfile.write(reinterpret_cast<const char *>(&third[j][k]), 1);
      // cout << " YYY : " << third[j][k] << endl;
    }
    // outfile.write(reinterpret_cast<const char *>(&third[j]), 1);
  }
  outfile.close(); 
}

void readHuffmanTable(unordered_map<char, string>& setTable, string tfname)//unordered_map<char, string>& setTable)
{
  // cout << "----Reading Huffman Table-----\n";
  fstream infile;
  infile.open(tfname, fstream::binary | fstream::in);
  // reading first 2 bytes for size of the table
  uint16_t size;
  infile.read(reinterpret_cast<char *>(&size), 2);
  //reading the table now
  vector<unsigned char>first;
  vector<int>second;
  // vector<vector<bitset<8>>>third;
  unsigned char temp;
  unsigned char temp2;
  int nbr = 0;
  bitset<8> tempset;
  vector<string> vbs;
  string tempstr;
  bool t;
  int nrows=0;
  int bytesread = 0;
  while (bytesread < size)
  {      
    infile.read(reinterpret_cast<char *>(&temp), 1);
    bytesread++;
    first.push_back(temp);
    // cout << "///" << temp << endl;
    infile.read(reinterpret_cast<char *>(&temp2), 1);
    bytesread++;
    second.push_back((int)temp2);
    nbr = (int)temp2 / 8 + 1; 
    // cout << temp2 << "--------" << nbr << endl;
    for(int i = 0 ; i < nbr && bytesread < size; i++)
    {
      infile.read(reinterpret_cast<char *>(&tempset),1);
      bytesread++;
      // cout << "----->>>>" << tempset << endl;
      for(int j = 0; j < 4;j++)
      {
        t = tempset[j];
        tempset[j] = tempset[7-j];
        tempset[7-j] = t; 
      }
      tempstr.append(tempset.to_string());
    }
    // cout << "-->>>> " << (int) temp2 << "\n-->>>>" << tempstr << "\n";
    tempstr = tempstr.substr(0,temp2);
    vbs.push_back(tempstr);
    tempstr = "";
    nrows++;
  }

  // now creating unordered_map and setting it to the input param
  unordered_map<char, string> tablemap; 
  for(int i = 0; i < nrows; i++)
  {
    if (tablemap.find(first[i]) == tablemap.end()) 
    {
      tablemap[first[i]] = vbs[i];
      // cout << first[i] << " : " << second[i] << " : " << vbs[i] << endl;
    }
  } 
  setTable = tablemap;
  infile.close();
}
void buildTreeFromTable(char c, string code, Node*& root)
{
  // Create a leave on the given huffman tree using the given code    
  if(root==nullptr)
  {
    Node* temp = new Node;
    temp->ch = '\0';
    temp->freq = 0;
    temp->left = nullptr;
    temp->right = nullptr;
    root = temp;
  }
  if(code.length() == 1)
  {
    if(code[0]=='0')
    {
      if(root->left == nullptr)
        {
          Node* temp = new Node;
          temp->ch = c;
          temp->freq = 1;
          temp->left = nullptr;
          temp->right = nullptr;
          root->left = temp;
        }
      else {
          root->left->ch = c;
          root->left->freq++;
      }
    }
    else if(code[0] =='1') {
      if(root->right == nullptr)
        {
          Node* temp = new Node;
          temp->ch = c;
          temp->freq = 1;
          temp->left = nullptr;
          temp->right = nullptr;
          root->right = temp;
        }
      else {
          root->right->ch = c;
          root->right->freq++;
      }
    }
    return; 
  }
  else {
    if(code[0] == '0')
    {        
      if(root->left == nullptr)
      {
          Node* temp = new Node;
          temp->ch = '\0';
          temp->freq = 0;
          temp->left = nullptr;
          temp->right = nullptr;
          root->left = temp;
          buildTreeFromTable(c,code.substr(1),root->left);
      }
      else {
          buildTreeFromTable(c,code.substr(1),root->left);
      }
    }
    else if(code[0] == '1') {
      if(root->right == nullptr)
      {
          Node* temp = new Node;
          temp->ch = '\0';
          temp->freq = 0;
          temp->left = nullptr;
          temp->right = nullptr;
          root->right = temp;
          buildTreeFromTable(c,code.substr(1),root->right);
      }
      else {
          buildTreeFromTable(c,code.substr(1),root->right);
      }
    }
    else {
      return;
    }
  }
}

// Builds Huffman Tree and decode given input text
string buildHuffmanTree(string text, Node*& setRoot)
{
    // count frequency of appearance of each character
    // and store it in a map
    unordered_map<char, int> freq;
    for (char ch: text) {
        freq[ch]++;
    }

    // Create a priority queue to store live nodes of
    // Huffman tree;
    priority_queue<Node*, vector<Node*>, comp> pq;

    // Create a leaf node for each character and add it
    // to the priority queue.
    for (auto pair: freq) {
        pq.push(createNode(pair.first, pair.second, nullptr, nullptr));
    }

    // do till there is more than one node in the queue
    while (pq.size() != 1)
    {
        // Remove the two nodes of highest priority
        // (lowest frequency) from the queue
        Node *left = pq.top(); pq.pop();
        Node *right = pq.top();	pq.pop();

        // Create a new internal node with these two nodes
        // as children and with frequency equal to the sum
        // of the two nodes' frequencies. Add the new node
        // to the priority queue.
        int sum = left->freq + right->freq;
        pq.push(createNode('\0', sum, left, right));
    }

    // root stores pointer to root of Huffman Tree
    Node* root = pq.top();

    // traverse the Huffman Tree and store Huffman Codes
    // in a map. Also prints them
    unordered_map<char, string> huffmanCode;
    encode(root, "", huffmanCode);

    // cout << "Huffman Codes are :\n" << '\n';
    // for (auto pair: huffmanCode) {
    //     cout << pair.first << " " << pair.second << '\n';
    // }

    // cout << "\nOriginal string was :\n" << text << '\n';

    // print encoded string
    string str = "";
    for (char ch: text) {
        str += huffmanCode[ch];
    }

    // cout << "\nEncoded string is :\n" << str << '\n';
    cout << "File Compressed succesfully!" << endl;

   setRoot = root;
   return str;
}

string decodeString(Node* root, string encoded) 
{
    // traverse the Huffman Tree again and this time
    // decode the encoded string
    int index = -1;
    // cout << "\nDecoded string is: \n";
    string decoded = "";
    while (index < (int)encoded.size() - 2) {
      // cout << "loop" << endl;
      decoded += decode(root, index, encoded);
    }
    // cout << "Decoded : "  << decoded << endl;
    return decoded; 
}
//vector<char> decodeVec(Node* root, vector<char> encoded)
//{
//    // traverse the Huffman Tree again and this time
//    // decode the encoded string
//    int index = -1;
//    // cout << "\nDecoded string is: \n";
//    vector<char> decoded;
//    while (index < (int)encoded.size() - 2) {
//      // cout << "loop" << endl;
//      decoded.push_back(decodeV(root, index, encoded));
//    }
//    // cout << "Decoded : "  << decoded << endl;
//    return decoded;
//}

void hcompress(string ifname, string ofname, string tfname, bool single)
{
  fstream infile;
  std::fstream output(ofname, std::ios::binary|std::ios::in|std::ios::out|std::ios::trunc);
  infile.open(ifname);
  string text((istreambuf_iterator<char>(infile)), (istreambuf_iterator<char>()));
  // getline(infile, text);
  /* const std::string text = "My name is Anurag Hooda."; */
  Node* setRoot;
  const string encoded = buildHuffmanTree(text, setRoot);
  unordered_map<char,string> table;
  encode(setRoot, "", table);
  if(single)
  {
      writeHuffmanTable(table, ofname);
      uint16_t rsize;
      output.seekg(0);
      output.read(reinterpret_cast<char*>(&rsize),2);
      output.seekg(rsize+2);
  }
  else
    writeHuffmanTable(table, tfname);
  // cout << endl;
  /* std::cout << "Original : " << text << std::endl; */
  /* std::cout << "Encoded : " << encoded << std::endl; */
  /* std::cout << "------------------------------\n"; */

  /* std::bitset<256> bs(encoded); */
  std::vector<std::bitset<8>> vbs;
  short bitc = 0;
  std::bitset<8> temp ("00000000");
  int len = encoded.length();
  // cout << len;
  for(int i = 0; i < len; i++)
  {
    char c = encoded[i];
    if(c == '0' || c == '1')
    {
      if(c=='0')
        temp.set(bitc, 0);
      else
        temp.set(bitc, 1);
      bitc++;
    }
    if(bitc == 8 || i == len - 1)
    {
      if(bitc != 8)
      {
        for(int j = bitc; j < 8; j++)
        {
          // cout << "j: " << j << endl;
          temp.set(j, 0);
        }
        vbs.push_back(temp);
        temp = 8 - bitc;
        vbs.push_back(temp);
      }
      else { 
        vbs.push_back(temp);
        temp.set(0);
      }
      bitc = 0;
    }
  }
  std::cout << vbs.size() << " bytes written."<< std::endl;
  // for (auto d : vbs)
  //   std::cout << d << std::endl;
  /* std::cout << bs << std::endl; */

  for (auto d : vbs){
    unsigned long n = d.to_ulong();
    unsigned char cc = static_cast<unsigned char>(n);
    output.write(reinterpret_cast<const char*>(&cc), sizeof(cc));
  }
  infile.close();
  output.close();
}
void displayTree(Node* root)
{
  if(root == nullptr)
    return;
  displayTree(root->left);
  cout << root->ch << " : " << root->freq << endl;
  displayTree(root->right);
}

void hdecompress(string ifname, string tfname, string ofname, bool single)
{ 
  unordered_map<char, string> tablemap;
  Node* root = nullptr;
  uint16_t rsize = 0;
  ifstream infile;
  infile.open(ifname, ios::binary | ios::in);
  infile.seekg(0,ios::end);
  size_t fsize = infile.tellg();

  if(single)
  {
    readHuffmanTable(tablemap, ifname);
    infile.seekg(0);
    infile.read(reinterpret_cast<char*>(&rsize),2);
    infile.seekg(rsize+2);
    fsize -= rsize + 2;
  }
  else
  {
    readHuffmanTable(tablemap, tfname);
  }
  for(auto& entry : tablemap)
  {
    buildTreeFromTable(entry.first, entry.second, root);
  }
  cout << "Decompressing....\n";
  cout << "File size: " << fsize << " bytes" <<  endl;
  if(!single)
    infile.seekg(0,ios::beg);
  char byte;
  bitset<8>byteset;
  bool t;
  string encoded, text;
  int lastbyte; 
  ofstream outfile;
  outfile.open(ofname, ios::out | ios::trunc);
  for(auto i = 0; (i < fsize) && infile; i++)
  {
    cout << "Read " << i << " bytes / " << fsize << "bytes : " << (float)i/fsize * 100 << "%\n" ;
    infile.read(&byte, 1);
    byteset = byte;
    if(i % 1000 == 0 && fsize - i >= 16)
    {
        cout << "Writing Buffer" << endl;
        text = decodeString(root, encoded);
        outfile << text;
        encoded = "";
    }
    if(i == fsize - 1)
    {
      cout << "On last byte\n";
      lastbyte = (int)byteset.to_ulong(); 
      cout << lastbyte << endl;
      string lastbyteset;
      lastbyteset = encoded.substr(encoded.length()-8);
      encoded = encoded.substr(0, encoded.length()-8+1);
      lastbyteset = lastbyteset.substr(0,8-(lastbyte));
      encoded.append(lastbyteset);
    }
    else {
      for(auto j = 0; j < 4; j++)
      {
        t = byteset[j];
        byteset[j] = byteset[7-j];
        byteset[7-j] = t;
      }
      encoded.append(byteset.to_string());
      cout << byteset.to_string() << endl;

    }
  }
  infile.close();

  text = decodeString(root, encoded);
  outfile << text;
  outfile.close();

  cout << "Decompressed text written in : " << ofname << endl;
}

long long filesize(const char* filename)
{
    std::ifstream in(filename, std::ifstream::ate | std::ifstream::binary);
    auto pos = in.tellg();
    cout << pos;
    return pos;
}
