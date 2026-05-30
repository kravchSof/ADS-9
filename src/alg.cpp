// Copyright 2022 NNTU-CS
#include "../include/tree.h"
#include <algorithm>
#include <memory>
#include <vector>

PMTree::PMTree(const std::vector<char>& input) : elements(input) {
    if (input.empty()) {
        root = nullptr;
        return;
    }

    root = std::make_shared<TreeNode>('\0');
    buildTree(root, input);
}

void PMTree::buildTree(std::shared_ptr<TreeNode> node,
                       std::vector<char> remaining) {
    if (remaining.empty()) {
        return;
    }

    std::sort(remaining.begin(), remaining.end());

    for (char c : remaining) {
        auto child = std::make_shared<TreeNode>(c);
        node->children.push_back(child);

        std::vector<char> newRemaining;
        for (char val : remaining) {
            if (val != c) {
                newRemaining.push_back(val);
            }
        }

        buildTree(child, newRemaining);
    }
}

void PMTree::collectPerms(std::shared_ptr<TreeNode> node,
                         std::vector<char>& current,
                         std::vector<std::vector<char>>& result) {
    if (!node) return;

    if (node->value != '\0') {
        current.push_back(node->value);
    }

    if (node->children.empty() && node->value != '\0') {
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

std::vector<std::vector<char>> PMTree::getAllPerms() {
    std::vector<std::vector<char>> result;
    std::vector<char> current;
    if (root) {
        collectPerms(root, current, result);
    }
    return result;
}

std::vector<std::vector<char>> getAllPerms(PMTree& tree) {
    return tree.getAllPerms();
}

bool PMTree::findPermByNum(std::shared_ptr<TreeNode> node,
                          std::vector<char>& current,
                          int& counter, int targetNum,
                          std::vector<char>& result) {
    if (!node) return false;

    if (node->value != '\0') {
        current.push_back(node->value);
    }

    if (node->children.empty() && node->value != '\0') {
        counter++;
        if (counter == targetNum) {
            result = current;
            if (node->value != '\0') current.pop_back();
            return true;
        }
    } else {
        for (auto& child : node->children) {
            if (findPermByNum(child, current, counter,
                            targetNum, result)) {
                if (node->value != '\0') current.pop_back();
                return true;
            }
        }
    }

    if (node->value != '\0') {
        current.pop_back();
    }
    return false;
}

bool PMTree::navigateToPerm(std::shared_ptr<TreeNode> node,
                           std::vector<char>& current,
                           int targetNum, std::vector<char>& result,
                           int& index) {
    if (!node) return false;

    if (node->value == '\0') {
        if (!node->children.empty()) {
            return navigateToPerm(node->children[0], current,
                                targetNum, result, index);
        }
        return false;
    }

    current.push_back(node->value);

    if (node->children.empty()) {
        index++;
        if (index == targetNum) {
            result = current;
            current.pop_back();
            return true;
        }
        current.pop_back();
        return false;
    }

    int permsInSubtree = 1;
    for (size_t i = node->children.size(); i > 1; i--) {
        permsInSubtree *= static_cast<int>(i);
    }

    int subtreeIndex = 0;
    int cumulative = 0;

    for (size_t i = 0; i < node->children.size(); i++) {
        if (targetNum <= cumulative + permsInSubtree) {
            subtreeIndex = static_cast<int>(i);
            break;
        }
        cumulative += permsInSubtree;
    }

    if (subtreeIndex < static_cast<int>(node->children.size())) {
        if (navigateToPerm(node->children[subtreeIndex], current,
                          targetNum - cumulative, result, index)) {
            return true;
        }
    }

    current.pop_back();
    return false;
}

std::vector<char> getPerm1(PMTree& tree, int num) {
    if (num <= 0) return {};

    std::vector<char> result;
    std::vector<char> current;
    int counter = 0;

    if (tree.getRoot() && !tree.getRoot()->children.empty()) {
        // Исправленный вызов метода
        if (tree.findPermByNum(tree.getRoot()->children[0], current,
                              counter, num, result)) {
            return result;
        }
    }

    return {};
}

std::vector<char> getPerm2(PMTree& tree, int num) {
    if (num <= 0) return {};

    int totalPerms = 1;
    for (size_t i = 2; i <= tree.elements.size(); i++) {
        totalPerms *= static_cast<int>(i);
    }

    if (num > totalPerms) return {};

    std::vector<char> result;
    std::vector<char> current;
    int index = 0;

    if (tree.getRoot() && !tree.getRoot()->children.empty()) {
        if (tree.navigateToPerm(tree.getRoot(), current, num,
                               result, index)) {
            return result;
        }
    }

    return {};
}
