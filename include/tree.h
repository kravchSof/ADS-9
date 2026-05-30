// Copyright 2022 NNTU-CS
#ifndef INCLUDE_TREE_H_
#define INCLUDE_TREE_H_

#include <vector>
#include <memory>
#include <algorithm>

// Узел дерева перестановок
struct TreeNode {
    char value;  // Значение в узле
    std::vector<std::shared_ptr<TreeNode>> children;  // Дочерние узлы
    
    TreeNode(char val) : value(val) {}
};

class PMTree {
private:
    std::shared_ptr<TreeNode> root;
    std::vector<char> elements;
    
    // Рекурсивное построение дерева
    void buildTree(std::shared_ptr<TreeNode> node, std::vector<char> remaining);
    
    // Рекурсивный сбор всех перестановок
    void collectPerms(std::shared_ptr<TreeNode> node, std::vector<char>& current, 
                     std::vector<std::vector<char>>& result);
    
    // Рекурсивный поиск перестановки по номеру (метод обхода)
    bool findPermByNum(std::shared_ptr<TreeNode> node, std::vector<char>& current,
                      int& counter, int targetNum, std::vector<char>& result);
    
    // Рекурсивный поиск перестановки по номеру (метод навигации)
    bool navigateToPerm(std::shared_ptr<TreeNode> node, std::vector<char>& current,
                        int targetNum, std::vector<char>& result, int& index);
    
public:
    // Конструктор
    PMTree(const std::vector<char>& input);
    
    // Получение всех перестановок
    std::vector<std::vector<char>> getAllPerms();
    
    // Дружественные функции для доступа к private методам
    friend std::vector<std::vector<char>> getAllPerms(PMTree& tree);
    friend std::vector<char> getPerm1(PMTree& tree, int num);
    friend std::vector<char> getPerm2(PMTree& tree, int num);
    
    // Метод для получения корня дерева (для friend функций)
    std::shared_ptr<TreeNode> getRoot() const { return root; }
};

// Внешние функции
std::vector<std::vector<char>> getAllPerms(PMTree& tree);
std::vector<char> getPerm1(PMTree& tree, int num);
std::vector<char> getPerm2(PMTree& tree, int num);

#endif  // INCLUDE_TREE_H_
