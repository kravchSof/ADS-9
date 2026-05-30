// Copyright 2022 NNTU-CS
#ifndef INCLUDE_TREE_H_
#define INCLUDE_TREE_H_

#include <vector>
#include <memory>
#include <algorithm>

// Узел дерева перестановок
struct TreeNode {
    char value;
    std::vector<std::shared_ptr<TreeNode>> children;

    explicit TreeNode(char val) : value(val) {}
};

class PMTree {
 private:
    std::shared_ptr<TreeNode> root;
    std::vector<char> elements;

    void buildTree(std::shared_ptr<TreeNode> node,
                   std::vector<char> remaining);

    void collectPerms(std::shared_ptr<TreeNode> node,
                     std::vector<char>& current,
                     std::vector<std::vector<char>>& result);

    bool findPermByNum(std::shared_ptr<TreeNode> node,
                      std::vector<char>& current,
                      int& counter, int targetNum,
                      std::vector<char>& result);

    bool navigateToPerm(std::shared_ptr<TreeNode> node,
                       std::vector<char>& current,
                       int targetNum, std::vector<char>& result,
                       int& index);

 public:
    explicit PMTree(const std::vector<char>& input);

    std::vector<std::vector<char>> getAllPerms();

    friend std::vector<std::vector<char>> getAllPerms(PMTree& tree);
    friend std::vector<char> getPerm1(PMTree& tree, int num);
    friend std::vector<char> getPerm2(PMTree& tree, int num);

    std::shared_ptr<TreeNode> getRoot() const { return root; }
};

std::vector<std::vector<char>> getAllPerms(PMTree& tree);
std::vector<char> getPerm1(PMTree& tree, int num);
std::vector<char> getPerm2(PMTree& tree, int num);

#endif  // INCLUDE_TREE_H_
