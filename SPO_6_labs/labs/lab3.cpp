#include <iostream>
#include <string>
#include <sstream>
#include <limits>

using namespace std;

namespace {
    const size_t N = 80;

    int pos_mas(const char s[], const char s1[]) {
        if (!s || !s1) return -1;

        for (size_t i = 0;; ++i) {
            size_t j = 0;

            while (s1[j] != '\0' && s[i + j] != '\0' &&
                s[i + j] == s1[j]) {
                ++j;
            }

            if (s1[j] == '\0') return static_cast<int>(i);
            if (s[i] == '\0') return -1;
        }
    }

    int pos_ptr(const char* s, const char* s1) {
        if (!s || !s1) return -1;

        const char* start = s;

        while (true) {
            const char* p = s;
            const char* q = s1;

            while (*q != '\0' && *p != '\0' && *p == *q) {
                ++p;
                ++q;
            }

            if (*q == '\0') return static_cast<int>(s - start);
            if (*s == '\0') return -1;

            ++s;
        }
    }

    bool readString(const string& message, string& value) {
        while (true) {
            cout << message;

            if (!getline(cin, value)) return false;

            if (value.size() <= N) return true;

            cout << "Ошибка: длина строки не должна превышать 80 байт.\n";
        }
    }

    bool readChoice(int& choice) {
        string line;

        while (true) {
            cout << "\n1. Выполнить новый поиск\n";
            cout << "0. Вернуться в главное меню\n";
            cout << "Выберите действие: ";

            if (!getline(cin, line)) return false;

            istringstream input(line);
            int number;
            string extra;

            if (input >> number && !(input >> extra) &&
                (number == 0 || number == 1)) {
                choice = number;
                return true;
            }

            cout << "Некорректный ввод. Введите 0 или 1.\n";
        }
    }
}

void lab3() {
    while (true) {
        string s, s1;

        cout << "\n=== ЛАБОРАТОРНАЯ РАБОТА №3 ===\n";
        cout << "Поиск первого вхождения подстроки в строку (типа strstr).\n";
        cout << "Индексация символов начинается с 0.\n";

        if (!readString("\nВведите исходную строку: ", s)) return;
        cout << "Проверка: " << s << '\n';
        if (!readString("Введите искомую подстроку: ", s1)) return;

        int resultMas = pos_mas(s.c_str(), s1.c_str());
        int resultPtr = pos_ptr(s.c_str(), s1.c_str());

        cout << "\nРезультат обработки массивом: ";

        if (resultMas == -1)
            cout << "подстрока не найдена.\n";
        else
            cout << "первое вхождение с позиции " << resultMas << ".\n";

        cout << "Результат обработки указателями: ";

        if (resultPtr == -1)
            cout << "подстрока не найдена.\n";
        else
            cout << "первое вхождение с позиции " << resultPtr << ".\n";

        int choice;

        if (!readChoice(choice) || choice == 0) return;
    }
}