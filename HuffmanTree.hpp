/*Daud Ahmad Nisar (U37366522) & Adesh Kessani (U69434322)
This code implements Huffman coding for lossless data compression and decompression, 
including functions for building a Huffman tree, generating compressed binary output, 
serializing the tree, and reconstructing the tree for decompression.*/

#ifndef HUFFMANTREE_HPP
#define HUFFMANTREE_HPP

#include "HuffmanBase.hpp"
#include "HeapQueue.hpp"
#include <map>

class HuffmanTree : public HuffmanTreeBase {
private:
    size_t n;
    HuffmanNode* root;

public:
    std::string compress(const std::string inputStr) override;
    std::string serializeTree() const override;
    std::string decompress(const std::string inputCode, const std::string serializedTree) override;
    
    //helper method to find the Huffman code for each character
    void findCode(const HuffmanNode* node, std::map<char, std::string>& codeMap, std::string code) {
        if (node == nullptr) return;
        if(node->isLeaf()) {
            codeMap[node->getCharacter()] = code;
        }
        findCode(node->left, codeMap, code + "0");
        findCode(node->right, codeMap, code + "1");
    }

    //post order traversal recursive function to serialize the huffman tree
    void serialize(const HuffmanNode* node, std::string& outString) const {
        if (node == nullptr) return;
        
        serialize(node->left, outString);
        serialize(node->right, outString);

        if (node->isLeaf()) {
            outString = outString + "L" + node->getCharacter();
        }
        if (node->isBranch()) {
            outString += "B";
        }
    }
};

#endif 
