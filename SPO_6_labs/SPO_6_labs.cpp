// SPO_6_labs.cpp: определяет точку входа для приложения.
//

#include "SPO_6_labs.h"
#include <iostream>
#include <limits>
#include <clocale>

extern "C" __declspec(dllimport) int __stdcall SetConsoleCP(unsigned int);
extern "C" __declspec(dllimport) int __stdcall SetConsoleOutputCP(unsigned int);

using namespace std;

int main() {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    setlocale(LC_ALL, "Russian");

    int choice;

    while (true) {
        cout << "\n========== ЛАБОРАТОРНЫЕ РАБОТЫ ==========\n";
        cout << "1. Лабораторная работа №1\n";
        cout << "2. Лабораторная работа №2\n";
        cout << "3. Лабораторная работа №3\n";
        cout << "4. Лабораторная работа №4\n";
        cout << "5. Лабораторная работа №5\n";
        cout << "6. Лабораторная работа №6\n";
        cout << "0. Выход\n";
        cout << "=========================================\n";
        cout << "Выберите пункт: ";

        if (!(cin >> choice)) {
            cout << "Некорректный ввод. Введите число от 0 до 6.\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }

        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        if (choice == 0) {
            cout << "Завершение программы.\n";
            break;
        }

        switch (choice) {
        case 1:
            lab1();
            break;
        case 2:
            lab2();
            break;
        case 3:
            lab3();
            break;
        case 4:
            lab4();
            break;
        case 5:
            lab5();
            break;
        case 6:
            lab6();
            break;
        default:
            cout << "Лабораторная работа с таким номером отсутствует.\n";
            continue;
        }

        cout << "\nЛабораторная работа завершена.\n";
    }

    return 0;
}