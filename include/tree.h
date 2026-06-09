// Copyright 2022 NNTU-CS
#ifndef INCLUDE_TREE_H_
#define INCLUDE_TREE_H_

#include <vector>

struct Node {
    char value;
    std::vector<Node*> children;
    size_t subtree_size;

    explicit Node(char v);
    ~Node();
};

class PMTree {
 private:
    Node* root;

    std::vector<Node*> buildChildren(std::vector<char> available);
    void collectPerms(Node* node, std::vector<char>& current,
                      std::vector<std::vector<char>>& output) const;

 public:
    explicit PMTree(const std::vector<char>& symbols);
    ~PMTree();

    Node* getRoot() const;
    std::vector<std::vector<char>> getAllPerms() const;
};

std::vector<char> getPerm1(PMTree& tree, int num);
std::vector<char> getPerm2(PMTree& tree, int num);

#endif  // INCLUDE_TREE_H_
