
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <iomanip>
#include <stdexcept>
#include <locale>
#include "../dll/FuzzyRelations.h"

using namespace std;

namespace {
    bool readInteger(const string& prompt, int minimum, int maximum, int& value) {
        string line;

        while (true) {
            cout << prompt;

            if (!getline(cin, line)) return false;

            istringstream input(line);
            long long number;
            string extra;

            if (input >> number && !(input >> extra) &&
                number >= minimum && number <= maximum) {
                value = static_cast<int>(number);
                return true;
            }

            cout << "Введите целое число от " << minimum
                << " до " << maximum << ".\n";
        }
    }

    bool readDouble(const string& prompt, double& value) {
        string line;

        while (true) {
            cout << prompt;

            if (!getline(cin, line)) return false;

            istringstream input(line);
            input.imbue(locale::classic());

            double number;
            string extra;

            if (input >> number && !(input >> extra) &&
                number >= 0.0 && number <= 1.0) {
                value = number;
                return true;
            }

            cout << "Введите число от 0 до 1, используя точку.\n";
        }
    }

    bool readMatrix(const string& name, vector<double>& matrix,
        int rows, int columns) {

        cout << "\nМатрица " << name << " (" << rows
            << " x " << columns << "):\n";

        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < columns; ++j) {
                if (!readDouble(
                    name + "[" + to_string(i) + "][" +
                    to_string(j) + "]: ",
                    matrix[i * columns + j]))
                    return false;
            }
        }

        return true;
    }

    void printMatrix(const string& name,
        const vector<double>& matrix, int rows, int columns) {

        cout << "\n" << name << ":\n";
        cout << fixed << setprecision(3);

        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < columns; ++j)
                cout << setw(8) << matrix[i * columns + j];

            cout << '\n';
        }

        cout << defaultfloat;
    }

    bool runComposition(int operation) {
        int m, n, p;

        cout << "\nРазмеры матриц: A (m x n), B (n x p)\n";

        if (!readInteger("Введите m (1..6): ", 1, 6, m)) return false;
        if (!readInteger("Введите n (1..6): ", 1, 6, n)) return false;
        if (!readInteger("Введите p (1..6): ", 1, 6, p)) return false;

        vector<double> a(m * n);
        vector<double> b(n * p);
        vector<double> result(m * p);

        if (!readMatrix("A", a, m, n)) return false;
        if (!readMatrix("B", b, n, p)) return false;

        try {
            switch (operation) {
            case 1:
                Fuzzy::Relations::MaxMin(
                    a.data(), b.data(), result.data(), m, n, p);
                break;

            case 2:
                Fuzzy::Relations::MinMax(
                    a.data(), b.data(), result.data(), m, n, p);
                break;

            case 3:
                Fuzzy::Relations::MaxProduct(
                    a.data(), b.data(), result.data(), m, n, p);
                break;
            }
        }
        catch (const exception& error) {
            cout << "Ошибка: " << error.what() << '\n';
            return true;
        }

        printMatrix("Исходная матрица A", a, m, n);
        printMatrix("Исходная матрица B", b, n, p);
        printMatrix("Результат композиции", result, m, p);

        return true;
    }
}

void lab6() {
    while (true) {
        cout << "\n=== ЛАБОРАТОРНАЯ РАБОТА №6 ===\n";
        cout << "1. Максиминная композиция\n";
        cout << "2. Минимаксная композиция\n";
        cout << "3. Максмультипликативная композиция\n";
        cout << "0. Вернуться в главное меню\n";

        int choice;

        if (!readInteger("Выберите действие: ", 0, 3, choice))
            return;

        if (choice == 0)
            return;

        if (!runComposition(choice))
            return;
    }
}
