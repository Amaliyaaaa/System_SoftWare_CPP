#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <limits>
#include <ctime>
#include <cstdint>
#include <new>
#include <cctype>
#include <cstdlib>

using namespace std;

namespace {
    enum class Relation {
        Friend = 1,
        Colleague,
        Relative,
        Acquaintance
    };

    struct BirthDate {
        unsigned day : 5;
        unsigned month : 4;
        unsigned year : 12;
    };

    struct Contact {
        string surname;
        string name;
        string patronymic;
        string address;
        string phone;
        string organization;
        string position;
        string qualities;
        BirthDate birthday;
        Relation relation;
    };

    bool readLine(const string& prompt, string& value, bool allowEmpty = false) {
        while (true) {
            cout << prompt;

            if (!getline(cin, value)) return false;

            if (value.find_first_not_of(" \t\r\n") != string::npos || allowEmpty)
                return true;

            cout << "Поле не может быть пустым.\n";
        }
    }

    bool readNumber(const string& prompt, long long& value) {
        string line;

        while (true) {
            if (!readLine(prompt, line, true)) return false;

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

    bool leapYear(int year) {
        return year % 400 == 0 || (year % 4 == 0 && year % 100 != 0);
    }

    int daysInMonth(int month, int year) {
        const int days[] = { 31, 28, 31, 30, 31, 30,
                             31, 31, 30, 31, 30, 31 };

        if (month == 2 && leapYear(year)) return 29;
        return days[month - 1];
    }

    bool readBirthDate(BirthDate& date) {
    time_t now = time(nullptr);
    tm today{};

#ifdef _WIN32
    if (localtime_s(&today, &now) != 0) {
#else
    if (!localtime_r(&now, &today)) {
#endif
        cout << "Не удалось получить текущую дату.\n";
        return false;
    }

    int currentYear = today.tm_year + 1900;
    int day, month, year;

    while (true) {
        if (!readRange(
            "Год рождения (1900.." + to_string(currentYear) + "): ",
            1900, currentYear, year))
            return false;

        if (!readRange("Месяц рождения (1..12): ", 1, 12, month))
            return false;

        if (!readRange("День рождения (1..31): ", 1, 31, day))
            return false;

        if (day > daysInMonth(month, year)) {
            cout << "Такой даты не существует. Повторите ввод.\n";
            continue;
        }

        if (year == currentYear &&
            (month > today.tm_mon + 1 ||
                (month == today.tm_mon + 1 && day > today.tm_mday))) {
            cout << "Дата рождения не может быть позже текущей даты. Повторите ввод.\n";
            continue;
        }

        date.year = year;
        date.month = month;
        date.day = day;
        return true;
    }
}

    string relationName(Relation relation) {
        switch (relation) {
        case Relation::Friend: return "Друг";
        case Relation::Colleague: return "Коллега";
        case Relation::Relative: return "Родственник";
        case Relation::Acquaintance: return "Знакомый";
        }

        return "Неизвестно";
    }

    bool readRelation(Relation& relation) {
        cout << "\nХарактер знакомства:\n";
        cout << "1. Друг\n";
        cout << "2. Коллега\n";
        cout << "3. Родственник\n";
        cout << "4. Знакомый\n";

        int choice;

        if (!readRange("Выберите вариант: ", 1, 4, choice))
            return false;

        relation = static_cast<Relation>(choice);
        return true;
    }

    void printContact(const Contact& contact, size_t number) {
        cout << "\nКонтакт №" << number << '\n';
        cout << "ФИО: " << contact.surname << ' '
            << contact.name << ' ' << contact.patronymic << '\n';
        cout << "Дата рождения: "
            << contact.birthday.day << '.'
            << contact.birthday.month << '.'
            << contact.birthday.year << '\n';
        cout << "Адрес: " << contact.address << '\n';
        cout << "Телефон: " << contact.phone << '\n';
        cout << "Место работы или учёбы: " << contact.organization << '\n';
        cout << "Должность: " << contact.position << '\n';
        cout << "Характер знакомства: "
            << relationName(contact.relation) << '\n';
        cout << "Деловые качества: " << contact.qualities << '\n';
    }

    bool addContact(vector<Contact>& contacts) {
        Contact contact;

        cout << "\nДобавление контакта\n";

        if (!readLine("Фамилия: ", contact.surname)) return false;

        cout << '\n';
        cout << "Проверка фамилии: " << contact.surname << '\n';
        if (!readLine("Имя: ", contact.name)) return false;
        if (!readLine("Отчество (при отсутствии нажмите Enter): ",
            contact.patronymic, true)) return false;

        if (!readBirthDate(contact.birthday)) return false;
        if (!readLine("Адрес: ", contact.address, true)) return false;
        if (!readLine("Телефон: ", contact.phone, true)) return false;
        if (!readLine("Место работы или учёбы: ",
            contact.organization, true)) return false;
        if (!readLine("Должность: ", contact.position, true)) return false;
        if (!readRelation(contact.relation)) return false;
        if (!readLine("Деловые качества: ",
            contact.qualities, true)) return false;

        contacts.push_back(contact);
        cout << "Контакт добавлен.\n";
        return true;
    }

    string lowerAscii(string value) {
        for (char& c : value) {
            unsigned char ch = static_cast<unsigned char>(c);

            if (ch >= 'A' && ch <= 'Z')
                c = static_cast<char>(ch + ('a' - 'A'));
        }

        return value;
    }

    bool searchContacts(const vector<Contact>& contacts) {
        string surname;

        if (!readLine("Введите фамилию для поиска: (регистр важен)", surname))
            return false;

        bool found = false;

        for (size_t i = 0; i < contacts.size(); ++i) {
            if (lowerAscii(contacts[i].surname) == lowerAscii(surname)) {
                printContact(contacts[i], i + 1);
                found = true;
            }
        }

        if (!found) cout << "Контакты с такой фамилией не найдены.\n";

        return true;
    }

    void printBirthdays(const vector<Contact>& contacts) {
        time_t now = time(nullptr);
        tm today{};

#ifdef _WIN32
        if (localtime_s(&today, &now) != 0) {
#else
        if (!localtime_r(&now, &today)) {
#endif
            cout << "Не удалось получить текущую дату.\n";
            return;
        }

        int day = today.tm_mday;
        int month = today.tm_mon + 1;

        cout << "\nПоздравления на " << day << '.' << month
            << '.' << today.tm_year + 1900 << ":\n";

        bool found = false;

        for (const Contact& contact : contacts) {
            if (contact.birthday.day == day &&
                contact.birthday.month == month) {
                cout << "\nС днём рождения, " << contact.name
                    << "! Желаем счастья, здоровья и успехов!\n";
                found = true;
            }
        }

        if (!found)
            cout << "Сегодня дней рождения среди контактов нет.\n";
        }

    void checkDivisibility() {
        long long number;

        if (!readNumber("\nВведите целое число A: ", number))
            return;

        uint64_t bits = static_cast<uint64_t>(number);

        cout << "A & 15 = " << (bits & 15ULL) << '\n';

        if ((bits & 15ULL) == 0)
            cout << "Число " << number << " кратно 16.\n";
        else
            cout << "Число " << number << " не кратно 16.\n";
    }
    }

    void lab4() {

        vector<Contact> contacts;

    while (true) {
        cout << "\n=== ЛАБОРАТОРНАЯ РАБОТА №4 ===\n";
        cout << "1. Добавить контакт\n";
        cout << "2. Просмотреть все контакты\n";
        cout << "3. Найти контакт по фамилии\n";
        cout << "4. Сформировать поздравления с днём рождения\n";
        cout << "5. Проверить кратность числа 16\n";
        cout << "6. Запустить заново\n";
        cout << "0. Вернуться в главное меню\n";

        int choice;

        if (!readRange("Выберите действие: ", 0, 6, choice))
            return;

        switch (choice) {
        case 1:
            if (!addContact(contacts)) return;
            break;

        case 2:
            if (contacts.empty()) {
                cout << "Записная книжка пуста.\n";
            }
            else {
                for (size_t i = 0; i < contacts.size(); ++i)
                    printContact(contacts[i], i + 1);
            }
            break;

        case 3:
            if (!searchContacts(contacts)) return;
            break;

        case 4:
            printBirthdays(contacts);
            break;

        case 5:
            checkDivisibility();
            if (!cin) return;
            break;

        case 6:
            contacts.clear();
            cout << "Лабораторная работа запущена заново. Записная книжка очищена.\n";
            break;

        case 0:
            return;
        }
    }
}