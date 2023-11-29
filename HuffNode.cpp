#include "HuffNode.h"

HuffmanNode::HuffmanNode(uint8_t val, int fr) {
    value = val;
    freq = fr;
    left = nullptr;
    right = nullptr;
}

HuffmanNode::HuffmanNode(HuffmanNode* l, HuffmanNode* r) {
    value = 0;
    freq = 0;
    left = l;
    right = r;
}

HuffmanNode::HuffmanNode(uint8_t val, int fr, HuffmanNode* l, HuffmanNode* r) {
    value = val;
    freq = fr;
    left = l;
    right = r;
}

HuffmanNode::~HuffmanNode() {
    if (left != nullptr) {
        delete left;
    }
    if (right != nullptr) {
        delete right;
    }
}