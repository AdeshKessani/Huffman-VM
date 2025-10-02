/*Daud Ahmad Nisar (U37366522) & Adesh Kessani (U69434322)
This code implements Huffman coding for lossless data compression and decompression, 
including functions for building a Huffman tree, generating compressed binary output, 
serializing the tree, and reconstructing the tree for decompression.*/



#include "HuffmanTree.hpp"
#include <map>
#include <iostream>
#include <stack>

std::string HuffmanTree::compress(const std::string inputStr) {
    std::map<char, size_t> charMap;
    std::map<char, size_t>::iterator it;

    size_t count[256] = {0};

    //counting frequency of charaacters in input string
    for (int i = 0; inputStr[i] != '\0'; i++) {
        count[inputStr[i]]++;
    }
    for (int i = 0; i < 256; i++) {
        if (count[i] != 0) {
            charMap.insert(std::pair<char, size_t>((char)i, count[i]));
        }
    }

    //create heap priority queue and populate it
    HeapQueue<HuffmanNode*, HuffmanNode::Compare> PQ;
    for (it = charMap.begin(); it != charMap.end(); ++it) {
        HuffmanNode* node = new HuffmanNode(it->first, it->second);
        PQ.insert(node);
    }

    //huffman tree creation from PQ
    while (PQ.size() != 1) {
        HuffmanNode* node1 = PQ.min();
        PQ.removeMin();
        HuffmanNode* node2 = PQ.min();
        PQ.removeMin();
        size_t frequencyT = (node1->getFrequency() + node2->getFrequency());
        HuffmanNode* parentN1N2 = new HuffmanNode('\0', frequencyT, nullptr, node1, node2);
        node1->parent = parentN1N2;
        node2->parent = parentN1N2;
        PQ.insert(parentN1N2);
    }

    
    root = PQ.min();

    //encoding of each character in the string
    std::map<char, std::string> codeMap;
    std::string code = "";
    std::string output = "";

    //finding the Huffman codes for each character
    for (it = charMap.begin(); it != charMap.end(); ++it) {
        findCode(PQ.min(), codeMap, code);
    }

    //construct the compressed output
    for (auto i : inputStr) {
        output += codeMap[i];
    }
    return output;
}

//tree serialization
std::string HuffmanTree::serializeTree() const {
    if (root == nullptr) {
        return "";
    }

    std::string output = "";
    serialize(root, output);
    return output;
}

//tree decompression 
std::string HuffmanTree::decompress(const std::string inputCode, const std::string serializedTree) {
    std::string output = "";
    size_t len = serializedTree.length();
    std::stack<HuffmanNode*> treeStack;

    std::cout << "Decompressing... Start reconstructing tree from serialized string." << std::endl;

    //reconstruct the tree from serializedTree
    for (size_t i = 0; i < len; ++i) {
        char character = serializedTree[i];
        std::cout << "Processing character: " << character << std::endl;

        if (character == 'L') {
            if (i + 1 < len) {
                char leafValue = serializedTree[++i];  //increment to get the next character as the leaf value
                HuffmanNode* huffNode = new HuffmanNode(leafValue, 0);
                treeStack.push(huffNode);
                std::cout << "Created Leaf Node: " << leafValue << std::endl;
            }
        } else if (character == 'B') {
            if (treeStack.size() < 2) {
                std::cout << "Error: Insufficient nodes in stack, cannot process 'B'." << std::endl;
                return "";  //return empty if the tree is invalid
            }
            HuffmanNode* right = treeStack.top();
            treeStack.pop();
            HuffmanNode* left = treeStack.top();
            treeStack.pop();
            HuffmanNode* branch = new HuffmanNode('\0', 0, nullptr, left, right);
            treeStack.push(branch);
            right->parent = branch;
            left->parent = branch;
            std::cout << "Created Branch Node: Left (" << left->getCharacter() << "), Right (" << right->getCharacter() << ")" << std::endl;
        }
    }

    //copy the remaining stack to be the root of the newly constructed Huffman Tree
    if (treeStack.empty()) {
        std::cout << "Error: Tree reconstruction failed. Stack is empty." << std::endl;
        return "";  // prevent accessing invalid memory
    }

    HuffmanNode* root = treeStack.top();
    treeStack.pop();
    std::cout << "Reconstructed root node." << std::endl;

    //reconstructing the text from the code using the built Huffman tree
    HuffmanNode* curr = root;
    for (auto character : inputCode) {
        std::cout << "Processing code: " << character << std::endl;
        if (character == '0') {
            if (curr->left != nullptr) {
                curr = curr->left;
            } else {
                std::cout << "Error: Left child is null. Stopping." << std::endl;
                break;  //prevent null pointer access if left child is null
            }
        } else if (character == '1') {
            if (curr->right != nullptr) {
                curr = curr->right;
            } else {
                std::cout << "Error: Right child is null. Stopping." << std::endl;
                break;  //prevent null pointer access if right child is null
            }
        }

        //once we hit the leaf add the char to output, start over from the root
        if (curr->isLeaf()) {
            output += curr->getCharacter();
            std::cout << "Added character to output: " << curr->getCharacter() << std::endl;
            curr = root;  //reset to root for the next character
        }
    }

    std::cout << "Decompression complete. Output: " << output << std::endl;
    return output;
}
