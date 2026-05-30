// Copyright 2022 NNTU-CS
#include "../include/tree.h"
#include <iostream>
#include <chrono>
#include <random>
#include <fstream>
#include <vector>
#include <iomanip>
#include <algorithm>

using std::cout;
using std::endl;
using std::vector;
using std::ofstream;
using std::setw;
using std::min;
using std::random_device;
using std::mt19937;
using std::uniform_int_distribution;
using std::chrono::high_resolution_clock;
using std::chrono::duration_cast;
using std::chrono::microseconds;

vector<char> generateAlphabet(int n) {
    vector<char> alphabet;
    for (int i = 0; i < n; i++) {
        alphabet.push_back('a' + static_cast<char>(i));
    }
    return alphabet;
}

template<typename Func>
long long measureTime(Func func) {
    auto start = high_resolution_clock::now();
    func();
    auto end = high_resolution_clock::now();
    return duration_cast<microseconds>(end - start).count();
}

int main() {
    cout << "=== Пример использования дерева перестановок ===" << endl;

    vector<char> example1 = {'1', '2', '3'};
    PMTree tree1(example1);

    cout << "\nВсе перестановки для {1,2,3}:" << endl;
    vector<vector<char>> perms = getAllPerms(tree1);
    for (size_t i = 0; i < perms.size(); i++) {
        cout << i + 1 << ": ";
        for (char c : perms[i]) {
            cout << c;
        }
        cout << endl;
    }

    cout << "\n=== Проверка getPerm1 и getPerm2 ===" << endl;
    for (int num = 1; num <= 6; num++) {
        vector<char> perm1 = getPerm1(tree1, num);
        vector<char> perm2 = getPerm2(tree1, num);

        cout << "Перестановка #" << num << ": ";
        cout << "getPerm1: ";
        for (char c : perm1) cout << c;
        cout << ", getPerm2: ";
        for (char c : perm2) cout << c;
        cout << endl;
    }

    vector<char> example2 = {'1', '3', '5', '7'};
    PMTree tree2(example2);

    cout << "\nПервые 5 перестановок для {1,3,5,7}:" << endl;
    vector<vector<char>> perms2 = getAllPerms(tree2);
    for (int i = 0; i < min(5, static_cast<int>(perms2.size())); i++) {
        cout << i + 1 << ": ";
        for (char c : perms2[i]) {
            cout << c;
        }
        cout << endl;
    }

    cout << "\n=== ВЫЧИСЛИТЕЛЬНЫЙ ЭКСПЕРИМЕНТ ===" << endl;
    cout << "Измерение времени работы функций" << endl;
    cout << "при различных размерах алфавита (n)" << endl;
    cout << "-------------------------------------" << endl;
    cout << setw(6) << "n"
         << setw(15) << "getAll(μs)"
         << setw(15) << "get1(μs)"
         << setw(15) << "get2(μs)" << endl;
    cout << "-------------------------------------" << endl;

    vector<int> sizes;
    vector<double> timesAll, times1, times2;

    for (int n = 1; n <= 10; n++) {
        vector<char> alphabet = generateAlphabet(n);
        PMTree tree(alphabet);

        int totalPerms = 1;
        for (int i = 2; i <= n; i++) totalPerms *= i;
        random_device rd;
        mt19937 gen(rd());
        uniform_int_distribution<> dis(1, std::max(1, totalPerms));
        int randomNum = dis(gen);

        long long timeAll = measureTime([&]() {
            getAllPerms(tree);
        });

        long long time1 = measureTime([&]() {
            getPerm1(tree, randomNum);
        });

        long long time2 = measureTime([&]() {
            getPerm2(tree, randomNum);
        });

        sizes.push_back(n);
        timesAll.push_back(static_cast<double>(timeAll));
        times1.push_back(static_cast<double>(time1));
        times2.push_back(static_cast<double>(time2));

        cout << setw(6) << n
             << setw(15) << timeAll
             << setw(15) << time1
             << setw(15) << time2 << endl;
    }

    ofstream dataFile("result/data.txt");
    if (dataFile.is_open()) {
        dataFile << "# n getAllPerms(us) getPerm1(us) getPerm2(us)"
                 << endl;
        for (size_t i = 0; i < sizes.size(); i++) {
            dataFile << sizes[i] << " "
                     << timesAll[i] << " "
                     << times1[i] << " "
                     << times2[i] << endl;
        }
        dataFile.close();
        cout << "\nДанные сохранены в result/data.txt" << endl;
    }

    ofstream gnuplotScript("result/plot.gnu");
    if (gnuplotScript.is_open()) {
        gnuplotScript << "set terminal png size 1200,800 enhanced "
                      << "font 'Arial,12'" << endl;
        gnuplotScript << "set output 'plot.png'" << endl;
        gnuplotScript << "set title 'Зависимость времени работы "
                      << "функций от размера алфавита'" << endl;
        gnuplotScript << "set xlabel 'Размер алфавита (n)'" << endl;
        gnuplotScript << "set ylabel 'Время выполнения "
                      << "(микросекунды)'" << endl;
        gnuplotScript << "set logscale y" << endl;
        gnuplotScript << "set grid" << endl;
        gnuplotScript << "set key top left" << endl;
        gnuplotScript << "plot 'data.txt' using 1:2 with linespoints "
                      << "linewidth 2 title 'getAllPerms', \\"
                      << endl;
        gnuplotScript << "     'data.txt' using 1:3 with linespoints "
                      << "linewidth 2 title 'getPerm1 (обход)', \\"
                      << endl;
        gnuplotScript << "     'data.txt' using 1:4 with linespoints "
                      << "linewidth 2 title 'getPerm2 (навигация)'"
                      << endl;
        gnuplotScript.close();

        cout << "Gnuplot скрипт сохранен в result/plot.gnu" << endl;
        cout << "Для построения графика выполните команду:" << endl;
        cout << "cd result && gnuplot plot.gnu" << endl;
    }

    cout << "\n=== Тестирование граничных случаев ===" << endl;

    vector<char> emptyVec;
    PMTree emptyTree(emptyVec);
    vector<char> emptyPerm = getPerm1(emptyTree, 1);
    cout << "Пустое дерево: " << (emptyPerm.empty() ? "OK" : "FAIL")
         << endl;

    vector<char> testVec = {'A', 'B'};
    PMTree testTree(testVec);
    vector<char> invalidPerm = getPerm1(testTree, 10);
    cout << "Несуществующая перестановка: "
         << (invalidPerm.empty() ? "OK" : "FAIL") << endl;

    invalidPerm = getPerm2(testTree, 0);
    cout << "Номер 0: " << (invalidPerm.empty() ? "OK" : "FAIL")
         << endl;

    cout << "\nЭксперимент завершен!" << endl;

    return 0;
}
