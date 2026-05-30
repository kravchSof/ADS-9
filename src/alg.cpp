// Copyright 2022 NNTU-CS
#include "../include/tree.h"
#include <cmath>

// Конструктор: строит дерево вариантов
PMTree::PMTree(const std::vector<char>& input) : elements(input) {
    if (input.empty()) {
        root = nullptr;
        return;
    }
    
    root = std::make_shared<TreeNode>('\0'); // Корневой узел без значения
    buildTree(root, input);
}

// Построение дерева рекурсивно
void PMTree::buildTree(std::shared_ptr<TreeNode> node, std::vector<char> remaining) {
    if (remaining.empty()) {
        return;
    }
    
    // Сортируем оставшиеся элементы для упорядоченного построения
    std::sort(remaining.begin(), remaining.end());
    
    // Для каждого элемента создаем дочерний узел
    for (char c : remaining) {
        auto child = std::make_shared<TreeNode>(c);
        node->children.push_back(child);
        
        // Создаем вектор оставшихся элементов (без текущего)
        std::vector<char> newRemaining;
        for (char val : remaining) {
            if (val != c) {
                newRemaining.push_back(val);
            }
        }
        
        // Рекурсивно строим поддерево
        buildTree(child, newRemaining);
    }
}

// Сбор всех перестановок
void PMTree::collectPerms(std::shared_ptr<TreeNode> node, std::vector<char>& current,
                         std::vector<std::vector<char>>& result) {
    if (!node) return;
    
    // Добавляем значение текущего узла (кроме корня)
    if (node->value != '\0') {
        current.push_back(node->value);
    }
    
    // Если достигли листа (нет детей), сохраняем перестановку
    if (node->children.empty() && node->value != '\0') {
        result.push_back(current);
    } else {
        // Рекурсивно обходим всех детей
        for (auto& child : node->children) {
            collectPerms(child, current, result);
        }
    }
    
    // Удаляем текущее значение при возврате
    if (node->value != '\0') {
        current.pop_back();
    }
}

// Получение всех перестановок
std::vector<std::vector<char>> PMTree::getAllPerms() {
    std::vector<std::vector<char>> result;
    std::vector<char> current;
    if (root) {
        collectPerms(root, current, result);
    }
    return result;
}

// Внешняя функция getAllPerms
std::vector<std::vector<char>> getAllPerms(PMTree& tree) {
    return tree.getAllPerms();
}

// Метод 1: предварительный обход (для getPerm1)
bool PMTree::findPermByNum(std::shared_ptr<TreeNode> node, std::vector<char>& current,
                          int& counter, int targetNum, std::vector<char>& result) {
    if (!node) return false;
    
    if (node->value != '\0') {
        current.push_back(node->value);
    }
    
    if (node->children.empty() && node->value != '\0') {
        counter++;
        if (counter == targetNum) {
            result = current;
            current.pop_back();
            return true;
        }
    } else {
        for (auto& child : node->children) {
            if (findPermByNum(child, current, counter, targetNum, result)) {
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

// Метод 2: прямая навигация (для getPerm2)
bool PMTree::navigateToPerm(std::shared_ptr<TreeNode> node, std::vector<char>& current,
                           int targetNum, std::vector<char>& result, int& index) {
    if (!node) return false;
    
    // Для корневого узла начинаем навигацию по детям
    if (node->value == '\0') {
        // Если дети есть, переходим к ним
        if (!node->children.empty()) {
            return navigateToPerm(node->children[0], current, targetNum, result, index);
        }
        return false;
    }
    
    // Добавляем текущий узел в путь
    current.push_back(node->value);
    
    // Если достигли листа (перестановка найдена)
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
    
    // Вычисляем количество перестановок в каждом поддереве
    int permsInSubtree = 1;
    for (int i = node->children.size(); i > 1; i--) {
        permsInSubtree *= i;
    }
    
    // Определяем, в каком поддереве находится нужная перестановка
    int subtreeIndex = 0;
    int cumulative = 0;
    
    for (size_t i = 0; i < node->children.size(); i++) {
        if (targetNum <= cumulative + permsInSubtree) {
            subtreeIndex = i;
            break;
        }
        cumulative += permsInSubtree;
    }
    
    // Рекурсивно ищем в выбранном поддереве
    if (subtreeIndex < node->children.size()) {
        if (navigateToPerm(node->children[subtreeIndex], current, 
                          targetNum - cumulative, result, index)) {
            return true;
        }
    }
    
    current.pop_back();
    return false;
}

// getPerm1: получение перестановки с помощью предварительного обхода
std::vector<char> getPerm1(PMTree& tree, int num) {
    if (num <= 0) return {};
    
    std::vector<char> result;
    std::vector<char> current;
    int counter = 0;
    
    if (tree.getRoot() && !tree.getRoot()->children.empty()) {
        // Начинаем с первого ребенка корня
        if (tree.getRoot()->findPermByNum(tree.getRoot()->children[0], current, 
                                         counter, num, result)) {
            return result;
        }
    }
    
    return {};
}

// getPerm2: получение перестановки с помощью навигации по дереву
std::vector<char> getPerm2(PMTree& tree, int num) {
    if (num <= 0) return {};
    
    // Проверяем общее количество перестановок
    int totalPerms = 1;
    for (int i = 2; i <= tree.elements.size(); i++) {
        totalPerms *= i;
    }
    
    if (num > totalPerms) return {};
    
    std::vector<char> result;
    std::vector<char> current;
    int index = 0;
    
    if (tree.getRoot() && !tree.getRoot()->children.empty()) {
        if (tree.navigateToPerm(tree.getRoot(), current, num, result, index)) {
            return result;
        }
    }
    
    return {};
}
