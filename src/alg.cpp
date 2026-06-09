// Copyright 2022 NNTU-CS

#include "tree.h"

#include <algorithm>
#include <vector>

Node::Node(char v) : value(v), subtree_size(0) {}

Node::~Node() {
    for (Node* child : children) delete child;
}

PMTree::PMTree(const std::vector<char>& symbols) {
    std::vector<char> sorted = symbols;
    std::sort(sorted.begin(), sorted.end());
    root = new Node('\0');
    root->children = buildChildren(sorted);
    root->subtree_size = 0;
    for (Node* child : root->children) {
        root->subtree_size += child->subtree_size;
    }
}

PMTree::~PMTree() {
    delete root;
}

Node* PMTree::getRoot() const {
    return root;
}

std::vector<Node*> PMTree::buildChildren(std::vector<char> available) {
    std::vector<Node*> result;
    if (available.empty()) return result;

    for (size_t i = 0; i < available.size(); ++i) {
        char c = available[i];
        Node* node = new Node(c);

        std::vector<char> remaining;
        remaining.reserve(available.size() - 1);
        for (size_t j = 0; j < available.size(); ++j) {
            if (j != i) remaining.push_back(available[j]);
        }

        node->children = buildChildren(remaining);

        if (node->children.empty()) {
            node->subtree_size = 1;
        } else {
            node->subtree_size = 0;
            for (Node* child : node->children) {
                node->subtree_size += child->subtree_size;
            }
        }
        result.push_back(node);
    }
    return result;
}

void PMTree::collectPerms(Node* node, std::vector<char>& current,
                          std::vector<std::vector<char>>& output) const {
    if (node->value != '\0') {
        current.push_back(node->value);
    }

    if (node->children.empty()) {
        output.push_back(current);
    } else {
        for (Node* child : node->children) {
            collectPerms(child, current, output);
        }
    }

    if (node->value != '\0') {
        current.pop_back();
    }
}

std::vector<std::vector<char>> PMTree::getAllPerms() const {
    std::vector<std::vector<char>> result;
    std::vector<char> current;
    collectPerms(root, current, result);
    return result;
}

std::vector<char> getPerm1(PMTree& tree, int num) {
    if (num <= 0) return {};
    auto all = tree.getAllPerms();
    if (static_cast<size_t>(num) > all.size()) return {};
    return all[num - 1];
}

std::vector<char> getPerm2(PMTree& tree, int num) {
    if (num <= 0) return {};
    Node* root = tree.getRoot();
    if (static_cast<size_t>(num) > root->subtree_size) return {};

    std::vector<char> result;
    Node* current = root;
    int remaining = num;

    while (!current->children.empty()) {
        for (Node* child : current->children) {
            if (static_cast<size_t>(remaining) <= child->subtree_size) {
                result.push_back(child->value);
                current = child;
                break;
            } else {
                remaining -= static_cast<int>(child->subtree_size);
            }
        }
    }
    return result;
}
