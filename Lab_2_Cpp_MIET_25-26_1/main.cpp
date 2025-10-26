#include "prepod.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <cstring>
#include <locale.h>
#include <windows.h>

using namespace std;

const char* DATABASE_FILE = "prepod_database.dat";

inline void WaitEnter() { 
    cout << "Press Enter to continue...";
    Sleep(100);
    std::cin.ignore(80, '\n');
    std::cin.get();
}

void saveToFile(Prepod** database, int size) {
    ofstream file(DATABASE_FILE, ios::binary);
    if (!file) {
        cout << "Ошибка открытия файла для записи!" << endl;
        return;
    }

    file.write(reinterpret_cast<char*>(&size), sizeof(int));

    for (int i = 0; i < size; i++) {
        int famLen = strlen(database[i]->getFam()) + 1;
        int nameLen = strlen(database[i]->getName()) + 1;
        int status = database[i]->getStatus();

        file.write(reinterpret_cast<char*>(&famLen), sizeof(int));
        file.write(database[i]->getFam(), famLen);

        file.write(reinterpret_cast<char*>(&nameLen), sizeof(int));
        file.write(database[i]->getName(), nameLen);

        file.write(reinterpret_cast<char*>(&status), sizeof(int));
    }

    file.close();
    cout << "База данных сохранена в файл." << endl;
}

Prepod** loadFromFile(int& size) {
    ifstream file(DATABASE_FILE, ios::binary);
    if (!file) {
        size = 0;
        return nullptr;
    }

    file.read(reinterpret_cast<char*>(&size), sizeof(int));

    Prepod** database = new Prepod * [size];

    for (int i = 0; i < size; i++) {
        int famLen, nameLen, status;

        file.read(reinterpret_cast<char*>(&famLen), sizeof(int));
        char* fam = new char[famLen];
        file.read(fam, famLen);

        file.read(reinterpret_cast<char*>(&nameLen), sizeof(int));
        char* name = new char[nameLen];
        file.read(name, nameLen);

        file.read(reinterpret_cast<char*>(&status), sizeof(int));

        database[i] = new Prepod(fam, name, status);

        delete[] fam;
        delete[] name;
    }

    file.close();
    cout << "База данных загружена из файла. Записей: " << size << endl;
    return database;
}

void displayAll(Prepod** database, int size) {
    if (size == 0) {
        cout << "База данных пуста." << endl;
        return;
    }

    cout << "\n=== Все преподаватели ===" << endl;
    for (int i = 0; i < size; i++) {
        cout << i + 1 << ". ";
        database[i]->display();
    }
    cout << "Всего записей: " << size << endl;
    cout << "Количество существующих объектов: " << Prepod::getCount() << endl;
    WaitEnter();
}

void searchByName(Prepod** database, int size) {
    char surname[100], firstName[100];
    cout << "Введите фамилию: ";
    cin >> surname;
    cout << "Введите имя: ";
    cin >> firstName;

    bool found = false;
    cout << "\n=== Результаты поиска ===" << endl;
    for (int i = 0; i < size; i++) {
        if (strcmp(database[i]->getFam(), surname) == 0 &&
            strcmp(database[i]->getName(), firstName) == 0) {
            database[i]->display();
            found = true;
        }
    }

    if (!found) {
        cout << "Преподаватель не найден." << endl;
    }
    WaitEnter();
}

void searchByStatus(Prepod** database, int size) {
    int status;
    cout << "Введите ученое звание (1-преподаватель, 2-доцент, 3-доктор наук): ";
    cin >> status;

    if (status < 1 || status > 3) {
        cout << "Неверное значение звания!" << endl;
        return;
    }

    bool found = false;
    cout << "\n=== Преподаватели со званием: " << Prepod::getStatusName(status) << " ===" << endl;
    for (int i = 0; i < size; i++) {
        if (database[i]->getStatus() == status) {
            database[i]->display();
            found = true;
        }
    }

    if (!found) {
        cout << "Преподаватели с таким званием не найдены." << endl;
    }
    WaitEnter();
}

Prepod** addPrepod(Prepod** database, int& size) {
    char surname[100], firstName[100];
    int status;

    cout << "Введите фамилию: ";
    cin >> surname;
    cout << "Введите имя: ";
    cin >> firstName;
    cout << "Введите ученое звание (1-преподаватель, 2-доцент, 3-доктор наук): ";
    cin >> status;

    if (status < 1 || status > 3) {
        cout << "Неверное значение звания!" << endl;
        return database;
    }

    Prepod** newDatabase = new Prepod * [size + 1];
    for (int i = 0; i < size; i++) {
        newDatabase[i] = database[i];
    }

    newDatabase[size] = new Prepod(surname, firstName, status);

    delete[] database;
    size++;

    cout << "Преподаватель добавлен!" << endl;
    return newDatabase;
}

int main() {
    setlocale(LC_ALL, "Russian");
    int size = 0;
    Prepod** database = loadFromFile(size);

    if (database == nullptr) {
        cout << "Файл базы данных не найден. Создание новой базы." << endl;
        cout << "Введите количество начальных записей: ";
        int initialSize;
        cin >> initialSize;

        if (initialSize > 0) {
            database = new Prepod * [initialSize];
            size = initialSize;

            for (int i = 0; i < initialSize; i++) {
                cout << "\n--- Запись " << (i + 1) << " ---" << endl;
                char surname[100], firstName[100];
                int status;

                cout << "Введите фамилию: ";
                cin >> surname;
                cout << "Введите имя: ";
                cin >> firstName;
                cout << "Введите ученое звание (1-преподаватель, 2-доцент, 3-доктор наук): ";
                cin >> status;

                if (status < 1 || status > 3) status = 1;

                database[i] = new Prepod(surname, firstName, status);
            }
        }
        else {
            database = new Prepod * [0];
        }
    }

    int choice;
    do {
        system("cls");
        cout << "\n========== МЕНЮ ==========" << endl;
        cout << "1. Показать всех преподавателей" << endl;
        cout << "2. Добавить преподавателя" << endl;
        cout << "3. Поиск по имени и фамилии" << endl;
        cout << "4. Поиск по ученому званию" << endl;
        cout << "5. Показать количество объектов" << endl;
        cout << "0. Выход" << endl;
        cout << "=========================" << endl;
        cout << "Выберите действие: ";
        cin >> choice;

        system("cls");

        switch (choice) {
        case 1:
            displayAll(database, size);
            break;
        case 2:
            database = addPrepod(database, size);
            break;
        case 3:
            searchByName(database, size);
            break;
        case 4:
            searchByStatus(database, size);
            break;
        case 5:
            cout << "Количество существующих объектов Prepod: " << Prepod::getCount() << endl;
            break;
        case 0:
            saveToFile(database, size);
            cout << "Выход из программы." << endl;
            break;
        default:
            cout << "Неверный выбор!" << endl;
        }
    } while (choice != 0);

    for (int i = 0; i < size; i++) {
        delete database[i];
    }
    delete[] database;

    return 0;
}
