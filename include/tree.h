// Copyright 2022 NNTU-CS
#ifndef INCLUDE_TREE_H_
#define INCLUDE_TREE_H_

#include <vector>
#include <memory>

// Узел дерева
struct Node {
    char value;
    std::vector<std::shared_ptr<Node>> children;

    explicit Node(char val) : value(val) {}
};

// Класс дерева перестановок
class PMTree {
 private:
    std::shared_ptr<Node> root;
    int size;

    void buildTree(std::shared_ptr<Node> node, std::vector<char> remaining);
    void collectPerms(std::shared_ptr<Node> node,
                      std::vector<char>& current,
                      std::vector<std::vector<char>>& result) const;
    int getSubtreePermCount(std::shared_ptr<Node> node, int depth) const;

 public:
    explicit PMTree(const std::vector<char>& symbols);

    std::shared_ptr<Node> getRoot() const { return root; }
    int getSize() const { return size; }

    friend std::vector<std::vector<char>> getAllPerms(PMTree& tree);
    friend std::vector<char> getPerm1(PMTree& tree, int num);
    friend std::vector<char> getPerm2(PMTree& tree, int num);
};

// Функции для работы с деревом
std::vector<std::vector<char>> getAllPerms(PMTree& tree);
std::vector<char> getPerm1(PMTree& tree, int num);
std::vector<char> getPerm2(PMTree& tree, int num);

// Вспомогательная функция
int factorial(int n);

#endif  // INCLUDE_TREE_H_
