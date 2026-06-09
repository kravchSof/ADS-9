// Copyright 2022 NNTU-CS
#include "../include/tree.h"
#include <algorithm>
#include <functional>
#include <cmath>

PMTree::PMTree(const std::vector<char>& symbols) {
    size = symbols.size();
    if (symbols.empty()) {
        root = nullptr;
        return;
    }
    
    std::vector<char> sorted = symbols;
    std::sort(sorted.begin(), sorted.end());
    
    root = std::make_shared<Node>('\0');
    buildTree(root, sorted);
}

void PMTree::buildTree(std::shared_ptr<Node> node, std::vector<char> remaining) {
    if (remaining.empty()) return;
    
    for (char c : remaining) {
        auto child = std::make_shared<Node>(c);
        node->children.push_back(child);
        
        std::vector<char> newRemaining;
        for (char rc : remaining) {
            if (rc != c) {
                newRemaining.push_back(rc);
            }
        }
        
        buildTree(child, newRemaining);
    }
}

void PMTree::collectPerms(std::shared_ptr<Node> node, 
                          std::vector<char>& current,
                          std::vector<std::vector<char>>& result) const {
    if (!node) return;
    
    if (node->value != '\0') {
        current.push_back(node->value);
    }
    
    if (current.size() == static_cast<size_t>(size)) {
        result.push_back(current);
    } else {
        for (auto& child : node->children) {
            collectPerms(child, current, result);
        }
    }
    
    if (node->value != '\0') {
        current.pop_back();
    }
}

std::vector<std::vector<char>> getAllPerms(PMTree& tree) {
    std::vector<std::vector<char>> result;
    if (!tree.root || tree.size == 0) return result;
    
    std::vector<char> current;
    tree.collectPerms(tree.root, current, result);
    return result;
}

int PMTree::getSubtreePermCount(std::shared_ptr<Node> node, int depth) const {
    if (!node) return 0;
    if (depth == 0) return 1;
    
    int count = 0;
    for (auto& child : node->children) {
        count += getSubtreePermCount(child, depth - 1);
    }
    return count;
}

std::vector<char> getPerm1(PMTree& tree, int num) {
    if (num <= 0) return std::vector<char>();
    
    auto allPerms = getAllPerms(tree);
    if (num > static_cast<int>(allPerms.size())) {
        return std::vector<char>();
    }
    
    return allPerms[num - 1];
}

int factorial(int n) {
    int result = 1;
    for (int i = 2; i <= n; ++i) {
        result *= i;
    }
    return result;
}

std::vector<char> getPerm2(PMTree& tree, int num) {
    if (num <= 0 || !tree.root || tree.size == 0) {
        return std::vector<char>();
    }
    
    int totalPerms = factorial(tree.size);
    if (num > totalPerms) {
        return std::vector<char>();
    }
    
    std::vector<char> result;
    auto currentNode = tree.root;
    int currentNum = num;
    int remainingSize = tree.size;
    
    while (currentNode && result.size() < static_cast<size_t>(tree.size)) {
        if (currentNode->children.empty()) break;
        
        int permsPerChild = factorial(remainingSize - 1);
        
        int childIndex = (currentNum - 1) / permsPerChild;
        
        if (childIndex >= static_cast<int>(currentNode->children.size())) {
            return std::vector<char>();
        }
        
        currentNode = currentNode->children[childIndex];
        result.push_back(currentNode->value);
        
        currentNum = currentNum - childIndex * permsPerChild;
        remainingSize--;
    }
    
    return result;
}
