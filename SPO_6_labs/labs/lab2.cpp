#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>
#include <limits>
#include <memory>
#include <new>
#include <cstddef>
#include <utility>

using namespace std;

namespace {
    int NN = 0;
    size_t SIZE = 0;
    unique_ptr<int[]> m_addr;
    int L2_RESULT = 0;

    bool readInt(const string& message, int& value) {
        string line;

        while (true) {
            cout << message;

            if (!getline(cin, line)) return false;

            istringstream input(line);
            long long number;
            string extra;

            if (input >> number && !(input >> extra) &&
                number >= numeric_limits<int>::min() &&
                number <= numeric_limits<int>::max()) {
                value = static_cast<int>(number);
                return true;
            }

            cout << "Некорректный ввод. Введите целое число.\n";
        }
    }

    bool ch_coord(int x, int y) {
        if (!m_addr || x < 0 || y < 0 || x >= NN || y >= NN) {
            L2_RESULT = -1;
            return false;
        }

        L2_RESULT = 0;
        return true;
    }

    size_t lin(int x, int y) {
        size_t row = static_cast<size_t>(x);
        size_t n = static_cast<size_t>(NN);

        return row * (n / 2) + (n % 2) * ((row + 1) / 2)
            + static_cast<size_t>(y) / 2;
    }

    int close_matr() {
        if (!m_addr) {
            L2_RESULT = -1;
            return -1;
        }

        m_addr.reset();
        NN = 0;
        SIZE = 0;
        L2_RESULT = 0;
        return 0;
    }

    int creat_matr(int N) {
        if (N < 1 || N > 100) {
            L2_RESULT = -1;
            return -1;
        }

        size_t n = static_cast<size_t>(N);
        size_t count = (n * n + 1) / 2;

        unique_ptr<int[]> data(new (nothrow) int[count]());

        if (!data) {
            L2_RESULT = -1;
            return -1;
        }

        m_addr = move(data);
        NN = N;
        SIZE = count;
        L2_RESULT = 0;
        return 0;
    }

    int read_matr(int x, int y) {
        if (!ch_coord(x, y)) return 0;

        if ((x + y) % 2 != 0) return 0;

        return m_addr[lin(x, y)];
    }

    int write_matr(int x, int y, int value) {
        if (!ch_coord(x, y)) return 0;

        if ((x + y) % 2 != 0) {
            if (value != 0) {
                L2_RESULT = -1;
                return 0;
            }

            return 0;
        }

        m_addr[lin(x, y)] = value;
        return value;
    }

    void print_matr() {
        cout << "\nМатрица " << NN << " x " << NN << ":\n\n";

        for (int x = 0; x < NN; ++x) {
            for (int y = 0; y < NN; ++y) {
                cout << setw(12) << read_matr(x, y);
            }
            cout << '\n';
        }

        cout << "\nВсего элементов: "
            << static_cast<size_t>(NN) * NN;
        cout << "\nХранится элементов: " << SIZE;
        cout << "\nЭкономия памяти: "
            << (static_cast<size_t>(NN) * NN - SIZE) * sizeof(int)
            << " байт\n";
    }

    bool readCoordinates(int& x, int& y) {
        if (!readInt("Введите индекс строки: ", x)) return false;
        if (!readInt("Введите индекс столбца: ", y)) return false;
        return true;
    }
}

void lab2() {

    while (true) {
        int N;

        while (true) {
            if (!readInt("\nВведите размерность квадратной матрицы (1..100): ", N))
                return;

            if (N >= 1 && N <= 100) break;

            cout << "Размерность должна быть от 1 до 100.\n";
        }

        if (creat_matr(N) == -1) {
            cout << "Не удалось создать матрицу.\n";
            return;
        }

        int value = 1;

        for (int x = 0; x < N; ++x) {
            for (int y = 0; y < N; ++y) {
                if ((x + y) % 2 == 0) {
                    write_matr(x, y, value++);
                }
            }
        }

        print_matr();

        bool recreate = false;

        while (true) {
            cout << "\n=== ЛАБОРАТОРНАЯ РАБОТА №2 ===\n";
            cout << "1. Прочитать элемент матрицы\n";
            cout << "2. Записать элемент матрицы\n";
            cout << "3. Вывести матрицу\n";
            cout << "4. Создать новую матрицу\n";
            cout << "0. Вернуться в главное меню\n";

            int choice;

            if (!readInt("Выберите действие: ", choice)) {
                close_matr();
                return;
            }

            if (choice == 0) {
                close_matr();
                return;
            }

            if (choice == 4) {
                recreate = true;
                break;
            }

            if (choice == 3) {
                print_matr();
                continue;
            }

            if (choice != 1 && choice != 2) {
                cout << "Такого пункта меню нет.\n";
                continue;
            }

            int x, y;

            if (!readCoordinates(x, y)) {
                close_matr();
                return;
            }

            if (choice == 1) {
                int result = read_matr(x, y);

                if (L2_RESULT == -1) {
                    cout << "Ошибка: неверные координаты.\n";
                }
                else {
                    cout << "Элемент [" << x << "][" << y
                        << "] = " << result << '\n';
                }
            }
            else {
                int number;

                if (!readInt("Введите новое значение: ", number)) {
                    close_matr();
                    return;
                }

                write_matr(x, y, number);

                if (L2_RESULT == -1) {
                    if (x < 0 || y < 0 || x >= NN || y >= NN) {
                        cout << "Ошибка: неверные координаты.\n";
                    }
                    else {
                        cout << "Ошибка: в эту ячейку нельзя записать ненулевое значение.\n";
                    }
                }
                else {
                    cout << "Значение записано.\n";
                }
            }
        }

        close_matr();

        if (!recreate) return;
    }
}