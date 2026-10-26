#include <iostream>
#include <string>
#include <iomanip>
#include <cstdint>
#include <limits>
#include <clocale>
using namespace std;

const int WIDTH = 32;

string unsignedBinary(uint32_t n) {
    string result(WIDTH, '0');
    for (int i = WIDTH - 1; i >= 0; --i) {
        result[i] = char('0' + n % 2);
        n /= 2;
    }
    return result;
}

string binaryAdd(const string& a, const string& b) {
    string result(WIDTH, '0');
    int carry = 0;
    for (int i = WIDTH - 1; i >= 0; --i) {
        int sum = (a[i] - '0') + (b[i] - '0') + carry;
        result[i] = char('0' + sum % 2);
        carry = sum / 2;
    }
    return result;
}

string negateBinary(string bits) {
    for (char& bit : bits) bit = (bit == '0') ? '1' : '0';
    return binaryAdd(bits, unsignedBinary(1));
}

string decimalToBinary(int32_t value) {
    if (value >= 0) return unsignedBinary(static_cast<uint32_t>(value));
    return negateBinary(unsignedBinary(static_cast<uint32_t>(-static_cast<int64_t>(value))));
}

string binarySubtract(const string& a, const string& b) {
    return binaryAdd(a, negateBinary(b));
}

int64_t binaryToDecimal(const string& bits) {
    uint64_t value = 0;
    for (char bit : bits) value = value * 2 + (bit - '0');
    if (bits[0] == '1') return static_cast<int64_t>(value) - (int64_t(1) << WIDTH);
    return static_cast<int64_t>(value);
}

int readNumber(const string& name) {
    int value;
    while (true) {
        cout << name << " [-32767..32767]: ";
        if (cin >> value && value >= -32767 && value <= 32767) return value;
        cout << "Некорректное число. Повторите ввод.\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
}

uint32_t readHex16(const string& name) {
    string input;
    while (true) {
        cout << name << " (шестнадцатеричное значение, 0000..FFFF): ";
        cin >> input;
        if (input.empty() || input.size() > 4) {
            cout << "Некорректное шестнадцатеричное значение.\n";
            continue;
        }
        uint32_t value = 0;
        bool ok = true;
        for (char c : input) {
            int digit;
            if (c >= '0' && c <= '9') digit = c - '0';
            else if (c >= 'a' && c <= 'f') digit = c - 'a' + 10;
            else if (c >= 'A' && c <= 'F') digit = c - 'A' + 10;
            else { ok = false; break; }
            value = value * 16 + digit;
        }
        if (ok) return value;
        cout << "Некорректное шестнадцатеричное значение.\n";
    }
}

void printBinary(const string& bits) {
    for (int i = 0; i < WIDTH; ++i) {
        cout << bits[i];
        if (i % 8 == 7 && i != WIDTH - 1) cout << ' ';
    }
}

void lab1() {
    setlocale(LC_ALL, "Russian");

    while (true) {
        cout << "\n=== ЛАБОРАТОРНАЯ РАБОТА №1 ===\n";
        cout << "Работа с двоичными и шестнадцатеричными числами.\n";

        int a = readNumber("A");
        int b = readNumber("B");
        int c = readNumber("C");
        int d = readNumber("D");

        string binary[5] = {
            decimalToBinary(a), decimalToBinary(b),
            decimalToBinary(c), decimalToBinary(d), ""
        };
        binary[4] = binarySubtract(binarySubtract(binary[1], binary[2]), binary[0]);

        int64_t values[5] = { a, b, c, d, binaryToDecimal(binary[4]) };
        string names[5] = { "A", "B", "C", "D", "X" };

        cout << "\nX = B - C - A = " << values[4] << "\n\n";
        cout << "Переменная | Десятичное число   | Двоичное представление (32 бита)\n";

        for (int i = 0; i < 5; ++i) {
            cout << setw(10) << names[i] << " | "
                << setw(18) << dec << values[i] << " | ";
            printBinary(binary[i]);
            cout << '\n';
        }

        uint32_t segment, offset;

        while (true) {
            cout << "\nВведите адрес переменной A:\n";
            segment = readHex16("Сегмент");
            offset = readHex16("Смещение");

            if (offset > 0xFFFF - 19) {
                cout << "20 байт не помещаются в пределах сегмента. Укажите меньшее смещение.\n";
                continue;
            }

            if (segment * 16 + offset + 19 > 0xFFFFF) {
                cout << "Адреса переменных выходят за пределы 20-битного адресного пространства. Повторите ввод.\n";
                continue;
            }

            break;
        }

        uint32_t base = segment * 16 + offset;

        cout << "\nРасчёт адресов: физический адрес = сегмент * 10h + смещение\n";
        cout << uppercase << hex << setfill('0');

        for (int i = 0; i < 5; ++i) {
            uint32_t currentOffset = offset + 4 * i;
            uint32_t physical = base + 4 * i;

            cout << names[i] << ": "
                << setw(4) << segment << ':'
                << setw(4) << currentOffset << " -> "
                << setw(6) << physical << "h\n";
        }

        cout << "\nДамп памяти (порядок байтов от младшего к старшему, 4 байта на переменную):\n";
        cout << "Адрес   | Б0  Б1  Б2  Б3 | Переменная\n";

        for (int i = 0; i < 5; ++i) {
            cout << setw(6) << (base + 4 * i) << "h | ";

            for (int byte = 0; byte < 4; ++byte) {
                int start = WIDTH - 8 * (byte + 1);
                unsigned value = 0;

                for (int bit = start; bit < start + 8; ++bit) {
                    value = value * 2 + (binary[i][bit] - '0');
                }

                cout << setw(2) << value << (byte == 3 ? "" : "  ");
            }

            cout << " | " << names[i] << '\n';
        }

        cout << dec << nouppercase << setfill(' ');

        int choice;

        while (true) {
            cout << "\n1. Запустить заново\n";
            cout << "0. Вернуться в главное меню\n";
            cout << "Выберите действие: ";

            string line;

            if (!getline(cin >> ws, line)) return;

            if (line == "0") return;
            if (line == "1") break;

            cout << "Некорректный ввод. Введите 0 или 1.\n";
        }
    }
}