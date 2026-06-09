// Copyright 2022 NNTU-CS
#include "../include/tree.h"

#include <chrono>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <random>
#include <vector>

// Функция для измерения времени выполнения
template<typename Func>
double measureTime(Func func) {
    auto start = std::chrono::high_resolution_clock::now();
    func();
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> duration = end - start;
    return duration.count();
}

// Генерация случайного номера перестановки
int getRandomPermNumber(int maxPerms) {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(1, maxPerms);
    return dis(gen);
}

// Эксперимент с замерами времени
void runExperiment() {
    std::vector<int> sizes = {3, 4, 5, 6, 7, 8};
    std::vector<double> timesGetAllPerms;
    std::vector<double> timesGetPerm1;
    std::vector<double> timesGetPerm2;

    std::cout << "Запуск эксперимента по измерению времени..."
              << std::endl;
    std::cout << "----------------------------------------"
              << std::endl;

    for (int n : sizes) {
        std::vector<char> symbols;
        for (int i = 0; i < n; ++i) {
            symbols.push_back('0' + (i % 10));
        }

        std::cout << "Размер n = " << n
                  << ", количество перестановок: "
                  << factorial(n) << std::endl;

        auto treeCreateStart = std::chrono::high_resolution_clock::now();
        PMTree tree(symbols);
        auto treeCreateEnd = std::chrono::high_resolution_clock::now();
        double treeCreateTime =
            std::chrono::duration<double, std::milli>(
                treeCreateEnd - treeCreateStart).count();
        std::cout << "  Время построения дерева: "
                  << treeCreateTime << " мс" << std::endl;

        double getAllPermsTime = measureTime([&]() {
            getAllPerms(tree);
        });
        timesGetAllPerms.push_back(getAllPermsTime);
        std::cout << "  getAllPerms: " << getAllPermsTime << " мс"
                  << std::endl;

        int permNum = getRandomPermNumber(factorial(n));
        std::cout << "  Случайный номер перестановки: "
                  << permNum << std::endl;

        double getPerm1Time = measureTime([&]() {
            getPerm1(tree, permNum);
        });
        timesGetPerm1.push_back(getPerm1Time);
        std::cout << "  getPerm1 (обход): " << getPerm1Time << " мс"
                  << std::endl;

        double getPerm2Time = measureTime([&]() {
            getPerm2(tree, permNum);
        });
        timesGetPerm2.push_back(getPerm2Time);
        std::cout << "  getPerm2 (навигация): " << getPerm2Time << " мс"
                  << std::endl;
        std::cout << "----------------------------------------"
                  << std::endl;
    }

    std::ofstream outfile("result/times.csv");
    if (outfile.is_open()) {
        outfile << "n,getAllPerms,getPerm1,getPerm2" << std::endl;
        for (size_t i = 0; i < sizes.size(); ++i) {
            outfile << sizes[i] << ","
                    << timesGetAllPerms[i] << ","
                    << timesGetPerm1[i] << ","
                    << timesGetPerm2[i] << std::endl;
        }
        outfile.close();
        std::cout << "\nРезультаты сохранены в result/times.csv"
                  << std::endl;
    }

    std::ofstream plotScript("result/plot.py");
    if (plotScript.is_open()) {
        plotScript << "import matplotlib.pyplot as plt\n"
                   << "import numpy as np\n"
                   << "import pandas as pd\n\n"
                   << "data = pd.read_csv('times.csv')\n\n"
                   << "plt.figure(figsize=(12, 7))\n"
                   << "plt.semilogy(data['n'], data['getAllPerms'], "
                   << "'o-', linewidth=2, markersize=8, "
                   << "label='getAllPerms')\n"
                   << "plt.semilogy(data['n'], data['getPerm1'], "
                   << "'s-', linewidth=2, markersize=8, "
                   << "label='getPerm1 (обход)')\n"
                   << "plt.semilogy(data['n'], data['getPerm2'], "
                   << "'^-', linewidth=2, markersize=8, "
                   << "label='getPerm2 (навигация)')\n\n"
                   << "plt.xlabel('Размер алфавита (n)', "
                   << "fontsize=12)\n"
                   << "plt.ylabel('Время выполнения (мс)', "
                   << "fontsize=12)\n"
                   << "plt.title('Зависимость времени выполнения "
                   << "от размера алфавита', fontsize=14)\n"
                   << "plt.legend(fontsize=10)\n"
                   << "plt.grid(True, alpha=0.3)\n"
                   << "plt.savefig('plot.png', dpi=300, "
                   << "bbox_inches='tight')\n"
                   << "plt.show()\n";
        plotScript.close();
        std::cout << "Скрипт для построения графика создан: "
                  << "result/plot.py" << std::endl;
    }
}

// Демонстрация работы функций
void demonstrate() {
    std::cout << "\n=== Демонстрация работы функций ==="
              << std::endl;

    std::vector<char> symbols1 = {'1', '2', '3'};
    PMTree tree1(symbols1);

    std::cout << "\nВсе перестановки для {1,2,3}:" << std::endl;
    auto allPerms = getAllPerms(tree1);
    for (size_t i = 0; i < allPerms.size(); ++i) {
        std::cout << "  " << (i+1) << ": ";
        for (char c : allPerms[i]) {
            std::cout << c;
        }
        std::cout << std::endl;
    }

    std::cout << "\nПолучение перестановок по номеру:" << std::endl;
    for (int num = 1; num <= 6; ++num) {
        auto perm1 = getPerm1(tree1, num);
        auto perm2 = getPerm2(tree1, num);
        std::cout << "  Номер " << num << ": ";
        for (char c : perm1) std::cout << c;
        std::cout << " (getPerm1) / ";
        for (char c : perm2) std::cout << c;
        std::cout << " (getPerm2)" << std::endl;
    }

    auto invalidPerm = getPerm2(tree1, 10);
    std::cout << "\nЗапрос несуществующей перестановки (10): ";
    if (invalidPerm.empty()) {
        std::cout << "пустой вектор (корректно)" << std::endl;
    }

    std::vector<char> symbols2 = {'1', '3', '5', '7'};
    PMTree tree2(symbols2);

    std::cout << "\nПервые 5 перестановок для {1,3,5,7}:"
              << std::endl;
    for (int num = 1; num <= 5; ++num) {
        auto perm = getPerm2(tree2, num);
        std::cout << "  " << num << ": ";
        for (char c : perm) std::cout << c;
        std::cout << std::endl;
    }
}

int main() {
    std::cout << "=== Генерация перестановок с помощью "
              << "дерева вариантов ===" << std::endl;

    demonstrate();

    std::cout << "\n\n=== ВЫЧИСЛИТЕЛЬНЫЙ ЭКСПЕРИМЕНТ ==="
              << std::endl;
    runExperiment();

    return 0;
}
