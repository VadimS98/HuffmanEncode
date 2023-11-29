#pragma once
#include <memory>
class HuffmanNode {
public:
    uint8_t value;
    int freq;
    HuffmanNode* left;
    HuffmanNode* right;

    HuffmanNode(uint8_t, int);
    HuffmanNode(HuffmanNode*, HuffmanNode*);
    HuffmanNode(uint8_t, int, HuffmanNode*, HuffmanNode*);
    ~HuffmanNode();
};