
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <limits>
#include <cstdint>

using namespace std;

extern "C" void MAS_SWAP(int* array);

namespace {
    bool readNumber(const string& prompt, long long& value) {
        string line;

        while (true) {
            cout << prompt;

            if (!getline(cin, line)) return false;

            istringstream input(line);
            long long number;
            string extra;

            if (input >> number && !(input >> extra)) {
                value = number;
                return true;
            }

            cout << "Некорректный ввод. Введите целое число.\n";
        }
    }

    bool readRange(const string& prompt, int minimum, int maximum, int& value) {
        long long number;

        while (true) {
            if (!readNumber(prompt, number)) return false;

            if (number >= minimum && number <= maximum) {
                value = static_cast<int>(number);
                return true;
            }

            cout << "Введите число от " << minimum
                << " до " << maximum << ".\n";
        }
    }

    bool calculateExpression() {
        int a, b, c;

        cout << "\nВычисление выражения 6a - (3b - 2c)\n";

        if (!readRange("Введите a (-100000..100000): ",
            -100000, 100000, a)) return false;

        if (!readRange("Введите b (-100000..100000): ",
            -100000, 100000, b)) return false;

        if (!readRange("Введите c (-100000..100000): ",
            -100000, 100000, c)) return false;

        int result;

        __asm {
            mov eax, a
            imul eax, eax, 6

            mov ebx, b
            imul ebx, ebx, 3
            sub eax, ebx

            mov ecx, c
            imul ecx, ecx, 2
            add eax, ecx

            mov result, eax
        }

        cout << "Результат: 6 * " << a
            << " - (3 * " << b
            << " - 2 * " << c
            << ") = " << result << '\n';

        return true;
    }

    bool swapArrayElements() {
        int n;

        cout << "\nОбмен второго и третьего элементов массива\n";

        if (!readRange("Введите размер массива (3..100) (массив вручную вводится если что по одному элементу пользователем): ",
            3, 100, n)) return false;

        vector<int> array(n);

        for (int i = 0; i < n; ++i) {
            if (!readRange(
                "Элемент [" + to_string(i) + "]: ",
                numeric_limits<int>::min(),
                numeric_limits<int>::max(),
                array[i])) return false;
        }

        cout << "\nМассив до обмена: ";

        for (int value : array)
            cout << value << ' ';

        cout << '\n';

        MAS_SWAP(array.data());

        cout << "Массив после обмена: ";

        for (int value : array)
            cout << value << ' ';

        cout << '\n';

        return true;
    }
}

void lab5() {
    while (true) {
        cout << "\n=== ЛАБОРАТОРНАЯ РАБОТА №5 ===\n";
        cout << "1. Вычислить 6a - (3b - 2c)\n";
        cout << "2. Поменять местами элементы массива с индексами 1 и 2\n";
        cout << "0. Вернуться в главное меню\n";

        int choice;

        if (!readRange("Выберите действие: ", 0, 2, choice))
            return;

        switch (choice) {
        case 1:
            if (!calculateExpression()) return;
            break;

        case 2:
            if (!swapArrayElements()) return;
            break;

        case 0:
            return;
        }
    }
}
